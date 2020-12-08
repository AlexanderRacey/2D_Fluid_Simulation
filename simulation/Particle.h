#pragma once
#include <SFML/Graphics.hpp>

// Particle class
class Particle
{
	public:
		// Particle forces
		sf::Vector2f force;
		sf::Vector2f position;
		sf::Vector2f velocity;

		float density;
		float mass;
		float pressure;

		float color;
		sf::Vector2f normal;

		Particle();
		Particle(sf::Vector2f position);

		sf::Color renderColour;

		// Get particle velocity length
		float PgetPVL() const; 
};