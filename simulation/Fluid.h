#pragma once
#include <vector>

#include "Constants.h"
#include "Particle.h"
#include "Grid.h"

// Declare different displays
enum class Display
{
	Particles,
	Water,
	Velocity,
	Density,
	Pressure
};

// Fluid Class
class Fluid
{
	public:
		Fluid();

		void update(float dt, Display display);
		void render(sf::RenderTarget& rt, Display display);

		void repulsionForce(sf::Vector2f position);
		void attractionForce(sf::Vector2f position);

	private:
		int particleAmount;
		std::vector<Particle> particles;
		std::vector<std::vector<int>> particleAdjacents;
		Grid particleGrid;

		// Kernel variables
		float kernel(sf::Vector2f x, float h);
		sf::Vector2f gradKernel(sf::Vector2f x, float h);
		float lapKernel(sf::Vector2f x, float h);

		// Calculating particle movement
		void findAdjacents();
		void pCalcDensity();
		void pCalcPressure();
		void pCalcForceDensity();
		void pCalcIntegration(float dt);
		void pCalcCollision();

		// Particle appearance 
		void setParticleColours(Display display);
		sf::Texture metaballTexture;
};