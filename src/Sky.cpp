#include "Sky.hpp"

//   2---4
//   |\  |
//   | \ |
//   |  \|
//   1___3

/// Generate the fullscreen quad.


#define v1 glm::vec3{-1.0, -1.0, 0.0}
#define v2 glm::vec3{-1.0, 1.0, 0.0}
#define v3 glm::vec3{1.0, -1.0, 0.0}
#define v4 glm::vec3{1.0, 1.0, 0.0}

#define u1 glm::vec2{0, 0}
#define u2 glm::vec2{0, 1}
#define u3 glm::vec2{1, 0}
#define u4 glm::vec2{1, 1}

#define vertices std::vector<glm::vec3>{v1, v2, v3, v3, v2, v4}

// We don't really need them, but whatever.
#define normals std::vector<glm::vec3>{6, glm::vec3{0.0, 0.0, -1.0}}

#define uvs std::vector<glm::vec2>{u1, u2, u3, u2, u4, u3}

#define indices std::vector<GLuint>{0, 1, 2, 3, 4, 5}

Sky::Sky():
	Mesh::Mesh(
	    vertices, normals, uvs, indices,
	    nullptr, nullptr
	)
{

}

void Sky::Render()
{
	glDisable(GL_DEPTH_TEST);
	Mesh::Render();
	glEnable(GL_DEPTH_TEST);
}
