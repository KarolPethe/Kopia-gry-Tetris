#pragma once
#ifndef TABELA_H
#define TABELA_H

#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include "czesc.h"

constexpr int COLS = 10;
constexpr int ROWS = 20;
constexpr int BLOCK = 32;

class Obszar
{
public:

	Obszar();

	bool poprawna(const std::vector<Punkt>& bl) const;

	void zablokuj(const Figura& f);

	int usun();

	void rysuj(sf::RenderWindow& win) const;

private:

	std::array<std::array<sf::Color, COLS>, ROWS> komorka;

};

#endif 
