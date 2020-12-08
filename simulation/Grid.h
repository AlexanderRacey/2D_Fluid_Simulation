#pragma once
#include <vector>
#include "Particle.h"

// Vector for storing cells
typedef std::vector<int> Cell;

// Grid Class
class Grid
{
	public:
		Grid();

		void updateStruct(std::vector<Particle> &particles);

		std::vector<Cell> getAdjacingCells(sf::Vector2f position);
	
	private:
		int xCellsAmount;
		int yCellsAmount;

		std::vector<std::vector<Cell>> cells;
};