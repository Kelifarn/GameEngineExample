#include "application_data.h"
#include "application.h"
#include "glad/glad.h"
#include "Engine/imgui/imgui_impl_opengl3.h"
#include "Engine/imgui/imgui_impl_sdl.h"
#include <SDL2/SDL.h>

void compile_shaders();

void start();
void update();
void imgui_update();
void before_exit_game();

Application::Application(const string &window_name, const string &project_path):
context(window_name), timer(),
projectPath(project_path),
projectResourcesPath(project_path + "/Resources"),
projectShaderPath(project_path + "/Shaders")
{
  application = this;
  compile_shaders();
}
void Application::start_application()
{
  start();
}
bool Application::sdl_event_handler()
{
  SDL_Event event;
  bool running = true;
  Input &input = Input::input();
  while (SDL_PollEvent(&event))
  {
    ImGui_ImplSDL2_ProcessEvent(&event);
    switch(event.type){
      case SDL_QUIT: running = false; break;
      
      case SDL_KEYDOWN: 
      
      
      case SDL_KEYUP: input.event_process(event.key, Time::time());

      if(event.key.keysym.sym == SDLK_ESCAPE)
        running = false;
        

      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP: input.event_process(event.button, Time::time()); break;

      case SDL_MOUSEMOTION: input.event_process(event.motion, Time::time()); break;

      case SDL_MOUSEWHEEL: input.event_process(event.wheel, Time::time()); break;
      
      case SDL_WINDOWEVENT: break;
    }
  }
  return running;
}
void Application::main_loop()
{
  bool running = true;
  while(running){
    timer.update();
		running = sdl_event_handler();
    if (running)
    {
      update();
      context.swap_buffer();
      
      context.start_imgui();
      imgui_update();
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
	}
}
void Application::exit()
{
  before_exit_game();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  SDL_Quit();
}
string project_resources_path(const string &path)
{
  return Application::instance().projectResourcesPath.string() + "/" + path;
}
string project_resources_path()
{
  return Application::instance().projectResourcesPath.string();
}

