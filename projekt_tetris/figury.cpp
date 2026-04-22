#include "figury.h"
#include "forma.h"
#include <cstdlib>
#include <ctime>
#include <array>

TworzenieF::TworzenieF()
	: seeded(false) {}

std::unique_ptr<Figura> TworzenieF::losowa()
{
	if (!seeded)
	{
		std::srand(static_cast<unsigned>(std::time(nullptr)));
		seeded = true;
	}

	int k = std::rand() % 7;
	sf::Color kolor;
	std::array<Punkt, 4> bloki;

	switch (k) {
	case 0: // I
		bloki = { { {0,1}, {1,1}, {2,1}, {3,1} } };
		kolor = sf::Color::Cyan;
		break;

	case 1: // J
		bloki = { { {0,0}, {0,1}, {1,1}, {2,1} } };
		kolor = sf::Color::Blue;
		break;

	case 2: // L
		bloki = { { {2,0}, {0,1}, {1,1}, {2,1} } };
		kolor = sf::Color(255, 165, 0);
		break;

	case 3: // O
		bloki = { { {1,0}, {2,0}, {1,1}, {2,1} } };
		kolor = sf::Color::Yellow;
		break;

	case 4: // S
		bloki = { { {1,0}, {2,0}, {0,1}, {1,1} } };
		kolor = sf::Color::Green;
		break;

	case 5: // T
		bloki = { { {0,1}, {1,0}, {1,1}, {2,1} } };
		kolor = sf::Color::Magenta;
		break;

	case 6: // Z
		bloki = { { {0,1}, {1,1}, {1,0}, {2,0} } };
		kolor = sf::Color::Red;
		break;

	}

	return std::make_unique<Forma>(bloki, kolor);

}