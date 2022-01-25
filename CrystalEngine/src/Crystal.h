#pragma once
#include <memory>
#include <glm/glm.hpp>

namespace crystal
{
	using Float32 = float;
	using Float64 = double;
#ifdef CRYSTAL_USE_FLOAT32
	using Float = float;
#else
	using Float = double;
#endif

	// Core
	class Engine;
	class Application;

	// GraphicsAPI
	class IGraphicsDevice;
	class IGraphicsResourceManager;
	class IGraphicsProvider;

	// Platform
	class IGameWindow;
	class IFileSystem;
	class IPlatformProvider;

	class GameTimer;
}