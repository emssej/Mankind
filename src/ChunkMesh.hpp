#pragma once

#include "GraphX.hpp"
#include "Chunk.hpp"
#include "Program.hpp"
#include "Mesh.hpp"
#include "TileManager.hpp"
#include "Terrain.hpp"

class ChunkMesh
{
private:
	Terrain& terrain;
	Chunk& chunk;
	std::unique_ptr<Mesh> mesh;
	void Update();
	std::vector<glm::vec3> vertices;
	int x, y, z;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<GLuint> indices;
	std::vector<GLuint> real_indices;
	std::vector<glm::vec3> real_vertices;
	int global_index;
	void PushFace(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 norm, int typeID);
	/* v MUST MOVE ASAP */
	TileManager &tile_manager;
public:
	ChunkMesh(Terrain& t, Chunk& chunk, TileManager& tile_manager, int x, int y, int z);
	void Render();

	int GetX()
	{
		return x;
	}
	int GetY()
	{
		return y;
	}
	int GetZ()
	{
		return z;
	}
};

