#include "PhysicsSystem.hpp"

#include "TransformComponent.hpp"
#include "PhysicsComponent.hpp"

#include "Log.hpp"

#include <ctime>

bool PhysicsSystem::AABBTest(glm::vec3 v, glm::vec3 box, glm::vec3 whd)
{
	for (int i(0); i < 3; ++i) {
		if (v[i] < box[i] or v[i] > box[i] + whd[i]) {
			return false;
		}
	}
	return true;
}

void PhysicsSystem::tick(ECS::World& ecs_world, float dt)
{
	ecs_world.each<PhysicsComponent>(
	[&](ECS::Entity& ent,ECS::ComponentHandle<PhysicsComponent> body) {
		body->vel += glm::vec3(0, -G_VALUE, 0) * dt;
		if (abs(body->vel.y) > TERMINAL_VELOCITY) {
			body->vel.y = TERMINAL_VELOCITY;
		}

		auto tc = ent.get<TransformComponent>();
		if (tc.isValid()) {
			glm::vec3 ds = body->vel * dt;
			glm::vec3 new_pos = tc->position + ds;
			glm::vec3 box = body->box_whd;

			/* X direction: for every block contained within the box */
			for (float x(new_pos.x); x <= new_pos.x + box.x; x += 1.) {
				for (float y(tc->position.y); y <= tc->position.y + box.y; y += 1.) {
					for (float z(tc->position.z); z <= tc->position.z + box.z; z += 1) {
						if (terrain.GetCube(x, y, z).IsSolid() ) {
							if (body->vel.x < 0) {
								new_pos.x = floor(tc->position.x) + EPSILON;
							} else if (body->vel.x > 0) {
								new_pos.x = tc->position.x - EPSILON - box.x;
							}
							body->vel.x = 0;
						}
					}
				}
			}

			/* Y direction: for every block contained within the box */
			for (float x(tc->position.x); x <= tc->position.x + box.x; x += 1.) {
				for (float y(new_pos.y); y <= new_pos.y + box.y; y += 1.) {
					for (float z(tc->position.z); z <= tc->position.z + box.z; z += 1) {
						if (terrain.GetCube(x, y, z).IsSolid() ) {
							if (body->vel.y < 0) {
								new_pos.y = floor(tc->position.y) + EPSILON;
							} else if (body->vel.y > 0) {
								new_pos.y = tc->position.y - EPSILON - box.y;
							}
							body->vel.y = 0;
						}
					}
				}
			}

			/* Z direction: for every block contained within the box */
			for (float x(tc->position.x); x <= tc->position.x + box.x; x += 1.) {
				for (float y(tc->position.y); y <= tc->position.y + box.y; y += 1.) {
					for (float z(new_pos.z); z <= new_pos.z + box.z; z += 1) {
						if (terrain.GetCube(x, y, z).IsSolid() ) {
							if (body->vel.z < 0) {
								new_pos.z = floor(tc->position.z) + EPSILON;
							} else if (body->vel.z > 0) {
								new_pos.z = tc->position.z - EPSILON - box.z;
							}
							body->vel.z = 0;
						}
					}
				}
			}

			tc->position = new_pos;
		}
	});
}

