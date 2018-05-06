#ifndef GENERIC_SHAPES_H
#define GENERIC_SHAPES_H

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

#include "Shape.h"

class Sphere : public Shape
{
public:
	Sphere();
};

class Cube : public Sphere
{
public:
	Cube();
};

#endif