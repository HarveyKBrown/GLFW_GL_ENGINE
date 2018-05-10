#ifndef SHAPE_H
#define SHAPE_H

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>

class Shape
{
	/* Default Values of a Cube */
public:
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

public:
	void draw(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO);
	int getNumVectors() { vertices.size(); }
	glm::mat4 getPosMatrix();

	/* Position and Roation */
	glm::vec3 position;
	glm::vec3 orientation;
	glm::vec3 color;
	float scale = 1;
	int sphereIterations = 0;
};

#endif