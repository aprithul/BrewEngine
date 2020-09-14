#include "GlAssert.hpp"

void gl_clear_error()
{
    while(glGetError() != GL_NO_ERROR); 
}

bool gl_get_error(char* func_name, char* file_name, int line_no)
{
    bool no_error = true;
    GLuint err = GL_NO_ERROR;
    while((err = glGetError())!=GL_NO_ERROR){
        std::cout<<"OpenGL error [ code: "<<err<<" ], in function: "<<func_name
                <<","<<" (line:"<<line_no<<") file: "<<file_name<<std::endl;
        no_error = false;
    }
    
    return no_error;
    
}