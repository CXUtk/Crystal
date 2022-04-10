#pragma once
#include <fstream>
#include <memory>
#include "FSCommon.h"

namespace crystal
{
	class IFileStream
	{
	public:
		IFileStream(const path_type& path, FileMode fileMode,
			FileAccess fileAccess);
		~IFileStream();

	private:
		std::ifstream	m_ifStream;
		std::ofstream	m_ofStream;
		std::fstream	m_fStream;

		FileAccess		m_fileAccess;
		path_type		m_filePath;
	};
}