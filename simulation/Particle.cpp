#include "Particle.h"
#include "Constants.h"

// Create particles
Particle::Particle()
{
	Particle(Vector2f());
}

// Define force values
Particle::Particle(sf::Vector2f pos)
{
	force = sf::Vector2f();
	position = pos;
	velocity = sf::Vector2f();
	
	density = 0;
	mass = Constants::P_MASS;
	pressure = 0;

	color = 0;
	normal = sf::Vector2f();
}

// Get particle velocity length
float Particle::PgetPVL() const
{
	return velocity.x * velocity.x + velocity.y * velocity.y;
}
