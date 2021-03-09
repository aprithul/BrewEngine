#ifndef EDITOR_UTILS_HPP
#define EDITOR_UTILS_HPP

#include <vector>
#ifdef  _WIN32
#include <windows.h>
#include <fileapi.h>
#endif 
namespace PrEngine
{
	std::vector<std::string> get_files_in_dir(const std::string& dir_name, const std::string& ext, std::vector<std::string>& directories);
}

#endif // !EDITOR_UTILS_HPP
