#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>

static int resizeCallback(void *data, SDL_Event *event) //https://stackoverflow.com/questions/32294913/getting-contiunous-window-resize-event-in-sdl-2
{
	if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED)
	{
		SDL_Window *win = SDL_GetWindowFromID(event->window.windowID);
		if (win == (SDL_Window *)data)
		{
			printf("resizing.....\n");
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{
	SDL_Window *sdl_window = nullptr;

	unsigned int window_flags = SDL_WINDOW_OPENGL;

	if (SDL_Init(SDL_INIT_VIDEO))
	{
		printf("Failed to init SDL Video, error: %s", SDL_GetError());
		return -1;
	}

	int win_width = 640;
	int win_height = 480;
	enum class SCREENSIZE
	{
		is640x480,
		is1366x768,
		fullscreen
	} curr_screen_size = SCREENSIZE::is640x480,
	  last_non_fullscreen_size = SCREENSIZE::is640x480;

	// Create an application window with the following settings:
	sdl_window = SDL_CreateWindow(
		"(F11: fullscreen F10: winresize ESC: quit)", // window title
		SDL_WINDOWPOS_UNDEFINED,					  // initial x position
		SDL_WINDOWPOS_UNDEFINED,					  // initial y position
		win_width,									  // width, in pixels
		win_height,									  // height, in pixels
		window_flags								  // flags - see below
	);

	// Check that the window was successfully created
	if (sdl_window == nullptr)
	{
		printf("Could not create window: %s", SDL_GetError());
		return -1;
	}

	SDL_AddEventWatch(resizeCallback, sdl_window);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GLContext Context = SDL_GL_CreateContext(sdl_window);

	glClearColor(1.f, 0.f, 1.f, 0.f); //pink background
	glViewport(0, 0, win_width, win_height);

	bool isRunning = true;
	SDL_Event sdl_event;
	while (isRunning)
	{
		// 1. check events
		while (SDL_PollEvent(&sdl_event) != 0)
		{
			if (sdl_event.type == SDL_QUIT)
			{
				isRunning = false;
			}
			else if (sdl_event.type == SDL_KEYDOWN)
			{
				switch (sdl_event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					isRunning = false;
					break;
				case SDLK_F11:
					if (curr_screen_size != SCREENSIZE::fullscreen) // then set it to fullscreen and save prev state
					{
						last_non_fullscreen_size = curr_screen_size;
						curr_screen_size = SCREENSIZE::fullscreen;
						SDL_SetWindowFullscreen(sdl_window, window_flags | SDL_WINDOW_FULLSCREEN_DESKTOP);
					}
					else // is currently fullscreen, set it back to the prev state
					{
						curr_screen_size = last_non_fullscreen_size;
						SDL_SetWindowFullscreen(sdl_window, window_flags);
					}
					SDL_GetWindowSize(sdl_window, &win_width, &win_height);
					glViewport(0, 0, win_width, win_height);
					break;
				case SDLK_F10: // toggle screensizes, does nothing if fullscreen
					switch (curr_screen_size)
					{
					case SCREENSIZE::fullscreen:
						break;
					case SCREENSIZE::is640x480:
						curr_screen_size = SCREENSIZE::is1366x768;
						SDL_SetWindowSize(sdl_window, 1366, 768);
						SDL_GetWindowSize(sdl_window, &win_width, &win_height);
						glViewport(0, 0, win_width, win_height);
						break;
					case SCREENSIZE::is1366x768:
						curr_screen_size = SCREENSIZE::is640x480;
						SDL_SetWindowSize(sdl_window, 640, 480);
						SDL_GetWindowSize(sdl_window, &win_width, &win_height);
						glViewport(0, 0, win_width, win_height);
						break;
					}

					break;
				}
			}

			// 2. update screen

			// clear screen
			glClear(GL_COLOR_BUFFER_BIT);

			// add new updates
			// your updated stuff to render here (future todo)

			// swap to new updated screen to render
			SDL_GL_SwapWindow(sdl_window);
		}
	}

	// clean up
	SDL_DestroyWindow(sdl_window);
	SDL_Quit();

	return 0;
}
