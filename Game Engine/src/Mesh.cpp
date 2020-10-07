//
//  Mesh.cpp
//  Game Engine
//
//  Created by Aniruddha Prithul on 7/31/17.
//  Copyright © 2017 Aniruddha Prithul. All rights reserved.
//

#include "Mesh.hpp"
#include <iostream>

namespace PrEngine {
    
    Mesh::Mesh()
    {
        
    }
    
    Mesh::~Mesh()
    {
        delete [] vertices;
        delete [] vertex_indices;
    }
    
    void Mesh::set_vertices(Vertex* _vertices, Int_32 vertex_count)
    {
        this->vertex_count = vertex_count;
        vertices_array_size = this->vertex_count*sizeof(Vertex);
        std::cout<<"SIZE: "<<vertices_array_size<<std::endl;
        std::cout<<"Count: "<<vertex_count<<std::endl;
        this->vertices = new Vertex[vertices_array_size];
        std::copy(_vertices,_vertices+vertex_count, this->vertices);
    }

    void Mesh::set_indices(GLuint* indices, Int_32 count)
    {
        this->vertex_indices = new GLuint[count];
        std::copy(indices,indices+count,this->vertex_indices);
        index_count = count;
        indices_array_size = index_count * sizeof(GLuint);
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
 
 Int_32 main(void)
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

Float_32 positions[6] = {
    -0.5f, -0.5,
    0.5f, -0.5f,
    0.0f,  0.5f
};

unsigned Int_32 buffer;
glGenBuffers(1, &buffer);
glBindBuffer(GL_ARRAY_BUFFER, buffer);
glBufferData(GL_ARRAY_BUFFER, 6*sizeof(Float_32), positions, GL_STATIC_DRAW);
glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Float_32)*2, 0);
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


