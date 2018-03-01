#ifndef SCENEA_H
#define SCENEA_H

class SceneA
{
	public:
		void draw(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO);
	private:
		float vertices[24] = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left
		};
		unsigned int indices[6] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
		};


};

#endif
