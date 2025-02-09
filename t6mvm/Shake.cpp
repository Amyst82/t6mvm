#include "pch.h"
#include "Shake.h"
#include <math.h>
#include <random>

float Shake::GetRandomFloat(int x, int y)
{
	return (float)(x + (rand() % (y - x + 1)));
}

float Shake::Fract(float x)
{
	return x - (float)floor(x);
}

float Shake::Rand(float x)
{
	return Fract(Shake::randomFactor * (float)sin(x));
}

float Shake::RandStep(float x)
{
	return Rand((float)floor(x));
}

float Shake::Smoothstep(float x)
{
	return (float)pow(x, 2) * (1 - 2 * (x - 1));
}

float Shake::Smoothsaw(float x)
{
	return Smoothstep(Fract(x));
}

float Shake::GetShakeOutput(float x)
{
	return RandStep(x) * Smoothsaw(x) + RandStep(x - 1) * (1 - Smoothsaw(x - 1));
}