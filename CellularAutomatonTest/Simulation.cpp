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
	, m_cells(config.simSize.x * config.simSize.y)
{
	tau = 0.75;
	delta_c = 0.3333;
	delta_r = 0.3125;
	delta_s0 = 0.25;

	std::mt19937 rng((unsigned)std::time(nullptr));
	cellForEach([&](unsigned x, unsigned y)
	{
		unsigned index = getCellIndex(x, y);
		std::uniform_int_distribution<int> dist(0, 3);

		auto& cell = m_cells[index];
		cell = (Cell)dist(rng);

		switch (cell)
		{
		case Cell::C:
			CellularAutomaton::setCellColour(x, y, sf::Color::Red);
			break;
		case Cell::R:
			CellularAutomaton::setCellColour(x, y, sf::Color::Green);
			break;
		case Cell::S:
			CellularAutomaton::setCellColour(x, y, sf::Color::Blue);
			break;
		case Cell::E:
			CellularAutomaton::setCellColour(x, y, sf::Color::Black);
			break;
		}
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
	std::cout << "files opened";
	populationC.open("C:/Users/gs1n15/Documents/populations/populationC.txt");
	populationR.open("C:/Users/gs1n15/Documents/populations/populationR.txt");
	populationS.open("C:/Users/gs1n15/Documents/populations/populationS.txt");
}

void Simulation::closeFiles()
{
	populationC.close();
	populationR.close();
	populationS.close();
	std::cout << "files closed";
}

void Simulation::countPopulations()
{
	unsigned population_c = 0;
	unsigned population_r = 0;
	unsigned population_s = 0;
	cellForEach([&](unsigned x, unsigned y)
	{
		sf::Vector2i loc(x, y);
		auto cell = m_cells[getCellIndex(x, y)];
		switch (cell)
		{
		case Cell::C:
			population_c++;
			break;
		case Cell::R:
			population_r++;
			break;
		case Cell::S:
			population_s++;
			break;
		}
	});

	populationC.open("C:/Users/gs1n15/Documents/populations/populationC.txt", std::ios_base::app);
	populationR.open("C:/Users/gs1n15/Documents/populations/populationR.txt", std::ios_base::app);
	populationS.open("C:/Users/gs1n15/Documents/populations/populationS.txt", std::ios_base::app);

	populationC << population_c << "\n";
	populationR << population_r << "\n";
	populationS << population_s << "\n";

	populationC.close();
	populationR.close();
	populationS.close();
}

void Simulation::count_neighbours(unsigned x, unsigned y)
{
	//benchmark_start();
	countC = 0;
	countR = 0;
	countS = 0;
	countE = 0;

	for (int nX = -60; nX <= 60; nX++)    //check neighbours
	{
		for (int nY = -60; nY <= 60; nY++)
		{
			if (!(nX == 0 && nY == 0)) //Cell itself
			{
				int newX = (nX + x) % (int)m_pConfig->simSize.x;
				int newY = (nY + y) % (int)m_pConfig->simSize.y;

				auto cell = m_cells[getCellIndex(newX, newY)];

				switch (cell)
				{
				case Cell::C:
					countC++;
					break;
				case Cell::R:
					countR++;
					break;
				case Cell::S:
					countS++;
					break;
				case Cell::E:
					countE++;
					break;
				}
			}
		}
	}
	//benchmark_end();
}

void Simulation::update()
{
	for (int i = 0; i < 14400; i++)
	{
		//std::uniform_int_distribution<int> dist(0, 119);

		unsigned x = xorshf96() % 120;
		unsigned y = xorshf96() % 120;

		sf::Vector2i loc(x, y);

		int index = getCellIndex(x, y);
		auto cell = m_cells[index];

		if (cell == Cell::E)
		{
			count_neighbours(x, y);

			double fractions[] =
			{ (double)countC / 14399.0,
				(double)countR / 14399.0,
				(double)countS / 14399.0,
				(double)countE / 14399.0 };

			Cell cells[] =
			{ Cell::C,
				Cell::R,
				Cell::S,
				Cell::E };

			double random = xorshf96() % 100 / 100.0;
			//std::discrete_distribution<> d(std::begin(fractions), std::end(fractions));
			unsigned ind;
			if (random < fractions[0])
				ind = 0;
			else if (random >= fractions[0] && random < fractions[0] + fractions[1])
				ind = 1;
			else if (random >= fractions[0] + fractions[1] && random < fractions[0] + fractions[1] + fractions[2])
				ind = 2;
			else //if (random >= fractions[0] + fractions[1] + fractions[2] && random < fractions[0] + fractions[1] + fractions[2] + fractions[3])
				ind = 3;
			m_cells[index] = cells[ind];
			//std::cout << ind << "\n";
		}
		else if (cell == Cell::C)
		{
			if ((double)(xorshf96() % 100) / 100.0 <= delta_c)
				m_cells[index] = Cell::E;
		}
		else if (cell == Cell::R)
		{
			if ((double)(xorshf96() % 100) / 100.0 <= delta_r)
				m_cells[index] = Cell::E;
		}
		else if (cell = Cell::S)
		{
			count_neighbours(x, y);
			double prob = delta_s0 + tau * ((double)countC / 14399.0);
			if ((double)(xorshf96() % 100) / 100.0 <= prob)
				m_cells[index] = Cell::E;
		}
		//benchmark_end();
	}
	/*for (auto& update : updates) {
		m_cells[getCellIndex(update.first.x, update.first.y)] = update.second;

		switch (update.second)
		{
		case Cell::C:
			CellularAutomaton::setCellColour(update.first.x, update.first.y, sf::Color::Red);
			break;
		case Cell::R:
			CellularAutomaton::setCellColour(update.first.x, update.first.y, sf::Color::Green);
			break;
		case Cell::S:
			CellularAutomaton::setCellColour(update.first.x, update.first.y, sf::Color::Blue);
			break;
		case Cell::E:
			CellularAutomaton::setCellColour(update.first.x, update.first.y, sf::Color::Black);
			break;
		}
	}*/

	cellForEach([&](unsigned x, unsigned y)
	{
		int index = getCellIndex(x, y);
		auto cell = m_cells[index];
		
		switch (cell)
		{
		case Cell::C:
			CellularAutomaton::setCellColour(x, y, sf::Color::Red);
			break;
		case Cell::R:
			CellularAutomaton::setCellColour(x, y, sf::Color::Green);
			break;
		case Cell::S:
			CellularAutomaton::setCellColour(x, y, sf::Color::Blue);
			break;
		case Cell::E:
			CellularAutomaton::setCellColour(x, y, sf::Color::Black);
			break;
		}
	});

	countPopulations();
	//benchmark_end();
}

//std::vector<sf::Vertex> Simulation::getNeighbours(unsigned x, unsigned y)
//{
//	
//}