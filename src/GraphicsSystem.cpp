#include "GraphicsSystem.hpp"

#include "GraphicsComponent.hpp"
#include "TransformComponent.hpp"

#include "Log.hpp"

GraphicsSystem::GraphicsSystem(GraphXManager& manager) :
	manager(manager)
{
}

void GraphicsSystem::tick(ECS::World& ecs_world, float dt)
{
	ecs_world.each<GraphicsComponent>(
	[&](ECS::Entity& ent, ECS::ComponentHandle<GraphicsComponent> gc) {
		manager.ResetRenderingInstances();
		if(ent.has<TransformComponent>()) {
			// Get the transform component.
			auto transform = ent.get<TransformComponent>();

			glm::mat4 model = glm::scale(glm::translate(glm::mat4(), transform->position), glm::vec3(1.f, 3.f, 1.f));

			RenderingInstance ri;
			ri.model_matrix = model;
			ri.meshID = gc->meshID;
			ri.textureID[0] = gc->textureID[0];
			ri.textureID[1] = gc->textureID[1];
			ri.textureID[2] = gc->textureID[2];
			ri.textureID[3] = gc->textureID[3];
			ri.programID = gc->programID;
			manager.AddRenderingInstance(ri);
		}
	});
}
