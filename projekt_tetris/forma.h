#pragma once
#ifndef FORMA_H
#define FORMA_H

#include "czesc.h"
#include <array>
#include <vector>
#include <random>


class Forma : public Figura
{
public:
    Forma(const std::array<Punkt, 4>& b, sf::Color c);

    void obrot() override;

    void przestun(int dx, int dy) override;

    const std::vector<Punkt>& getBlok() const override;

    sf::Color getKolor() const override;

private:
    std::array<Punkt, 4> bloki;

    std::vector<Punkt> vector_blokow;

    sf::Color kolor;

    void syncVector();
};

#endif 