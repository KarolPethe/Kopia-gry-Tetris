#pragma once
#ifndef FIGURY_H
#define FIGURY_H

#include <memory>
#include "czesc.h"

class TworzenieF
{
public:

	TworzenieF();
	std::unique_ptr<Figura> losowa();

private:

	bool seeded;
};

#endif 
