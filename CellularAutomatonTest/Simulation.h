#pragma once

#include <vector>
#include <random>
#include "CellularAutomaton.h"


class Simulation : public CellularAutomaton
{
	enum Cell
	{
		C,
		R,
		S,
		E
	};

public:
	Simulation(const Config& config, const Application& app);

	void update() override;

	void Simulation::countPopulations();

	static void Simulation::openFiles();
	static void Simulation::closeFiles();

	//std::vector<sf::Vertex> getNeighbours(unsigned x, unsigned y);

private:
	std::vector<Cell> m_cells;

	float tau;
	float delta_c;
	float delta_r;
	float delta_s0;

	/*std::ofstream* populationC;
	std::ofstream* populationR;
	std::ofstream* populationS;*/

	void Simulation::count_neighbours(unsigned x, unsigned y);
};