#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

// Rigidbody class
class Rigidbody
{
	public:
		Rigidbody();

		void update(float dt);
		void render(sf::RenderTarget& rt);

		sf::Vector2f force;
		sf::Vector2f position;
		sf::Vector2f velocity;

		float density;
		float mass;
		float pressure;

		float color;
		sf::Vector2f normal;

		Rigidbody(sf::Vector2f position);

		sf::Color renderColour;

		// Get rigidbody velocity length
		float RBgetPVL() const; 

	private:
		int rbAmount;
		std::vector<Rigidbody> rigidbodies;

		void calcuForceDensityRB();
		void integrationStepRB(float dt);
		void collisionHandlingRB();
};