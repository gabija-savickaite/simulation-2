#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

#include "CellularAutomaton.h"
#include "FPSCounter.h"
#include "Keyboard.h"

struct Config;

class Application
{
public:
	Application(const Config& config);

	template<typename T>
	void init()
	{
		m_automaton = std::make_unique<T>(*m_pConfig, *this);
	}

	void run();

	const sf::RenderWindow& getWindow() const;

private:
	void pollEvents();
	void step(float dt);
	void render();

	void resetView();

	std::unique_ptr<CellularAutomaton> m_automaton;

	sf::RenderWindow m_window;
	FPSCounter  m_fpsCounter;
	Keyboard    m_keyboard;
	sf::Text    m_guiText;
	sf::View    m_view;

	const Config* m_pConfig = nullptr;
	float m_zoom = 1;
};

