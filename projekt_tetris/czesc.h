#pragma once
#ifndef CZESC_H
#define CZESC_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>

struct Punkt
{
	int x;
	int y;
};

class Figura
{
public:

	virtual ~Figura() = default;
	virtual void obrot() = 0;
	virtual void przestun(int dx, int dy) = 0;
	virtual const std::vector<Punkt>& getBlok() const = 0;
	virtual sf::Color getKolor() const = 0;

};


#endif 