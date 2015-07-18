#include <iostream>
#include "math_utility.h"
#include "matrix.h"
#include "perlin.h"
#include "quaternion.h"

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

	Matrix2x2 m;
	m.rotation(0);

	cout<<"rotation_matrix:"<<endl;
	cout<<m<<endl;

	Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);
	Quaternion q2(2.0f, 3.0f, 4.0f, 5.0f);
	cout<<"quaternions: "<<endl;
	cout<<"Q1: "<<q1<<endl;
	cout<<"Q2: "<<q2<<endl;
	cout<<"Q1 * Q2: "<<q1*q2<<endl;
	cout<<"conjugate of Q1: "<<~q1<<endl;
	cout<<"norm of Q1: "<<sqrt(q1.norm_squared())<<endl;
	q2.normalize();
	cout<<"normalized Q2: "<<q2<<endl<<endl;

	cout<<"quaternion from axis / angle rotation..."<<endl;
	Quaternion rot_quat;
	Float3 rot_axis(1.0f, 0.0f, 0.0f);
	rot_quat.rotation_from_axis_angle(rot_axis, M_PI * 0.25f);
	rot_quat.normalize();
	cout<<"rot_quat:"<<endl<<rot_quat<<endl;

	Matrix3x3 m3;
	cout<<"rotation from quaternion rot_quat: "<<endl;
	m3.rotation_from_quaternion(rot_quat);
	cout<<m3<<endl;

	return 0;
}
