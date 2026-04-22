#include "tabela.h"

Obszar::Obszar()
{
    for (auto& row : komorka)
    {
        row.fill(sf::Color::Black);
    }
}

bool Obszar::poprawna(const std::vector<Punkt>& bl) const
{
    for (auto& p : bl)
    {
        if (p.x < 0 || p.x >= COLS || p.y < 0 || p.y >= ROWS)
        {
            return false;
        }
        if (komorka[p.y][p.x] != sf::Color::Black)
        {
            return false;
        }
    }
    return true;
}

void Obszar::zablokuj(const Figura& f)
{
    for (auto& p : f.getBlok())
    {
        komorka[p.y][p.x] = f.getKolor();
    }
}

int Obszar::usun()
{
    int count = 0;
    for (int y = ROWS - 1; y >= 0; y--)
    {
        bool full = true;
        for (int x = 0; x < COLS; x++)
        {
            if (komorka[y][x] == sf::Color::Black)
            {
                full = false;
                break;
            }
        }
        if (full)
        {
            count++;
            for (int y2 = y; y2 > 0; y2--)
            {
                for (int x = 0; x < COLS; ++x)
                {
                    komorka[y2][x] = komorka[y2 - 1][x];
                }
            }
            for (int x = 0; x < COLS; ++x)
            {
                komorka[0][x] = sf::Color::Black;
            }
            y++;
        }
    }
    return count;
}

void Obszar::rysuj(sf::RenderWindow& win) const
{
    sf::RectangleShape rect({ BLOCK - 1, BLOCK - 1 });

    for (int y = 0; y < ROWS; ++y)
    {
        for (int x = 0; x < COLS; ++x)
        {
            rect.setFillColor(komorka[y][x]);
            rect.setPosition(x * BLOCK, y * BLOCK);
            win.draw(rect);
        }
    }
}