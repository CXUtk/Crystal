#pragma once
#include <memory>
#include <set>
#include <map>
#include <glm/glm.hpp>

namespace crystal
{
	using Float32 = float;
	using Float64 = double;
#ifdef CRYSTAL_USE_FLOAT64
	using Float = double;
#else
	using Float = float;
#endif
	using Byte = unsigned char;

	
	// Core
	class Engine;
	class Application;
	class GameTimer;
	class InputController;
	class SpriteBatch;

	// Data
	struct InitArgs;

	// Platforms & GraphicsAPI
	
	// For creating all other objects (textures, buffers, etc.)
	class IGraphicsDevice;

	class IGameWindow;
	class IPlatformProvider;
	class IFileSystem;


	// Templates
	/**
	 * @brief Converts a shared pointer to a reference of object
	 * @tparam T Object Type
	 * @param ptr Target shared ponter
	 * @return reference to the object
	*/
	template<typename T>
	inline T& ref(const std::shared_ptr<T>& ptr) { return static_cast<T&>(*ptr); }

	/**
	 * @brief Converts a shared pointer to a const reference of object
	 * @tparam T Object Type
	 * @param ptr Target shared ponter
	 * @return const reference to the object
	*/
	template<typename T>
	inline const T& cref(const std::shared_ptr<T>& ptr) { return static_cast<const T&>(*ptr); }

	/**
	 * @brief Converts a shared pointer to a pointer of the object
	 * @tparam T Object Type
	 * @param ptr Target shared ponter
	 * @return const reference to the object
	*/
	template<typename T>
	inline T* ptr(const std::shared_ptr<T>& ptr) { return static_cast<T*>(ptr.get()); }

	/**
	 * @brief Converts a shared pointer to a pointer of the object
	 * @tparam T Object Type
	 * @param ptr Target shared ponter
	 * @return const reference to the object
	*/
	template<typename T>
	inline const T* cptr(const std::shared_ptr<T>& ptr) { return static_cast<const T*>(ptr.get()); }

	/**
	 * @brief Converts a unique pointer to a pointer of the object
	 * @tparam T Object Type
	 * @param ptr Target shared ponter
	 * @return const reference to the object
	*/
	template<typename T>
	inline T* ptr(const std::unique_ptr<T>& ptr) { return static_cast<T*>(ptr.get()); }


	/**
	 * @brief Converts a unique pointer to a const pointer of object
	 * @tparam T Object Type
	 * @param ptr Target shared ponter
	 * @return const reference to the object
	*/
	template<typename T>
	inline const T* cptr(const std::unique_ptr<T>& ptr) { return static_cast<const T*>(ptr.get()); }
}