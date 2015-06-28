#include <iostream>
#include "math_utility.h"
#include "perlin.h"

using namespace std;
using namespace Math;
using namespace PerlinNoise;

int main(int argc, char **argv)
{
	Float3 vec_test(1.5f, 0.4f, -21.4f);

	cout<<vec_test.magnitude()<<endl;

	cout<<raw_noise_2d(4.5f, 1.23f)<<endl;

	//test perlin
	float p = PerlinNoise::octave_noise_2d(1.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	cout<<p<<endl;

	return 0;
}
