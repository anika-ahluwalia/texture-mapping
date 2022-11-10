#pragma once
#include <GL/glew.h>
#include <iostream>

namespace Debug
{
    inline void glErrorCheck(const char* filename, int line_number){
        GLenum error = glGetError();
        while(error != GL_NO_ERROR){
            std::cout<< "ERROR! " << filename << " at line " << line_number << " contains the following error: " << error<<std::endl;
            error = glGetError();
        }
    }

    #define glErrorCheck() glErrorCheck(__FILE__, __LINE__)
}
