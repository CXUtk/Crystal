#pragma once
#include <memory>
#include <glm/glm.hpp>

namespace crystal
{
	using Point2f = glm::vec2;
	using Point2i = glm::ivec2;
	using Point3f = glm::vec3;
	using Vector2f = glm::vec2;
	using Vector3f = glm::vec3;
	using Normal3f = glm::vec3;
	using Spectrum = glm::vec3;
	using Color3f = glm::vec3;
	using Float = float;


	// Core
	class Engine;
	class Application;
}