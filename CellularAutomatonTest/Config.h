#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

const static int NUM_CELL_TYPES = 4;
const static double NUM_NEIGHBOURING_CELLS = 8;

struct Config
{
	Config() = default;
	Config(const sf::Vector2u& winSize, unsigned cellSize)
		: windowSize(winSize)
		, cellSize(cellSize)
	{
		init();
	}

	void init()
	{
		windowSize.x -= windowSize.x % cellSize;
		windowSize.y -= windowSize.y % cellSize;

		simSize.x = windowSize.x / cellSize;
		simSize.y = windowSize.y / cellSize;
	}

	sf::Vector2u windowSize = { 1280, 720 };
	sf::Vector2u simSize;
	unsigned int cellSize = 4;
	unsigned int fps = 30;

	sf::Color bgColour = { 150, 150, 150 };
	sf::Color fgColour = { 25, 25, 25 };
};
