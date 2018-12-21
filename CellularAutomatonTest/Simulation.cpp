#include "Simulation.h"
#include "Application.h"
#include <iostream>
#include <random>
#include <chrono>
#include <fstream>


static std::mt19937 rng((unsigned)std::time(nullptr));
static std::mt19937 gen(std::time(NULL));

static std::ofstream populationC("C:/Users/gs1n15/Documents/populations/populationC.txt");
static std::ofstream populationR("C:/Users/gs1n15/Documents/populations/populationR.txt");
static std::ofstream populationS("C:/Users/gs1n15/Documents/populations/populationS.txt");

static unsigned long x = 123456789, y = 362436069, z = 521288629;

unsigned long xorshf96(void) {          //period 2^96-1
	unsigned long t;
	x ^= x << 16;
	x ^= x >> 5;
	x ^= x << 1;

	t = x;
	x = y;
	y = z;
	z = t ^ x ^ y;

	return z;
}

Simulation::Simulation(const Config & config, const Application& app)
	: CellularAutomaton(config, app)
{
	tau = 0.75;
	delta_c = 0.3333;
	delta_r = 0.3125;
	delta_s0 = 0.25;
	sf::Color colours[4]
	{
		sf::Color::Red,
		sf::Color::Green,
		sf::Color::Blue,
		sf::Color::Black
	};

	std::mt19937 rng((unsigned)std::time(nullptr));
	cellForEach([&](unsigned x, unsigned y)
	{
		std::uniform_int_distribution<int> dist(0, 3);

		int& cell = n_cells[y][x];
		cell = (int)dist(rng);

		CellularAutomaton::setCellColour(x, y, colours[cell]);

	});

	populationC.open("C:/Users/gs1n15/Documents/populations/populationC.txt");
	populationR.open("C:/Users/gs1n15/Documents/populations/populationR.txt");
	populationS.open("C:/Users/gs1n15/Documents/populations/populationS.txt");
}

static std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();

void benchmark_start()
{
	t0 = std::chrono::high_resolution_clock::now();
}

void benchmark_end()
{
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration<double, std::nano>(t1 - t0).count() << "\n";
}

static unsigned countC, countR, countS, countE;

void Simulation::openFiles()
{
	populationC.open("C:/Users/gs1n15/Documents/populations/populationC.txt");
	populationR.open("C:/Users/gs1n15/Documents/populations/populationR.txt");
	populationS.open("C:/Users/gs1n15/Documents/populations/populationS.txt");
}

void Simulation::closeFiles()
{
	populationC.close();
	populationR.close();
	populationS.close();
}

void Simulation::countPopulations()
{
	unsigned population_c = 0;
	unsigned population_r = 0;
	unsigned population_s = 0;
	cellForEach([&](unsigned x, unsigned y)
	{
		sf::Vector2i loc(x, y);
		int cell = n_cells[y][x];
		switch (cell)
		{
		case 0:
			population_c++;
			break;
		case 1:
			population_r++;
			break;
		case 2:
			population_s++;
			break;
		}
	});

	/*populationC.open("C:/Users/gs1n15/Documents/populations/populationC.txt", std::ios_base::app);
	populationR.open("C:/Users/gs1n15/Documents/populations/populationR.txt", std::ios_base::app);
	populationS.open("C:/Users/gs1n15/Documents/populations/populationS.txt", std::ios_base::app);

	populationC << population_c << "\n";
	populationR << population_r << "\n";
	populationS << population_s << "\n";

	populationC.close();
	populationR.close();
	populationS.close();*/
}

int neighbours[4];

void Simulation::count_neighbours(unsigned x, unsigned y)
{
	//benchmark_start();
	for (int i = 0; i < 4; i++)
		neighbours[i] = 0;

	for (int nX = -1; nX <= 1; nX++)    //check neighbours
	{
		for (int nY = -1; nY <= 1; nY++)
		{
			if (!(nX == 0 && nY == 0)) //Cell itself
			{
				int newX = (nX + x) % (int)m_pConfig->simSize.x;
				int newY = (nY + y) % (int)m_pConfig->simSize.y;

				int cell = n_cells[newY][newX];
				neighbours[cell]++;
			}
		}
	}
	//benchmark_end();
}

void Simulation::update()
{
	for (int i = 0; i < 62500; i++)
	{

		unsigned x = xorshf96() % 250;
		unsigned y = xorshf96() % 250;

		sf::Vector2i loc(x, y);

		int cell = n_cells[y][x];
		if (cell == 3)
		{
			count_neighbours(x, y);

			double fractions[] =
			{ (double)neighbours[0] / 8.0,
				(double)neighbours[1] / 8.0,
				(double)neighbours[2] / 8.0,
				(double)neighbours[3] / 8.0 };

			double random = xorshf96() % 100 / 100.0;
			unsigned ind;
			if (random < fractions[0])
				ind = 0;
			else if (random >= fractions[0] && random < fractions[0] + fractions[1])
				ind = 1;
			else if (random >= fractions[0] + fractions[1] && random < fractions[0] + fractions[1] + fractions[2])
				ind = 2;
			else
				ind = 3;
			n_cells[y][x] = ind;
		}
		else if (cell == 0)
		{
			if ((double)(xorshf96() % 100) / 100.0 <= delta_c)
				n_cells[y][x] = 3;
		}
		else if (cell == 1)
		{
			if ((double)(xorshf96() % 100) / 100.0 <= delta_r)
				n_cells[y][x] = 3;
		}
		else if (cell = 2)
		{
			count_neighbours(x, y);
			double prob = delta_s0 + tau * ((double)neighbours[0] / 8.0);
			if ((double)(xorshf96() % 100) / 100.0 <= prob)
				n_cells[y][x] = 3;
		}
		//benchmark_end();
	}
	cellForEach([&](unsigned x, unsigned y)
	{
		unsigned cell = n_cells[y][x];
		
		switch (cell)
		{
		case 0:
			CellularAutomaton::setCellColour(x, y, sf::Color::Red);
			break;
		case 1:
			CellularAutomaton::setCellColour(x, y, sf::Color::Green);
			break;
		case 2:
			CellularAutomaton::setCellColour(x, y, sf::Color::Blue);
			break;
		case 3:
			CellularAutomaton::setCellColour(x, y, sf::Color::Black);
			break;
		}
	});

	//countPopulations();
	//benchmark_end();
}