#include "GUILabel.hpp"
#include "FormatString.hpp"

#include "GUI.hpp"
#include "Log.hpp"

/* Because text is rendered with SDL_TTF,
   the Y-coordinate must be reversed */
#define UV_BOTTOM_LEFT  {0, 1}
#define UV_BOTTOM_RIGHT {1, 1}
#define UV_TOP_LEFT     {0, 0}
#define UV_TOP_RIGHT    {1, 0}

void GUILabel::Reload()
{
	auto root = element.GetRoot();
	glm::vec2 screen_size(element.GetScreenSizePx());
	this->pos.x = -1 + ((float)pos.x / screen_size.x * 2.);
	this->pos.y = -1 + ((float)pos.y / screen_size.y * 2.);
	SDL_Color fg_color{255, 255, 255, 255};

	Font& font = element.GetGUI().GetFont(element.GetAttribute("font-family"));

	SDL_Surface* text_surface = TTF_RenderText_Blended(font.GetPointer(), element.GetText().c_str(), fg_color);
	this->fg_texture = std::make_shared<Texture>(text_surface);
	glm::vec2 wh{
		(float) text_surface->w / screen_size.x * 2,
		(float) text_surface->h / screen_size.y * 2};

	element.SetSizePx(glm::vec2{text_surface->w, text_surface->h});

	SDL_FreeSurface(text_surface);

	/* 0 - 1       .-->
	   | / | axis: |
	   2 - 3       v
	*/
	std::vector<glm::vec3> vertices(6, {0, 0, 0});

	for (auto& v : vertices) {
		v += glm::vec3(pos.x, pos.y, 0.f);
	}
	vertices[0] += glm::vec3(0.f, 0.f, 0.f);
	vertices[1] += glm::vec3(wh.x, 0.f, 0.f);
	vertices[2] += glm::vec3(0.f, -wh.y, 0.f);

	vertices[3] += glm::vec3(0.f, -wh.y, 0.f);
	vertices[4] += glm::vec3(wh.x, 0.f, 0.f);
	vertices[5] += glm::vec3(wh.x, -wh.y, 0.f);

	std::vector<glm::vec2> uvs{
		UV_TOP_LEFT,
		UV_TOP_RIGHT,
		UV_BOTTOM_LEFT,

		UV_BOTTOM_LEFT,
		UV_TOP_RIGHT,
		UV_BOTTOM_RIGHT
	};
	std::vector<GLuint> indices {
		0, 1, 2, 3, 4, 5
	};
	std::vector<glm::vec3> normals(6, glm::vec3(0, 0, -1));

	foreground = std::make_unique<Mesh>(vertices, normals, uvs, indices);
}

void GUILabel::Draw(Program& program)
{
	glEnable(GL_BLEND);
	glActiveTexture(GL_TEXTURE0);
	this->fg_texture->Bind();
	GUIDrawer::Draw(program);
	glDisable(GL_BLEND);
}

