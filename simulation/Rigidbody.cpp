#include <iostream>
#include <cmath>

#include "Fluid.h"
#include "Rigidbody.h"
#include "Constants.h"

/*
*	Was unable to implement rigidbodies within the simulation due to time constraints and conflict with Constant variables.
*	Will attempt to implement at a later date.
*/

// Create particles
Rigidbody::Rigidbody()
{
	Rigidbody(Vector2f());

	rbAmount = 2;
	rigidbodies = std::vector<Rigidbody>();

	float width = Constants::WIDTH / 4.2f;
	float height = 3.0f * Constants::HEIGHT / 4.0f;

	sf::FloatRect rigidbodyRect = sf::FloatRect((Constants::WIDTH - width) / 2, Constants::HEIGHT - height, width, height);

}

// Define force values
Rigidbody::Rigidbody(sf::Vector2f pos)
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
float Rigidbody::RBgetPVL() const
{
	return velocity.x * velocity.x + velocity.y * velocity.y;
}

// FIX THIS
// Calculate rigidbody forces
void Rigidbody::calcuForceDensityRB()
{
	for (int i = 0; i < rbAmount; i++)
	{
		sf::Vector2f rbPressure = sf::Vector2f(0.0f, 0.0f);
		sf::Vector2f rbGravity = sf::Vector2f(0.0f, 0.0f);

		// Calculate forces
		rbGravity =  rigidbodies[i].density * sf::Vector2f(0, Constants::GRAVITY);
		rbPressure *= -1.0f;

		rigidbodies[i].force += rbPressure + rbGravity;
	}
}

// Integrate forces and rigidbody positions
void Rigidbody::integrationStepRB(float dt)
{
	for (int i = 0; i < rbAmount; i++)
	{
		rigidbodies[i].velocity += dt * rigidbodies[i].force / rigidbodies[i].density;
		rigidbodies[i].position += dt * rigidbodies[i].velocity;
	}
}

// Rigidbody collision with sides of application
void Rigidbody::collisionHandlingRB()
{
	for (int i = 0; i < rbAmount; i++)
	{
		if (rigidbodies[i].position.x < 0.0f)
		{
			rigidbodies[i].position.x = 0.0f;
			rigidbodies[i].velocity.x = -0.5f * rigidbodies[i].velocity.x;
		}
		else if (rigidbodies[i].position.x > Constants::WIDTH)
		{
			rigidbodies[i].position.x = Constants::WIDTH;
			rigidbodies[i].velocity.x = -0.5f * rigidbodies[i].velocity.x;
		}
		if (rigidbodies[i].position.y < 0.0f)
		{
			rigidbodies[i].position.y = 0.0f;
			rigidbodies[i].velocity.y = -0.5f * rigidbodies[i].velocity.y;
		}
		else if (rigidbodies[i].position.y > Constants::HEIGHT)
		{
			rigidbodies[i].position.y = Constants::HEIGHT;
			rigidbodies[i].velocity.y = -0.5f * rigidbodies[i].velocity.y;
		}
	}
}