#include "GlAssert.hpp"

void gl_clear_error()
{
    while(glGetError() != GL_NO_ERROR); 
}

Bool_8 gl_get_error(Char_8* func_name, Char_8* file_name, Int_32 line_no)
{
    Bool_8 no_error = true;
    GLuint err = GL_NO_ERROR;
    while((err = glGetError())!=GL_NO_ERROR){
        std::cout<<"OpenGL error [ code: "<<err<<" ], in function: "<<func_name
                <<","<<" (line:"<<line_no<<") file: "<<file_name<<std::endl;
        no_error = false;
    }
    
    return no_error;
    
}