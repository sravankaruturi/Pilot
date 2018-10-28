#pragma once
#include "Entity.h"
#include "Object.h"
#include "FolderLocations.h"
#include "AssetManager.h"
#include <glm/gtc/matrix_transform.inl>
#include "SaveSceneHelpers.h"

#include "Terrain.h"

namespace piolot
{
	void Entity::SaveToFile(std::ofstream& _out)
	{

		// We need to save all the Data for the Entity.
		pe_helpers::store_strings(shaderName, _out);
		pe_helpers::store_strings(entityName, _out);
		pe_helpers::store_strings(objectName, _out);

		// Store the Bounding Box's minimum and maximum points.
		_out.write((char*)&(boundingBox.GetMinimumPoint()), sizeof(position));
		_out.write((char*)&(boundingBox.GetMaximumPoint()), sizeof(position));

		_out.write((char*)&selectedInScene, sizeof(bool));
		_out.write((char*)&position, sizeof(position));
		_out.write((char*)&rotation, sizeof(rotation));
		_out.write((char*)&scale, sizeof(scale));

	}

	void Entity::LoadFromFile(std::ifstream& _in)
	{

		// We need to save all the Data for the Entity.
		pe_helpers::read_strings(shaderName, _in);
		pe_helpers::read_strings(entityName, _in);
		pe_helpers::read_strings(objectName, _in);

		glm::vec3 min_point, max_point;

		// Store the Bounding Box's minimum and maximum points.
		_in.read((char*)&(min_point), sizeof(position));
		_in.read((char*)&(max_point), sizeof(position));

		this->boundingBox = BoundingBox(min_point, max_point);

		_in.read((char*)&selectedInScene, sizeof(bool));
		_in.read((char*)&position, sizeof(position));
		_in.read((char*)&rotation, sizeof(rotation));
		_in.read((char*)&scale, sizeof(scale));

	}

	void Entity::SetInitialPosition(const glm::vec3& _position, Terrain * _terrain)
	{
		position = _position;
		targetNode = _terrain->GetNodeIndicesFromPos(position.x, position.z);
		matrixDirty = true;
	}

	void Entity::UpdateMatrices()
	{
		glm::mat4 model_matrix(1.0f);
		model_matrix = glm::translate(model_matrix, position);
		model_matrix = glm::rotate(model_matrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		model_matrix = glm::rotate(model_matrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		model_matrix = glm::rotate(model_matrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		model_matrix = glm::scale(model_matrix, scale);
		modelMatrix = model_matrix;
	}

	Entity::Entity(const std::string& _entityName, const std::string& _objectPath, const std::string& _shaderName, glm::vec3 _boundingBoxLeast, glm::vec3 _boundingBoxHigh)
		:entityName(_entityName), shaderName(_shaderName), boundingBoxLeastVertex(_boundingBoxLeast), boundingBoxHighVertex(_boundingBoxHigh), boundingBox(BoundingBox(boundingBoxLeastVertex, boundingBoxHighVertex))
	{
		
		objectName = _objectPath.substr(_objectPath.find_last_of('/') + 1, _objectPath.find_last_of('.') - _objectPath.find_last_of('/') - 1);

		if (!ASMGR.IsObjectLoaded(objectName))
		{
			// This allocation would be cleared from the Asset Manager.
			std::shared_ptr<Object> object = std::make_shared<Object>(MODEL_FOLDER + _objectPath);
			ASMGR.objects.insert_or_assign(objectName, object);
		}

		// Anthaa ayyipoyaaka manam Bounding Box udpate cheyyali.
		// The Bounding Box can be created using the least vertex, the highest vertex, in terms of Both, X, Y, Z. Lets try it.
		Object * current_object = ASMGR.objects.at(objectName).get();
		//boundingBox = BoundingBox(current_object->GetHighestVertex(), current_object->GetHighestVertex());
		
		matrixDirty = true;

	}

	void Entity::Update(float _deltaTime)
	{
#if !ENABLE_GUI
		if ( matrixDirty)
		{
#endif
			UpdateMatrices();
#if !ENABLE_GUI
			matrixDirty = false;
		}
#endif
	}

	void Entity::Render()
	{

		PE_ASSERT(!shaderName.empty() && !objectName.empty());

		// Use the Current Shader.
		ASMGR.shaders.at(shaderName)->use();
		// Set the Model Matrix.
		ASMGR.shaders.at(shaderName)->setMat4("u_ModelMatrix", modelMatrix);

		// Render
		ASMGR.objects.at(objectName)->Render(shaderName);
		// Render.
		//object->Render(shaderName);

		ASMGR.shaders.at("bbox")->use();
		ASMGR.shaders.at("bbox")->setMat4("u_ModelMatrix", glm::scale(modelMatrix, glm::vec3(1.001f, 1.001f, 1.001f)));
		if ( selectedInScene )
		{
			boundingBox.Render(glm::vec3(1.0, 1.0, 0.0));
		}else
		{
			boundingBox.Render(glm::vec3(1.0, 0.0, 0.0));
		}
		
		
	}

	bool Entity::CheckIfMouseOvered(const glm::vec3 _cameraPosition, const glm::vec3 _mouseRayDirection, float& _distance) const
	{
		// Checks if this entity is under the cursor. And updates the _distance to hold it.
		return this->boundingBox.CheckForCollisionWithRay(this->modelMatrix, this->scale, _cameraPosition, _mouseRayDirection, _distance);
	}

	void Entity::DisplayDetailsImgui()
	{

		ImGui::InputFloat3("Position", glm::value_ptr(position));
		ImGui::InputFloat3("Rotation", glm::value_ptr(rotation));
		ImGui::InputFloat3("Scale", glm::value_ptr(scale));

		ImGui::Separator();

		// Show object details.
		ASMGR.objects.at(objectName)->MeshDetailsImGUI();

		ImGui::Separator();
		ImGui::Text("Pathing Debug");
		ImGui::DragInt2("Target Node", glm::value_ptr(this->targetNode));

		ImGui::Separator();
		ImGui::DragFloat("Health: ", &this->health);

	}

}
