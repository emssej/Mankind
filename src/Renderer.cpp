#include "Renderer.hpp"

#include <config.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Log.hpp"

#include "MathematX.hpp"

Renderer::Renderer() :
	window(nullptr),
	default_program(nullptr),
	sun(glm::vec3{0.3, 1.0, 0.3},
	    glm::vec3{0.5, 0.5, 0.5},
	    glm::vec3{1.0, 1.0, 1.0}),
	sky(nullptr)
{
	LOG("Initializing renderer");

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		throw std::runtime_error(SDL_GetError());
	}

	if (TTF_Init() == -1) {
		throw std::runtime_error(TTF_GetError());
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
}

Renderer::~Renderer()
{
	LOG("Destroying renderer");

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	TTF_Quit();
}

void Renderer::OpenWindow()
{
	LOG("Opening window");

	window = SDL_CreateWindow(PACKAGE_STRING,
	                          SDL_WINDOWPOS_CENTERED,
	                          SDL_WINDOWPOS_CENTERED,
	                          800, 600,
	                          SDL_WINDOW_SHOWN |
	                          SDL_WINDOW_OPENGL);

	if (window == nullptr) {
		throw std::runtime_error(SDL_GetError());
	}

	LOG("Creating GL context");

	context = SDL_GL_CreateContext(window);
	if (context == nullptr) {
		throw std::runtime_error(SDL_GetError());
	}

	LOG("Initializing GLEW");

	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Error initializing GLEW");
	}

	LOG("All initialized. Loading resources");

	main_font = std::make_shared<Font>("res/fonts/DejaVuSansMono.ttf", 16);

	SDL_GetWindowSize(window, &view_width, &view_height);
	version_label = std::make_unique<GUILabel>("version",
	                glm::vec2{5, view_height - 30}, glm::vec2{view_width, view_height}, main_font, PACKAGE_STRING);
	position_label = std::make_unique<GUILabel>("position",
	                 glm::vec2{5, view_height - 60}, glm::vec2{view_width, view_height}, main_font, "Position goes here");
	h_fov = 90.0f;
	v_fov_rad = xfov_to_yfov(deg2rad(h_fov), (float)view_width / (float)view_height);

	diffuse = std::make_shared<Texture>("res/tex/tiles_diffuse.png");
	specular = std::make_shared<Texture>("res/tex/tiles_specular.png");

	// Load default programs.
	this->default_program = std::make_unique<Program>("res/shaders/default.vert", "res/shaders/default.frag");
	this->sky_program = std::make_unique<Program>("res/shaders/sky.vert", "res/shaders/sky.frag");
	this->text_program = std::make_unique<Program>("res/shaders/text.vert", "res/shaders/text.frag");
	this->sky = std::make_unique<Sky>();
	uniform_mvp = default_program->GetUniform("MVP");

	LOG("Window correctly opened");

	glClearColor(0.6, 0.8, 1.0, 0.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::UpdateVectors(glm::vec3& angle, glm::vec3& forward,
                             glm::vec3& right, glm::vec3& lookat,
                             glm::vec3& up)
{
	forward.x = sinf(angle.x);
	forward.y = 0;
	forward.z = cosf(angle.x);
	right.x = -cosf(angle.x);
	right.y = 0;
	right.z = sinf(angle.x);
	lookat.x = sinf(angle.x) * cosf(angle.y);
	lookat.y = sinf(angle.y);
	lookat.z = cosf(angle.x) * cosf(angle.y);
	up = glm::cross(right, lookat);
}

void Renderer::Render(World& world, Camera& camera)
{
	LoadChunks(world, camera);

	SDL_GetWindowSize(window, &view_width, &view_height);

	position_label->SetText("(x,y,z) = %.2f %.2f %.2f", camera.x, camera.y, camera.z);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_POLYGON_OFFSET_FILL);

	DrawSky(camera);
	DrawTerrain(camera);
	DrawGUI();

	SDL_GL_SwapWindow(window);
}

void Renderer::AddChunk(World& world, int x, int y, int z, Chunk& c)
{
	this->chunk_meshes.insert(std::make_pair(CHUNK_ID(x, y, z), ChunkMesh(world, c, x, y, z, diffuse, specular)));
}

void Renderer::DrawSky(Camera& camera)
{
	glDisable(GL_DEPTH_TEST);

	this->sky_program->Use();
	this->sky_program->SetVec3("sun_direction", this->sun.direction);
	this->sky_program->SetFloat("camera_pitch", camera.pitch);
	this->sky_program->SetFloat("camera_yaw", camera.yaw);
	this->sky_program->SetFloat("vertical_fov", v_fov_rad);
	this->sky_program->SetFloat("horizontal_fov", deg2rad(h_fov));
	this->sky->Render();

	glEnable(GL_DEPTH_TEST);
}

void Renderer::DrawTerrain(Camera& camera)
{
	glm::vec3 position(camera.x, camera.y, camera.z);
	glm::vec3 angle(camera.yaw, camera.pitch, camera.roll);
	glm::vec3 forward, right, lookat, up;
	UpdateVectors(angle, forward, right, lookat, up);
	glm::mat4 view = glm::lookAt(position, position + lookat, up);
	glm::mat4 projection = glm::perspective(v_fov_rad, 1.0f*view_width/view_height, 0.01f, 1000.0f);

	this->default_program->Use();
	this->default_program->SetVec3("camera_position", position);
	this->sun.AddToProgram(*(this->default_program), 0);

	for(auto& kc : this->chunk_meshes) {
		int x(kc.second.GetX() * CHUNK_SIZE),
		    y(kc.second.GetY() * CHUNK_SIZE),
		    z(kc.second.GetZ() * CHUNK_SIZE);
		glm::mat4 translate = glm::translate(glm::mat4(), glm::vec3(x, y, z));
		glm::mat4 mvp = projection * view * translate;
		glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

		kc.second.Render();
	}
}

void Renderer::DrawGUI()
{
	text_program->Use();
	position_label->Draw();
	version_label->Draw();
}

void Renderer::LoadChunks(World& world, Camera& camera)
{
	// find out camera chunk
	int ccx(camera.x / CHUNK_SIZE),
	    ccy(camera.y / CHUNK_SIZE),
	    ccz(camera.z / CHUNK_SIZE);

	// Add a chunk.
	for (int i(ccx-5); i < ccx+5; ++i) {
		for (int j(ccy-5); j < ccy+5; ++j) {
			for (int k(ccz-5); k < ccz+5; ++k) {
				int64_t chunkid = CHUNK_ID(i, j, k);
				if (chunk_meshes.find(CHUNK_ID(i, j, k)) == chunk_meshes.end()) {
					this->AddChunk(world, i, j, k, world.GetChunk(i, j, k));
				} else {
				}
			}
		}
	}

}

