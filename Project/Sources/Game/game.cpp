#include "Application/application.h"
#include "Application/application_data.h"
#include "Engine/Render/world_renderer.h"
#include "Engine/Render/sprite.h"
#include "Engine/transform2d.h"
#include "Engine/input.h"
#include "Engine/time.h"

enum class ChessFigure
{
  King,
  Queen,
  Bishop,
  Rook,
  Knight,
  Pawn  
};
struct Figure
{
  Sprite sprite;
  Transform2D transform;
  ChessFigure type;
  int team;
};

static WorldRenderer worldRenderer;

static Figure king;
static mat3 cameraProjection;
static Transform2D cameraTransform;
void start()
{
  worldRenderer.init();
  Texture2D *figuresTex = 
  worldRenderer.add_texture(new Texture2D(project_resources_path("figures.png"), RGBA));
  
  constexpr int FigureCount = 6;
  float figureWidth = 1.f/FigureCount, figureHeight =  1.f/2.f;
  vec2 figureSize(figureWidth, figureHeight);

  king = Figure { Sprite(figuresTex, vec2(0, 0), figureSize), Transform2D(vec2(0), vec2(1)), ChessFigure::King, 0};

  worldRenderer.spriteShader = get_shader("standart_sprite");
  Resolution r = Application::get_context().get_screen_resolution();
  worldRenderer.resolution = vec2(r.x, r.y);
  float orthoScale = 1.f/100.f;//100 pixel per unit
  cameraProjection = mat3(0.f);
  cameraProjection[0][0] = 1.f / r.x;
  cameraProjection[1][1] = 1.f / r.y;
  cameraTransform = Transform2D(vec2(0), vec2(orthoScale));
}

void update()
{
  //logic
  worldRenderer.screenToWorld = cameraTransform.get_matrix();
  MousePosition mousePos = Input::get_mouse_position();
  king.transform.position = worldRenderer.screen_to_world(mousePos.x, mousePos.y);

  //render
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  mat3 viewProjection = cameraProjection * glm::inverse(cameraTransform.get_matrix());
  worldRenderer.spriteShader.use();
  glEnable(GL_BLEND); 
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
  king.sprite.texture->bind(worldRenderer.spriteShader, "sprite", 0);
  worldRenderer.spriteShader.set_mat3x3("trasformViewProjection", viewProjection * king.transform.get_matrix());
  worldRenderer.spriteShader.set_vec4("uvOffsetScale", vec4(king.sprite.offset, king.sprite.scale));
  worldRenderer.spriteShader.set_vec4("color", vec4(1.f));
  worldRenderer.spriteVao.render();

}

void imgui_update()
{

}

void before_exit_game()
{

}