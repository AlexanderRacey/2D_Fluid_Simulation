#pragma once

// Namespace for constants
// - CAPS used to distinguish from other variables
namespace Constants
{
	// Application sizes
	extern const float WIDTH;
	extern const float HEIGHT;

	// Particles per meter of screenspace
	extern const float SCALE;
	extern const float WINDOW_SCALE;

	extern const int RENDER_WIDTH;
	extern const int RENDER_HEIGHT;

	// Timestep
	extern const float DELTATIME;

	// Number of particles along one vector dimension
	extern const int P_ROW_NUMBER;
	
	// Particle forces
	extern const float GRAVITY;
	extern const float STIFFNESS;
	extern const float VISCOSITY;
	extern const float TENSION;
	extern const float REST_DENSITY;

	// Other particle properties
	extern const float P_SPACING;
	extern const float P_VOLUME;
	extern const float P_MASS;
	extern const float RB_MASS;
	extern const float SIMULATION_KERNEL;
}