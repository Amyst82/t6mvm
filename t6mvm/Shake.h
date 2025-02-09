#pragma once
class Shake
{
private:
	float Fract(float x);
	float Rand(float x);
	float RandStep(float x);
	float Smoothstep(float x);
	float Smoothsaw(float x);
	float GetRandomFloat(int x, int y);
	int randomFactor;
public:
	float GetShakeOutput(float x);

	Shake(float _randomFactor) { randomFactor = _randomFactor; };
};