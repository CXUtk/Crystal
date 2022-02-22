#include "IFileStream.h"
#include "File.h"
#include "Path.h"
#include <exception>
#include <Core/Utils/Misc.h>

namespace crystal
{
	IFileStream::IFileStream(const path_type& path, FileMode fileMode, FileAccess fileAccess)
		: m_fileAccess(fileAccess)
	{
		std::ios_base::openmode mode = 0;

		m_filePath = Path::Normalize(path);

		if (fileMode == FileMode::Open)
		{
			if (!File::Exists(m_filePath))
			{
				throw std::runtime_error(string_format("File %s does not exist", m_filePath.c_str()));
			}
		}

		if (fileMode == FileMode::Append)
		{
			mode |= std::ios::app;
		}

		if (m_fileAccess == CRYSTAL_FILE_ACCESS_READ)
		{
			m_ifStream.open(path, mode | std::ios::in);
		}
		else if (m_fileAccess == CRYSTAL_FILE_ACCESS_WRITE)
		{
			m_ofStream.open(path, mode | std::ios::out);
		}
		else if (m_fileAccess == (CRYSTAL_FILE_ACCESS_READ | CRYSTAL_FILE_ACCESS_WRITE))
		{
			m_fStream.open(path, mode | std::ios::in | std::ios::out);
		}
		else
		{
			throw std::invalid_argument("Invalid file access mode");
		}
	}

	IFileStream::~IFileStream()
	{
		if (m_fileAccess == CRYSTAL_FILE_ACCESS_READ)
		{
			m_ifStream.close();
		}
		else if (m_fileAccess == CRYSTAL_FILE_ACCESS_WRITE)
		{
			m_ofStream.close();
		}
		else if (m_fileAccess == (CRYSTAL_FILE_ACCESS_READ | CRYSTAL_FILE_ACCESS_WRITE))
		{
			m_fStream.close();
		}
	}
}