#pragma once
#include <string>
#include <set>

namespace sa
{
	class DirectoryHelper
	{
	public:
		std::string FindFile(const std::string& file);
	private:
		std::string FindFile(const std::string& currentDir, const std::string& file); 
		std::set<std::string> m_visitedDirs; 
	};

	class PathString
	{
	public:
		PathString(const std::string& path);

		const std::string& GetFilePart() const { return m_filePart; }
		const std::string& GetDirPart() const { return m_dirPart; }
	private:
		std::string m_path;
		std::string m_filePart;
		std::string m_dirPart;
	};
}
