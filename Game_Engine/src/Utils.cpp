#include "Utils.hpp"
#include <algorithm>

#define DEBUG_BASE_PATH

namespace PrEngine{
    // write to file
	void write_to_file(const std::string& text, const std ::string& file_name, Bool_8 binary, Bool_8 append)
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


#ifdef _WEB
		LOG(LOGTYPE_GENERAL, file_name);
		std::string output;
		FILE *file = fopen(file_name.c_str(), "rb");
		if (!file) {
			printf("cannot open file\n");
			return "";
		}
		while (!feof(file)) {
			char c = fgetc(file);
			if (c != EOF) {
				output += c;
			}
		}
		fclose(file);
		return output;
#else
		std::string _file_name(file_name);
		trim(_file_name);

		LOG(LOGTYPE_GENERAL, _file_name);
		std::ifstream _file(file_name);
		std::stringstream buffer;
		buffer << _file.rdbuf();
		return buffer.str();
#endif
//		
    }
    

    std::string get_resource_path(const std::string &subDir){
        //We need to choose the path separator properly based on which
        //platform we're running on, since Windows uses a different
        //separator than most systems
    
#ifdef _WIN64


        static std::string base_path;
        if (base_path.empty()){
            //SDL_GetBasePath will return NULL if something went wrong in retrieving the path
            Char_8 *_base_path = SDL_GetBasePath();
            if (_base_path){
                base_path = _base_path;
#ifdef DEBUG_BASE_PATH
				base_path = (std::string(_base_path)+".."+PATH_SEP+".."+PATH_SEP+".."+PATH_SEP).c_str();
#endif // DEBUG_base

                SDL_free(_base_path);
            }
            else {
                LOG(LOGTYPE_ERROR, std::string("Error getting resource path:").append(std::string(SDL_GetError())));
                return "";
                
            }
            //We replace the last bin/ with res/ to get the the resource path
            
            base_path = base_path + "res" + PATH_SEP;
        }
        //If we want a specific subdirectory path in the resource directory
        //append it to the base path. This would be something like Lessons/res/Lesson0
#endif // _WIN64
#ifdef _SWITCH
		static std::string base_path = "res" + PATH_SEP;
#endif // _SWITCH
#ifdef _WEB
		static std::string base_path = "res" + PATH_SEP;
#endif // _WEB

        return subDir.empty() ? base_path : base_path + subDir;
    }


	Uint_32 str_hash(const std::string& str)
	{
		Uint_32 _hash = 0;
		for (auto& c : str)
		{
			_hash += c;
		}

		return _hash;
	}

	void trim(std::string& str)
	{
		if (!str.empty())
		{
			str.erase(0, str.find_first_not_of("\t\n\v\f\r ")); // left trim
			str.erase(str.find_last_not_of("\t\n\v\f\r ") + 1); // right trim
		}
	}

/*    Bool_8 inside(Vec2f pos, SDL_FRect& rect, Bool_8 centered)
    {
        if(!centered)
        {
            if(pos.x<=rect.x+rect.w && pos.x>=rect.x &&
                pos.y<=rect.y+rect.h && pos.y>=rect.y)
                    return true;
        }
        else
        {
            if(pos.x<=rect.x+rect.w/2 && pos.x>=rect.x-rect.w/2 &&
                pos.y<=rect.y+rect.h/2 && pos.y>=rect.y-rect.h/2)
                    return true;
        }
        
        return false;
    }*/

	/* includes range */
	Float_32 RandomFraction(Float_32 low, Float_32 high) {
		Float_32 rand_frac = ((float)(rand() % 1001)) / (float)1000.0;
		return low + ((high - low)*rand_frac);
	}

	void tokenize_string(const std::string& str, char delim, std::vector<std::string>& tokens)
	{
		std::stringstream stream(str);
		std::string token;

		while (std::getline(stream, token, delim))
		{
			tokens.push_back(token);
		}
	}


	PerformanceTimer::PerformanceTimer()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	PerformanceTimer::~PerformanceTimer()
	{
		using namespace std::chrono;
		auto _now = high_resolution_clock::now();
		duration<double> time_span = duration_cast<duration<double>>(_now - start);
		std::cout << "Duration: "<< time_span.count() << std::endl;
	}
}
