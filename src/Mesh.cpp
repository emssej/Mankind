#include "Mesh.hpp"

Mesh::Mesh(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> uvs, std::vector<unsigned int> indices)
{
  this->vertices = vertices;
  this->normals = normals;
  this->uvs = uvs;
  this->indices = indices;
}

void Mesh::Initialize()
{
  // Generate the VAO.
  glGenVertexArrays(1, &this->VAO);
  glBindVertexArray(this->VAO);

  // Generate the buffer objects.

  // VBO
  glGenBuffers(1, &this->VBO);
  glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
  glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &this->vertices[0], GL_STATIC_DRAW);

  // NBO
  glGenBuffers(1, &this->NBO);
  glBindBuffer(GL_ARRAY_BUFFER, this->NBO);
  glBufferData(GL_ARRAY_BUFFER, this->normals.size() * sizeof(glm::vec3), &this->normals[0], GL_STATIC_DRAW);

  // UVBO
  glGenBuffers(1, &this->UVBO);
  glBindBuffer(GL_ARRAY_BUFFER, this->UVBO);
  glBufferData(GL_ARRAY_BUFFER, this->uvs.size() * sizeof(glm::vec2), &this->uvs[0], GL_STATIC_DRAW);

  // EBO
  glGenBuffers(1, &this->EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

  // Now, I don't understand what's going on here.

  // VBO
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  // NBO
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, this->NBO);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

  // UVBO
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
}

Mesh::~Mesh()
{
  // ok do nutan 4 now
}

void Mesh::Render()
{
  glBindVertexArray(this->VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);

  glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, (void*) 0);

  glBindVertexArray(0);
}