#include <GLES2/gl2.h>  // OpenGL ES 2.0 library
#include <glad/glad.h>  // glad library (OpenGL loader)
#include <SDL2/SDL.h>
#include <cstdio>
#include <switch.h>

int main(int argc, char* argv[])
{
	socketInitializeDefault();              // Initialize sockets
	nxlinkStdio();

	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	SDL_Window* window = SDL_CreateWindow("test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
	SDL_GLContext glContext = SDL_GL_CreateContext(window);

	if (!gladLoadGL())
		printf("Glad not initialized properly");
	else
		printf("Successfully initialized GLAD");




	if (glContext == NULL)
	{
		printf("SDL_Init failed: %s\n", SDL_GetError());
	}
	else
		printf("Context created with OpenGL version  (%s)\n", glGetString(GL_VERSION));


	GLuint id;
	glGenBuffers(1, &id);
	printf("Buffer: %d\n", id);

	socketExit();                           // Cleanup

	return 0;

}