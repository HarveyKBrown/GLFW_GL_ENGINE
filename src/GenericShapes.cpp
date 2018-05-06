#include "GenericShapes.h"

Sphere::Sphere()
{
	sphereIterations = 5;
	vertices = {
		0.f, 0.5f, 0.f, //Top
		0.35f, 0.35f, 0.f, //Top Rings
		0.f, 0.35f, 0.35f,
		-0.35f, 0.35f, 0.f,
		0.f, 0.35f, -0.35f,

		0.5f, 0.0f, 0.0f, //Mid Ring
		0.f, 0.0f, 0.5f,
		-0.5f, 0.0f, 0.f,
		0.f, 0.0f, -0.5f,

		0.35f, -0.35f, 0.f, //Bottom Ring
		0.f, -0.35f, 0.35f,
		-0.35f, -0.35f, 0.f,
		0.f, -0.35f, -0.35f,

		0.f, -0.5f, 0.f //0-13
	};
	indices = {
		0, 1, 2,     //Top - TopRing
		0, 2, 3,
		0, 3, 4,
		0, 4, 1,

		1, 5, 6,	//TopRing - MiddleRing
		2, 6, 7,
		3, 7, 8,
		4, 8, 5,
		5, 1, 2,	//MiddleRing - TopRing
		6, 2, 3,
		7, 3, 4,
		8, 4, 1,

		5, 9, 10,	//MiddleRing - BottomRing
		6, 10, 11,
		7, 11, 12,
		8, 12, 9,
		9, 5, 6,	//BottomRing - TopRing
		10, 6, 7,
		11, 7, 8,
		12, 8, 5,

		13, 9, 10,     //Bottom - BottomRing
		13, 10, 11,
		13, 11, 12,
		13, 12, 9
	};
}

Cube::Cube()
{
	sphereIterations = 1;
	vertices = {
		-0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,

		-0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f
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