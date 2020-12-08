#include <iostream>
#include <cmath>

#include "Constants.h"
#include "Fluid.h"

// Define value for pi
#ifndef M_PI 
#define M_PI    3.14159265358979323846f 
#endif

// Initialise fluid
Fluid::Fluid()
{
	int pX = Constants::P_ROW_NUMBER / 2.0f;
	int pY = Constants::P_ROW_NUMBER;

	// Calculate particle amount
	particleAmount = pX * pY;
	particles = std::vector<Particle>();

	float fluidWidth = Constants::WIDTH / 4.2f;
	float fluidHeight = 3.0f * Constants::HEIGHT / 4.0f;

	sf::FloatRect pRect = sf::FloatRect((Constants::WIDTH - fluidWidth) / 2, Constants::HEIGHT - fluidHeight, fluidWidth, fluidHeight);

	float dx = pRect.width / pX;
	float dy = pRect.height / pY;

	// Create particle starting positions
	for (int i = 0; i < Constants::P_ROW_NUMBER / 2.0f; i++)
	{
		for (int j = 0; j < Constants::P_ROW_NUMBER; j++)
		{
			sf::Vector2f position = sf::Vector2f(pRect.left, pRect.top) + sf::Vector2f(i * dx, j * dy);
			Particle p = Particle(position);
			particles.push_back(p);
		}
	}

	// Update grid with particle positions
	particleGrid.updateStruct(particles);

	// Load metaball texture 
	metaballTexture.loadFromFile("res/metaball.png");

	// Print particle number to console
	std::cout << " New fluid simulation constructed using " << particleAmount << " particles.\n" << std::endl;
}

// Calculate particle colours
sf::Color getColor(float value, float min, float max)
{
	float lowestColour = 0.0f;
	float highestColour = 255.0f;
	float finalColour = lowestColour + (highestColour - lowestColour) * (value - min) / (max - min);

	return sf::Color(50, 0, finalColour, 255);
}

// Set colours for particles
void Fluid::setParticleColours(Display display)
{
	float minValue;
	float maxValue;

	// Change particle colours based on display effects
	switch (display)
	{
		case Display::Particles:
			for (int i = 0; i < particleAmount; i++)
			{
				sf::Color pBlue = sf::Color(0, 163, 255);
				particles[i].renderColour = pBlue;
			}
			break;
		case Display::Velocity:
			minValue = 0.0f;
			maxValue = 20000.0f;
			for (int i = 0; i < particleAmount; i++)
			{
				sf::Color vColour = getColor(particles[i].PgetPVL(), minValue, maxValue);
				particles[i].renderColour = vColour;
			}
			break;
		case Display::Density:
			minValue = 2000.0f;
			maxValue = 12000.0f;
			for (int i = 0; i < particleAmount; i++)
			{
				sf::Color dColour = getColor(particles[i].density, minValue, maxValue);
				particles[i].renderColour = dColour;
			}
			break;
		case Display::Pressure:
			minValue = 500.0f;
			maxValue = 30000.0f;
			for (int i = 0; i < particleAmount; i++)
			{
				sf::Color pColour = getColor(particles[i].pressure, minValue, maxValue);
				particles[i].renderColour = pColour;
			}
			break;
		default:
			break;
	}
}

// Render fluid function
void Fluid::render(sf::RenderTarget &rt, Display display)
{
	if (display != Display::Water)
	{
		setParticleColours(display);
	}

	// Assign forces to particles 
	for (int i = 0; i < particleAmount; i++)
	{
		particles[i].force = sf::Vector2f(0.0f, 0.0f);
	}

	if (display != Display::Water)
	{
		// Align particles to identical ratio as application window
		sf::CircleShape circle(0.5f * Constants::P_SPACING * Constants::SCALE);

		for (int i = 0; i < particleAmount; i++)
		{
			circle.setFillColor(particles[i].renderColour);

			circle.setPosition(particles[i].position * Constants::SCALE);
			rt.draw(circle);
		}
	}
	else
	{
		// Create sprite for metaball texture
		sf::Sprite sprite;
		sprite.setTexture(metaballTexture);
		sprite.setOrigin(metaballTexture.getSize().x / 2.0f, metaballTexture.getSize().y / 2.0f);
		float scale = 4.2f * Constants::P_SPACING * Constants::SCALE / 256.0f;
		sprite.setScale(scale, scale);

		for (int i = 0; i < particleAmount; i++)
		{
			sprite.setPosition(particles[i].position * Constants::SCALE);
			rt.draw(sprite, sf::BlendAdd);
		}
	}
}

// Calculate repulsion force 
void Fluid::repulsionForce(sf::Vector2f position)
{
	for (int i = 0; i < particleAmount; i++)
	{
		sf::Vector2f x = particles[i].position - position;

		float distance = x.x * x.x + x.y * x.y;

		if (distance < Constants::SIMULATION_KERNEL * 3)
		{
			particles[i].force += x * 800000.0f * particles[i].density;
		}
	}
}

// Calculate attraction force 
void Fluid::attractionForce(sf::Vector2f position)
{
	for (int i = 0; i < particleAmount; i++)
	{
		sf::Vector2f x = position - particles[i].position;

		float distance = x.x * x.x + x.y * x.y;

		if (distance < Constants::SIMULATION_KERNEL * 3)
		{
			particles[i].force += x * 800000.0f * particles[i].density;
		}
	}
}

// Update simulation, calls multiple functions
void Fluid::update(float dt, Display display)
{
	findAdjacents();
	pCalcDensity();
	pCalcPressure();
	pCalcForceDensity();
	pCalcIntegration(dt);
	pCalcCollision();
	particleGrid.updateStruct(particles);
}

// Initialise kernel
float Fluid::kernel(sf::Vector2f x, float h)
{
	float h2 = h * h;
	float r2 = x.x * x.x + x.y * x.y;

	if (r2 < 0 || r2 > h2)
	{
		return 0.0f;
	}

	return 315.0f / (64.0f * M_PI * pow(h, 9)) * pow(h2 - r2, 3);
}

// Calculate kernel gradient
sf::Vector2f Fluid::gradKernel(sf::Vector2f x, float h)
{
	float r = sqrt(x.x * x.x + x.y * x.y);

	if (r == 0.0f)
	{
		return sf::Vector2f(0.0f, 0.0f);
	}

	float t1 = -45.0f / (M_PI * pow(h, 6));
	sf::Vector2f t2 = x / r;
	float t3 = pow(h - r, 2);

	return  t1 * t2 * t3;
}

// Calculating kernel laplacian
float Fluid::lapKernel(sf::Vector2f x, float h)
{
	float r = sqrt(x.x * x.x + x.y * x.y);

	return 45.0f / (M_PI * pow(h, 6)) * (h - r);
}

// Calculate adjacing cells/particles
void Fluid::findAdjacents()
{
	particleAdjacents = std::vector<std::vector<int>>();
	float maxDistance = Constants::SIMULATION_KERNEL * Constants::SIMULATION_KERNEL;

	for each (const Particle &p in particles)
	{
		std::vector<int> adjacents = std::vector<int>();
		std::vector<Cell> adjacingCells = particleGrid.getAdjacingCells(p.position);

		for each (const Cell &cell in adjacingCells)
		{
			for each (int cellIndex in cell)
			{
				sf::Vector2f x = p.position - particles[cellIndex].position;
				float pDistance = x.x * x.x + x.y * x.y;
				if (pDistance <= maxDistance)
				{
					adjacents.push_back(cellIndex);
				}
			}
		}
		particleAdjacents.push_back(adjacents);
	}
}

// Calculate particle density
void Fluid::pCalcDensity()
{
	for (int i = 0; i < particleAmount; i++)
	{
		std::vector<int> adjacents = particleAdjacents[i];
		float pDensity = 0.0f;

		for (int n = 0; n < adjacents.size(); n++)
		{
			int j = adjacents[n];

			sf::Vector2f x = particles[i].position - particles[j].position;
			pDensity += particles[j].mass * kernel(x, Constants::SIMULATION_KERNEL);
		}
		particles[i].density = pDensity;
	}
}

// Calculate particle pressure
void Fluid::pCalcPressure()
{
	for (int i = 0; i < particleAmount; i++)
	{
		particles[i].pressure = std::max(Constants::STIFFNESS * (particles[i].density - Constants::REST_DENSITY), 0.0f);
	}
}

// Calculate all particle forces
void Fluid::pCalcForceDensity()
{
	for (int i = 0; i < particleAmount; i++)
	{
		// Declare default force values
		sf::Vector2f gravity = sf::Vector2f(0.0f, 0.0f);
		sf::Vector2f pressure = sf::Vector2f(0.0f, 0.0f);
		sf::Vector2f viscosity = sf::Vector2f(0.0f, 0.0f);

		std::vector<int> adjacents = particleAdjacents[i];

		for (int n = 0; n < adjacents.size(); n++)
		{
			int j = adjacents[n];
			sf::Vector2f x = particles[i].position - particles[j].position;

			// Increase pressure and viscocity
			pressure += particles[j].mass * (particles[i].pressure + particles[j].pressure) / (2.0f * particles[j].density) * gradKernel(x, Constants::SIMULATION_KERNEL);
			viscosity += particles[j].mass * (particles[j].velocity - particles[i].velocity) / particles[j].density * lapKernel(x, Constants::SIMULATION_KERNEL);
		}

		// Gravitational force density
		gravity = particles[i].density * sf::Vector2f(0, Constants::GRAVITY);
		pressure *= -1.0f;
		viscosity *= Constants::VISCOSITY;

		particles[i].force += pressure + viscosity + gravity;
	}
}

// Integrate forces and particle positions
void Fluid::pCalcIntegration(float dt)
{
	for (int i = 0; i < particleAmount; i++)
	{
		particles[i].velocity += dt * particles[i].force / particles[i].density;
		particles[i].position += dt * particles[i].velocity;
	}
}

// Particle collision with sides of application
void Fluid::pCalcCollision()
{
	for (int i = 0; i < particleAmount; i++)
	{
		if (particles[i].position.x < 0.0f)
		{
			particles[i].position.x = 0.0f;
			particles[i].velocity.x = -0.5f * particles[i].velocity.x;
		}
		else if (particles[i].position.x > Constants::WIDTH)
		{
			particles[i].position.x = Constants::WIDTH;
			particles[i].velocity.x = -0.5f * particles[i].velocity.x;
		}
		if (particles[i].position.y < 0.0f)
		{
			particles[i].position.y = 0.0f;
			particles[i].velocity.y = -0.5f * particles[i].velocity.y;
		}
		else if (particles[i].position.y > Constants::HEIGHT)
		{
			particles[i].position.y = Constants::HEIGHT;
			particles[i].velocity.y = -0.5f * particles[i].velocity.y;
		}
	}
}