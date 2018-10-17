#include "ScreenQuad.h"

namespace piolot
{

	ScreenQuad::ScreenQuad(const std::string& _name, float _x, float _y)
		: Entity(_name, "plane/plane.obj", "screenQuad")
	{

		this->SetScale(glm::vec3(.5, 0.5, 0.5));

	}

}
