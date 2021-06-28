#include <stdio.h>
#include <vector>
#include <assert.h>
#include <fstream>
#include <sstream>
#ifdef  _WIN64
#include <windows.h>
#include <fileapi.h>
#endif 


std::vector<std::string> get_files_in_dir(const std::string& dir_name, const std::string& ext, std::vector<std::string>& directories)
{

#ifdef _WIN64
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
					str = dir_name + str + "/";
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
#endif // _WIN64

	return directories;
}


// write to file
void write_to_file(const std::string& text, const std::string& file_name, bool binary, bool append)
{
	std::ios_base::openmode mode = std::ios_base::out;
	if (binary)
		mode |= std::ios::binary;
	if (append)
		mode |= std::ios::app;

	std::ofstream _file(file_name, mode);
	if (_file.is_open())
	{
		_file << text;
		_file.close();
	}
}

// reads content of file_name, character by character
// stores in buffer and returns
// dynamically increases in size if needed
std::string read_file(const std::string& file_name)
{
	std::ifstream _file(file_name);
	std::stringstream buffer;
	buffer << _file.rdbuf();
	return buffer.str();
}

void trim(std::string& str)
{
	str.erase(0, str.find_first_not_of("\t\n\v\f\r ")); // left trim
	str.erase(str.find_last_not_of("\t\n\v\f\r ") + 1); // right trim
}

int main()
{
	std::string dir = "D:/PrEngine/PrEngine2D/Game/include/Scripts/";
	std::vector<std::string> dirs;
	std::vector<std::string> type_names;
	std::vector<std::string> headers;

	printf("Running MetaTool...\n");


	get_files_in_dir(dir, ".hpp", dirs);
	for (auto d : dirs)
	{

		// extract name of header and store in headers vector
		std::stringstream header_stream(d);
		std::string header_token;
		std::vector<std::string> header_tokens;
		while (std::getline(header_stream, header_token, '/'))
		{
			header_tokens.push_back(header_token);
		}
		headers.push_back(header_tokens.back());


		// get name of struct and store in type_names
		std::string file_text = read_file(d);
		
		std::stringstream file_text_stream(file_text);
		std::string line;
		while (std::getline(file_text_stream, line))
		{
			std::stringstream line_text_stream(line);
			std::string token;
			while (std::getline(line_text_stream, token, ':'))
			{
				std::stringstream token_stream(token);
				std::string type_name;
				bool next_token_is_name = false;
				//printf("%s\n", token.c_str());

				while (std::getline(token_stream, type_name, ' '))
				{
					trim(type_name);
					if (type_name == "struct")
						next_token_is_name = true;
					else if (next_token_is_name)
						type_names.push_back(type_name);
				}
			}

		}
	}

	// write to file
	std::string output_file = "D:/PrEngine/PrEngine2D/Game/src/ScriptGenerator.cpp";
	std::string types_res_file = "D:/PrEngine/PrEngine2D/Game/res/type_names.csv";
	if (type_names.size() > 0)
	{
		std::string output =
			"#include <string>\n";
		std::string type_name_output = type_names[0];
		for (int _i = 1; _i < type_names.size(); _i++)
		{
			type_name_output += "," + type_names[_i];
		}

		for (auto header : headers)
		{
			output += "#include \"Scripts/" + header + "\"\n";
		}

		/*output += "std::string type_names[" + std::to_string(type_names.size()) + "]={\""+ type_names[0]+"\"";

		for (int _i=1; _i<type_names.size(); _i++)
		{
			output += (",\""+type_names[_i])+"\"";
		}
		output += "};\n";*/

		output += "PrEngine::Script* get_script_of_type(const char* type_name)\n{\n";
		
		for (auto type_name : type_names)
		{
			output +=
				"\tif(strcmp(type_name,\"" + type_name + "\")==0){\n"
				"\t\treturn new " + type_name + "();\n\t}\n";
		}

		output += "}";
		
		//printf("%s\n", output.c_str());
		write_to_file(output, output_file, true, false);
		write_to_file(type_name_output, types_res_file, true, false);
	}

	return 0;

}