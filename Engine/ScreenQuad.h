#pragma once
#include "Entity.h"

namespace piolot
{

	/**
	 * \brief Quad is an Entity that is tied to the View Matrix instead of the World Space.
	 * 
	 * You should be able to specify an X and a Y in screen coordinates and get a Quad up in the Screen.
	 */
	class ScreenQuad : public Entity
	{



	public:

		/**
		 * \brief Create a Screen Quad at (x, y) coordinates.
		 * \param _name The Name of the Quad.
		 * \param _x X Coordinate in Screen Space
		 * \param _y Y Coordinate in Screen Space
		 */
		ScreenQuad(const std::string& _name, float _x, float _y);

	};

	
}