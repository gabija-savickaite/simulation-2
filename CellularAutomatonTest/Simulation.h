#pragma once

#include <vector>
#include <random>
#include "CellularAutomaton.h"


class Simulation : public CellularAutomaton
{
public:
	Simulation(const Config& config, const Application& app);

	void update() override;

	void Simulation::countPopulations();

	static void Simulation::openFiles();
	static void Simulation::closeFiles();

private:
	int n_cells[250][250];

	float tau;
	float delta_c;
	float delta_r;
	float delta_s0;

	void Simulation::count_neighbours(unsigned x, unsigned y);
};