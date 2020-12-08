#include <iostream>
#include <cmath>

#include "Constants.h"
#include "Grid.h"

// Create position grid
Grid::Grid()
{
	xCellsAmount = Constants::WIDTH / Constants::SIMULATION_KERNEL + 1;
	yCellsAmount = Constants::HEIGHT / Constants::SIMULATION_KERNEL + 1;

	std::cout << "   ---------------------------------------------\n Particle position grid created with " << xCellsAmount << " x " << yCellsAmount << " cells."<< std::endl;
}

// Get adjacing cells
// - Current usage of float variables allow for potential overflow, requires improvement in future
std::vector<Cell> Grid::getAdjacingCells(sf::Vector2f position)
{
	std::vector<Cell> finalCells = std::vector<Cell>();

	int xCell = position.x / Constants::SIMULATION_KERNEL;
	int yCell = position.y / Constants::SIMULATION_KERNEL;

	finalCells.push_back(cells[xCell][yCell]);

	if (xCell > 0)
	{
		finalCells.push_back(cells[xCell - 1][yCell]);
	}
	if (xCell < xCellsAmount - 1)
	{
		finalCells.push_back(cells[xCell + 1][yCell]);
	}

	if (yCell > 0)
	{
		finalCells.push_back(cells[xCell][yCell - 1]);
	}
	if (yCell < yCellsAmount - 1)
	{
		finalCells.push_back(cells[xCell][yCell + 1]);
	}

	if (xCell > 0 && yCell > 0)
	{
		finalCells.push_back(cells[xCell - 1][yCell - 1]);
	}
	if (xCell > 0 && yCell < yCellsAmount - 1)
	{
		finalCells.push_back(cells[xCell - 1][yCell + 1]);
	}
	if (xCell < xCellsAmount - 1 && yCell > 0)
	{
		finalCells.push_back(cells[xCell + 1][yCell - 1]);
	}

	if (xCell < xCellsAmount - 1 && yCell < yCellsAmount - 1)
	{
		finalCells.push_back(cells[xCell + 1][yCell + 1]);
	}

	return finalCells;
}

// Update grid structure with new cell positions
void Grid::updateStruct(std::vector<Particle> &particles)
{
	cells = std::vector<std::vector<Cell>>(xCellsAmount, std::vector<Cell>(yCellsAmount, Cell()));

	for (int i = 0; i < particles.size(); i++)
	{
		int xCell = particles[i].position.x / Constants::SIMULATION_KERNEL;
		int yCell = particles[i].position.y / Constants::SIMULATION_KERNEL;

		cells[xCell][yCell].push_back(i);
	}
}