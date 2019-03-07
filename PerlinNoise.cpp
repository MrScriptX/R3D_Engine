#include "PerlinNoise.h"


PerlinNoise::PerlinNoise(unsigned int seed)
{
	p.resize(256);

	std::iota(p.begin(), p.end(), 0);

	std::default_random_engine engine(seed);

	std::shuffle(p.begin(), p.end(), engine);

	p.insert(p.end(), p.begin(), p.end());
}

PerlinNoise::~PerlinNoise()
{
}

double PerlinNoise::fade(double t)
{
	return t*t*t*(t*(t*6.0 -15.0) + 10.0);
}
