#include "Utils.hpp"

namespace PrEngine{
    // writes text to file in append mode
    // doesn't automaticlaly close file
    // file pointer needs to be provided so file can be closed when writing is finished
    FILE* write_to_file(const char* text, const char* file_name, FILE *fp)
    {
        if(fp == NULL)
        {
            fp = fopen(file_name, "a");
        }
        fputs(text, fp); 
        return fp;
    }

    void close_file(FILE* fp)
    {
    	if(fp)
    		fclose(fp);
    }

    // reads content of file_name, character by character
    // stores in buffer and returns
    // dynamically increases in size if needed
    char* read_file(const char* file_name)
    {
        int n = 1024;
        FILE *fp;
        char* buffer = (char*)malloc(n*sizeof(char));
        int read_char_count = 0;
        fp = fopen(file_name, "r");
        
        if(fp!=NULL)
        {
            int ch;
            while((ch = fgetc(fp)) != EOF)
            {
                read_char_count++;
                if(read_char_count > n)
                {
                    n*=2;
                    buffer = (char*)realloc(buffer, sizeof(char)*n);
                }
                *(buffer+read_char_count-1) = ch; 
            }
            fclose(fp); 
        }
        else
            buffer = NULL;
    
        *(buffer+read_char_count) = '\0'; 
        return buffer;
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
            char *_base_path = SDL_GetBasePath();
            if (_base_path){
                base_path = _base_path;
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

    Rect::Rect(float x,float y,float w,float h)
    {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }
    
    bool inside(Vector2<int> pos, SDL_Rect& rect, bool centered)
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

    bool inside(Vector2<float> pos, SDL_Rect& rect, bool centered)
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

/*    bool inside(Vector2<float> pos, SDL_FRect& rect, bool centered)
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
