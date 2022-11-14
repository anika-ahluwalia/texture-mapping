#pragma once
#include <GL/glew.h>
#include <iostream>

namespace Debug
{
    inline void glErrorCheck(const char* filename, int line_number){
        std::cout << "before error" << std::endl;
        GLenum error = glGetError();
        std::cout << "after error" << std::endl;
        while(error != GL_NO_ERROR){
            std::cout<< "ERROR! " << filename << " at line " << line_number << " contains the following error: " << error<<std::endl;
            error = glGetError();
        }
    }

    #define glErrorCheck() glErrorCheck(__FILE__, __LINE__)
}
