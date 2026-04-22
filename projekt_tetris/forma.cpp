#include "forma.h"

Forma::Forma(const std::array<Punkt, 4>& b, sf::Color c)
    : bloki(b), kolor(c)
{
    syncVector();
}

void Forma::syncVector()
{
    vector_blokow.clear();
    for (auto& p : bloki)
    {
        vector_blokow.push_back(p);
    }
}

void Forma::przestun(int dx, int dy)
{
    for (auto& p : bloki)
    {
        p.x += dx;
        p.y += dy;
    }
    syncVector();
}

void Forma::obrot()
{

    Punkt mid = bloki[1];
    for (auto& p : bloki)
    {
        int x = p.x - mid.x;
        int y = p.y - mid.y;
        p.x = mid.x - y;
        p.y = mid.y + x;
    }
    syncVector();
}

const std::vector<Punkt>& Forma::getBlok() const
{
    return vector_blokow;
}

sf::Color Forma::getKolor() const
{
    return kolor;
}