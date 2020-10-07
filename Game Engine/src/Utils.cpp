#include "Utils.hpp"
#define DEBUG_BASE_PATH

namespace PrEngine{
    // writes text to file in append mode
    // doesn't automaticlaly close file
    // file pointer needs to be provided so file can be closed when writing is finished
	void write_to_file(const std::string& text, const std ::string& file_name, Bool_8 binary, Bool_8 append)
    {
		std::ofstream _file(file_name, (append?std::ios::app:0) | (binary?std::ios::binary:0));
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
    

    std::string get_resource_path(const std::string &subDir){
        //We need to choose the path separator properly based on which
        //platform we're running on, since Windows uses a different
        //separator than most systems
    
        //This will hold the base resource path: Lessons/res/
        //We give it static lifetime so that we'll only need to call
        //SDL_GetBasePath once to get the executable path
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
        return subDir.empty() ? base_path : base_path + subDir;
    }

    Rect::Rect()
    {
        x = 0;
        y = 0;
        w = 0;
        h = 0;
    }

    Rect::Rect(Float_32 x,Float_32 y,Float_32 w,Float_32 h)
    {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }
    
    Bool_8 inside(Vector2<Int_32> pos, SDL_Rect& rect, Bool_8 centered)
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
    }

    Bool_8 inside(Vector2<Float_32> pos, SDL_Rect& rect, Bool_8 centered)
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
    }

/*    Bool_8 inside(Vector2<Float_32> pos, SDL_FRect& rect, Bool_8 centered)
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


}
