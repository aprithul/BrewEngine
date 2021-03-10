#include "EditorUtils.hpp"
#include "Utils.hpp"
#include <assert.h>
namespace PrEngine
{
	std::vector<std::string> get_files_in_dir(const std::string& dir_name, const std::string& ext, std::vector<std::string>& directories)
	{
		HANDLE hFind;
		WIN32_FIND_DATAA FindFileData;
		std::string file_pattern = dir_name + "*.*";// +ext;
		hFind = FindFirstFileA(file_pattern.c_str(), &FindFileData);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do {
				if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
				{
					auto str = std::string(FindFileData.cFileName);
					if (str != "." && str != "..")
					{
						str = dir_name + str + PATH_SEP;
						get_files_in_dir(str, ext, directories);
					}
				}
				else
				{
					std::string file_name(FindFileData.cFileName);
					if (file_name.find(ext, file_name.size() - ext.size()) != std::string::npos)
					{
						std::string _path = dir_name + file_name;
						assert(_path.size() <= MAX_PATH);
						directories.push_back(_path);
					}
				}
			} while (FindNextFileA(hFind, &FindFileData));
		}
		//printf("FOUND: %s\n", FindFileData.cFileName);

		//// find directories
		//file_pattern = dir_name + "*";
		//hFind = FindFirstFileA(file_pattern.c_str(), &FindFileData);
		//
		//if (hFind != INVALID_HANDLE_VALUE)
		//{
		//	do {
		//		if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		//		{
		//			auto str = std::string(FindFileData.cFileName);
		//			if (str != "." && str != "..")
		//			{
		//				str = dir_name + str + PATH_SEP;
		//				get_files_in_dir(str, ext, directories);
		//			}
		//		}
		//	} while (FindNextFileA(hFind, &FindFileData));
		//}


		//LOG(LOGTYPE_WARNING, std::string(FindFileData.cFileName));

		return directories;
	}
}
