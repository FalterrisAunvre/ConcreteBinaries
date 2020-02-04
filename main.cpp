#include <SDL.h>
#include <SDL_syswm.h>

#include <glad/glad.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGui/imgui_impl_sdl.h"

int main(int argc, char *argv[])
{
	/*
		Per the ImGui example:
			"(Some versions of SDL before <2.0.10 appears to 
			 have performance/stalling issues on a minority 
			 of Windows systems, depending on whether 
			 SDL_INIT_GAMECONTROLLER is enabled or disabled.
			 Updating to latest version of SDL is recommended!"
	*/
	uint32_t SDLFlags = SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER;

	// Initialize SDL. A non-zero return from SDL_Init() means initialization has failed.
	if (SDL_Init(SDLFlags) != 0)
		return -1;

	// Uncomment for SDL to be able to respond to window events sent by the system.
	//SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);

	// Set up OpenGL 3.3 state.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	// Set the state to load OpenGL Core profile version 3.3.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// Set miscellaneous OpenGL context options.
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	// Create an SDL window with graphics context.
	const char *SDLWindowTitle = "SDL2 + OpenGL 3.3 Core + ImGui";
	SDL_WindowFlags SDLWindowFlags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Window *SDLWindow = SDL_CreateWindow(SDLWindowTitle, 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		900, 570, 
		SDLWindowFlags);
	// Create the OpenGL context within the window.
	SDL_GLContext SDLOpenGLContext = SDL_GL_CreateContext(SDLWindow);
	SDL_GL_MakeCurrent(SDLWindow, SDLOpenGLContext);
	SDL_GL_SetSwapInterval(1);	// Enables vsync.

	// Use GLAD to load OpenGL functions.
	// A return of zero from gladLoadGL() means initialization has failed.
	if (gladLoadGL() == 0)
		return -1;

	// Set up ImGui context.
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &IO = ImGui::GetIO();
	// Setup style.
	ImGui::StyleColorsDark();
	// Setup platform/renderer bindings.
	ImGui_ImplSDL2_InitForOpenGL(SDLWindow, SDLOpenGLContext);
	ImGui_ImplOpenGL3_Init("#version 330");


	// The window state is set up at this point.


	bool done = false;
	while (!done)
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL2_ProcessEvent(&event);

			if (event.type == SDL_QUIT)
			{
				done = true;
			}
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(SDLWindow))
			{
				done = true;
			}
		}

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(SDLWindow);
		ImGui::NewFrame();

		ImGui::ShowDemoWindow();

		// Rendering
		ImGui::Render();	// Just generates the draw paths
		glViewport(0, 0, (int)IO.DisplaySize.x, (int)IO.DisplaySize.y);
		glClearColor(0.45f, 0.55f, 0.60f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Custom OpenGL rendering would go here.

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(SDLWindow);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(SDLOpenGLContext);
	SDL_DestroyWindow(SDLWindow);
	SDL_Quit();

	return 0;
}