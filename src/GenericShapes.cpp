#include "GenericShapes.h"

Sphere::Sphere()
{
	sphereIterations = 5;
	vertices = {
		0.f, 1.f, 0.f, //Top
		0.f, -1.f, 0.f, //Bottom

		1.f, 0.f, 0.f, //Mid ring points
		0.f, 0.f, 1.f,
		-1.f, 0.f, 0.f,
		0.f, 0.f, -1.f
	};
	indices = {
		0, 2, 3,
		0, 3, 4,
		0, 4, 5,
		0, 5, 2,

		1, 2, 3,
		1, 3, 4,
		1, 4, 5,
		1, 5, 2
	};
}

Cube::Cube()
{
	sphereIterations = 1;
	vertices = {
		-2.f, 2.f, 2.f,
		2.f, 2.f, 2.f,
		-2.f, -2.f, 2.f,
		2.f, -2.f, 2.f,

		-2.f, 2.f, -2.f,
		2.f, 2.f, -2.f,
		-2.f, -2.f, -2.f,
		2.f, -2.f, -2.f
	};
	indices = {
		5, 6, 7,     //front
		4, 5, 6,

		0, 1, 2,    //back
		1, 2, 3,

		1, 3, 5,    //right
		3, 5, 7,

		0, 2, 6,    //left
		0, 4, 6,

		0, 1, 4,    //top
		1, 4, 5,

		2, 3, 6,    //bottom
		3, 6, 7,
	};
}