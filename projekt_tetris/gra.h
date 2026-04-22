#pragma once
#ifndef GRA_H
#define GRA_H

#include <SFML/Graphics.hpp>
#include "tabela.h"
#include "figury.h"
#include <thread>
#include <atomic>
#include <chrono>

enum class State { Start, Playing, GameOver };

class Gra
{
public:
	Gra();
	~Gra();
	void run();

private:
	void loadScores();
	void saveScores();
	void startGame();
	void sterowanie();
	void update();
	void rysuj();
	void timerThreadFunc();

	sf::RenderWindow window;
	Obszar area;
	TworzenieF factory;
	std::unique_ptr<Figura> obecny;
	State state;
	sf::Clock dropClock;
	std::thread timerThread;
	std::atomic<bool> timerRunning;
	std::atomic<int> elapsedSeconds;
	sf::Font czcionka;
	int score;
	int lastScore;
	int bestScore;
	bool widmo;

};


#endif 
