#include <iostream>
#include "Constants.h"

/*
*	In future, a new implementation will have to discard the usage of constant variables, as this limits the user interaction with the simulation.
*	Used namespace Constants to avoid confusion with variable declaration 'const'
*/

// Constant values declaration
namespace Constants
{
	const float WIDTH = 3;
	const float HEIGHT = 1.5;
	const float SCALE = 400;
	const float WINDOW_SCALE = 1.0f;

	const int RENDER_WIDTH = SCALE * WIDTH;
	const int RENDER_HEIGHT = SCALE * HEIGHT;

	const float DELTATIME = 0.0001f;

	// Number affects frametime, need to optimise this
	// - MUST BE EVEN NUMBER TO AVOID VECTOR ERROR - see Fluid::Fluid()
	const int P_ROW_NUMBER = 36;

	const float GRAVITY = 12000;
	const float STIFFNESS = 10000;
	const float VISCOSITY = 12000;
	const float TENSION = 10000.0f;
	const float REST_DENSITY = 1000;

	const float P_SPACING = 1.0f / P_ROW_NUMBER;
	const float P_VOLUME = P_SPACING * P_SPACING;
	const float P_MASS = P_VOLUME * REST_DENSITY;
	const float RB_MASS = P_MASS * 2;

	const float SIMULATION_KERNEL = 2 * P_SPACING;
}