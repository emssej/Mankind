#pragma once

#include <vector>
#include <string>
#include <memory>

#include <glm/glm.hpp>

#include "GraphX.hpp"
#include "Texture.hpp"
#include "Program.hpp"

class Mesh
{
private:
	// Vertex Array Object, Vertex Buffer Object, Normal Buffer Object, UV Buffer Object, Element Buffer Object.
	GLuint VAO, VBO, NBO, UVBO, EBO;
public:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<GLuint> indices;

	// TODO: 2D mesh or something
	Mesh(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals,
	     std::vector<glm::vec2> uvs, std::vector<GLuint> indices);
	~Mesh();

	virtual void Render();
};
