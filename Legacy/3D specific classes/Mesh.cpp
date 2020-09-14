//
//  Mesh.cpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/31/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//

#include "Mesh.hpp"
#include <iostream>

namespace Pringine {
    
    Mesh::Mesh()
    {
        
    }
    
    Mesh::~Mesh()
    {
        delete [] flattened_vertices;
    }
    
    void Mesh::set_vertices(Vertex* vertices, int vertex_count)
    {
        this->vertices = vertices;
        flattened_vertices = new GLfloat[vertex_count*Vertex::VERTEX_LENGTH];
        // flatten and put in array
        for(int _i = 0; _i < vertex_count; _i++ )
        {
            GLfloat* flattened_array = vertices[_i].get_flattened_attributes_array();
            for(int _j = 0; _j < Vertex::VERTEX_LENGTH; _j++)
            {
                flattened_vertices[ (_i*Vertex::VERTEX_LENGTH) + _j ] = flattened_array[_j];
            }
        }
        size = vertex_count*Vertex::SIZE;
        this->vertex_count = vertex_count;
    }
    
    
    GLfloat* Mesh::get_vertex_array_flat()
    {
        return flattened_vertices;
    }
    

}

/*
 //
 //  main.cpp
 //  OpenGL-Learning
 //
 //  Created by Aniruddha Prithul on 11/10/17.
 //  Copyright © 2017 Aniruddha Prithul. All rights reserved.
 //
 
 #include <iostream>
 #include <OpenGL/OpenGL.h>
 #include <GLFW/glfw3.h>
 
 int main(void)
 {
 // opengl version set
 glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
 glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
 glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
 glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 
 GLFWwindow* window;
 
if (!glfwInit())
return -1;

window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
if (!window)
{
    glfwTerminate();
    return -1;
}
glfwMakeContextCurrent(window);

float positions[6] = {
    -0.5f, -0.5,
    0.5f, -0.5f,
    0.0f,  0.5f
};

unsigned int buffer;
glGenBuffers(1, &buffer);
glBindBuffer(GL_ARRAY_BUFFER, buffer);
glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), positions, GL_STATIC_DRAW);
glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);
glEnableVertexAttribArray(0);

while (!glfwWindowShouldClose(window))
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(window);
    glfwPollEvents();
}

glfwTerminate();
return 0;
}
*/


