#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>

#include "Constants.h"
#include "Fluid.h"

using namespace Constants;

// Set application name
const std::string APPLICATION_TITLE = "2D Fluid Simulation";

// Set display to show normal particles
Display display = Display::Particles;

// Create shader for metaballs
sf::Shader shader;
sf::RenderTexture renderTexture;

// Text variables
sf::Font font;
sf::Text fpsText;
sf::String fpsString = "FPS: ";
sf::String currentFPS = "";

sf::Text instructionText;

std::vector<sf::RectangleShape> rects;

constexpr float initialAnimationTimer = 1.0f;
float animationTimer = initialAnimationTimer;

// Allows for animation to be paused
bool pauseSimulation = false;

// Declare functions
void update(float, Fluid&);
void render(sf::RenderWindow&, sf::RenderTexture&, Fluid&);
void toggleVisualisation();

// Clock for frames
sf::Clock frameClock;

// Load function
void Load(sf::RenderWindow& window)
{
	// Create render texture 
	renderTexture.create(RENDER_WIDTH, RENDER_HEIGHT);
	
	// Pre-load shaders for first time
	shader.loadFromFile("res/particle_shader.vert", "res/particle_shader.frag");
	
	// Load text font
	if (!font.loadFromFile("res/VCR_OSD_MONO.ttf"))
	{
		std::cout << "ERROR: Unable to load font" << std::endl;
	}

	// Set fps string contents
	fpsText.setFont(font);
	fpsText.setCharacterSize(16.0f);
	fpsText.setPosition(RENDER_WIDTH - 80, 10);
	fpsText.setString(fpsString + "0");

	// Set instruction text contents
	instructionText.setFont(font);
	instructionText.setCharacterSize(16.0f);
	instructionText.setPosition(10, 10);
	instructionText.setString("TAB   - Switch display\nNum1  - Particles\nNum2  - Water\nNum3  - Velocity View\nNum4  - Density View\nNum5  - Pressure View\nSpace - Pause simulation\n\nLMB   - Repulse\nRMB   - Attract");
}

// Update function
void Update(sf::RenderWindow& window, Fluid& fluid)
{
	// Update fps counter
	currentFPS = std::to_string((int)(1.0 / frameClock.restart().asSeconds()));
	fpsText.setString(fpsString + currentFPS);

	sf::Event event;
	while (window.pollEvent(event)) 
	{
		if (event.type == sf::Event::Closed) 
		{
			window.close();
			return;
		}

		// Keyboard actions
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				window.close();
			}
			if (event.key.code == sf::Keyboard::Tab)
			{
				toggleVisualisation();
			}
			if (event.key.code == sf::Keyboard::Space)
			{
				pauseSimulation = !pauseSimulation;
			}
			if (event.key.code == sf::Keyboard::Num1)
			{
				fluid = Fluid();
				display = Display::Particles;
			}
			if (event.key.code == sf::Keyboard::Num2)
			{
				fluid = Fluid();
				display = Display::Water;
			}
			if (event.key.code == sf::Keyboard::Num3)
			{
				fluid = Fluid();
				display = Display::Velocity;
			}
			if (event.key.code == sf::Keyboard::Num4)
			{
				fluid = Fluid();
				display = Display::Density;
			} 
			if (event.key.code == sf::Keyboard::Num5)
			{
				fluid = Fluid();
				display = Display::Pressure;
			}
		}

		// Repulsion force on mouse click
		if (event.type == sf::Event::MouseButtonPressed)
		{
			float x = event.mouseButton.x / WINDOW_SCALE / SCALE;
			float y = event.mouseButton.y / WINDOW_SCALE / SCALE;
			sf::Vector2f position = sf::Vector2f(x, y);

			// Repulse particles on left mouse click
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				fluid.repulsionForce(position);
			}

			// Attract particles on left mouse click
			if (event.mouseButton.button == sf::Mouse::Right)
			{
				fluid.attractionForce(position);
			}
		}

		// Change display
		switch (display)
		{
			case Display::Particles:
				window.setTitle(APPLICATION_TITLE + " - Particle");
				break;
			case Display::Water:
				window.setTitle(APPLICATION_TITLE + " - Water");
				break;
			case Display::Velocity:
				window.setTitle(APPLICATION_TITLE + " - Velocity View");
				break;
			case Display::Density:
				window.setTitle(APPLICATION_TITLE + " - Density View");
				break;
			case Display::Pressure:
				window.setTitle(APPLICATION_TITLE + " - Pressure View");
				break;
			default:
				break;
		}
	}
}

// Main function
int main(int, char const**)
{
	std::cout << " 2D Fluid Simulation" << std::endl;
	std::cout << " *******************" << std::endl;

	// Initialise fluid
	Fluid fluid = Fluid();

	// Create window set properties
	sf::RenderWindow window(sf::VideoMode(RENDER_WIDTH * WINDOW_SCALE, RENDER_HEIGHT * WINDOW_SCALE), APPLICATION_TITLE, sf::Style::Titlebar | sf::Style::Close);
	window.setKeyRepeatEnabled(false);

	// Call load function
	Load(window);

	while (window.isOpen())
	{
		window.clear();

		// Update function
		Update(window, fluid);

		// Pause simulation function
		float time = 0.0f;
		if (!pauseSimulation)
		{
			update(DELTATIME, fluid);
			animationTimer += time;
		}

		// Render scene
		render(window, renderTexture, fluid);

		// Load background colour - black makes particles stand out
		renderTexture.clear(sf::Color::Black);

		// Display text elements
		window.draw(fpsText);
		window.draw(instructionText);

		// Load cubes on spawn
		for (const auto& r : rects)
		{
			window.draw(r);
		}

		window.display();
	}
	return 0;
}

// Update simulation
void update(float dt, Fluid& fluid)
{
	fluid.update(dt, display);
}

// Render function
void render(sf::RenderWindow& window, sf::RenderTexture& renderTexture, Fluid& fluid)
{
	fluid.render(renderTexture, display);
	renderTexture.display();
	renderTexture.setSmooth(true);

	// Set water texture to be same as window size
	sf::Sprite sprite = sf::Sprite(renderTexture.getTexture());
	sprite.setScale(WINDOW_SCALE, WINDOW_SCALE);

	if (display != Display::Water)
	{
		window.draw(sprite);
	}
	else
	{
		// Set shader to replace particle shape and appearance
		shader.setParameter("texture", sf::Shader::CurrentTexture);
		window.draw(sprite, &shader);
	}
}

// Switch visualisation 
void toggleVisualisation()
{
	switch (display)
	{
		case Display::Particles:
			display = Display::Water;
			break;
		case Display::Water:
			display = Display::Velocity;
			break;
		case Display::Velocity:
			display = Display::Density;
			break;
		case Display::Density:
			display = Display::Pressure;
			break;
		case Display::Pressure:
			display = Display::Particles;
			break;
		default:
			break;
	}
}