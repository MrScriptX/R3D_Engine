#ifndef _PERLIN_NOISE_H
#define _PERLIN_NOISE_H

#include <vector>
#include <numeric>
#include <random>

class PerlinNoise
{
public:
	PerlinNoise(unsigned int seed);
	~PerlinNoise();

	double noise(double x, double y, double z);

private:
	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);

	std::vector<int> p;
};

#endif