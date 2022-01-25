#pragma once
namespace crystal
{
	enum class FileMode
	{
		Create,
		OpenRead,
		OpenWrite,
	};

	enum class FileAccessMode
	{
		Read,
		Write,
		ReadWrite,
	};

	class IGameWindow;
	class IFileSystem;
	class IPlatformProvider;
}