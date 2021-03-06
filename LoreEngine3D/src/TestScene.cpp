#include "TestScene.h"
#include "graphics\Graphics.h"
#include "utils\TextureLoader.h"
#include "entity\EntityDatabase.h"
#include "entity\components\RenderableComponent.h"
#include "entity\components\TextureComponent.h"
#include <GLFW\glfw3.h>

#define LOG_TICK 1

TestScene::TestScene()
{
	_camera = new Camera(vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 0.0f, 0.0f), CAMERA_PERSPECTIVE, 640.0f, 480.0f);
	_colorShader = new Shader("res/shaders/default.vs", "res/shaders/default.fs");
	_renderer = new BasicRenderer(*_colorShader, *_camera);

	_layer = new SceneLayer(*_renderer);

	Mesh* _cubeMesh = loadMesh("res/meshes/bunny.obj");	// DOES NOT GET DELETED!S

	Renderable* cube = new Renderable(*_cubeMesh, mat4f::Translation(0.0f, 0.0f, -3.0f));
	Renderable* cube2 = new Renderable(*_cubeMesh, mat4f::Translation(0.0f, 0.0f, 0.0f));
	Renderable* cube3 = new Renderable(*_cubeMesh, mat4f::Translation(-2.0f, -2.0f, 0.0f));
	Renderable* cube4 = new Renderable(*_cubeMesh, mat4f::Translation(-2.0f, -2.0f, -3.0f));

	Texture* texture = createTexture("crate.png");
	texture->bind();

	cubeEntity = new Entity();
	cubeEntity->add(*(new RenderableComponent(*cube)));
	cubeEntity->add(*(new TextureComponent(*texture)));

	Entity* cubeEntity2 = new Entity();
	cubeEntity2->add(*(new RenderableComponent(*cube)));
	cubeEntity2->add(*(new TextureComponent(*texture)));

	Entity* cubeEntity3 = new Entity();
	Entity* cubeEntity4 = new Entity();

	_layer->add(*cubeEntity);
	//_layer->add(*cubeEntity2);
	//_layer->add(*cubeEntity3);
	//_layer->add(*cubeEntity4);
}

TestScene::~TestScene()
{
	delete _camera;
	delete _colorShader;
	delete _renderer;
	delete _layer;
}

void resizeCallback(Window& window, int width, int height)
{
	std::cout << "RESIZE" << std::endl;
}

void TestScene::onLoad(Game& game)
{
	game.getTickHandler().setTPSLimit(10.0);
	_camera->resize(1920.0f, 1080.0f);
	//game.getGraphics().setDisplay(WINDOWED, 1920, 1080);
	game.getActiveWindow().setResizeCallback(resizeCallback);
}

void TestScene::onUnload(Game& game)
{

}

void TestScene::onTick(Game& game, double delta)
{
#if LOG_TICK
	printf("TICK:\n\tFPS:%d\n\tDELTA:%f\n\tMS:%f\n", 
		game.getTickHandler().getFPS(), delta, 
		game.getTickHandler().getRenderTime());
#endif
}

static float lastX = 0;
static float lastY = 0;
static float currentX = 0;
static float currentY = 0;

void TestScene::onUpdate(Game& game, double delta)
{
	lastX = currentX;
	lastY = currentY;
	currentX = (float)Input::getMouseX();
	currentY = (float)Input::getMoustY();

	float xOffset = lastX - currentX;
	float yOffset = lastY - currentY;

	float sensitivity = 0.2f;

	float cameraSpeed = (float)delta * 1.2f;

	if (Input::isMouseCaptured()) _camera->rotate(vec3f(-yOffset * sensitivity, -xOffset * sensitivity, 0), 1.0f);

	// Caputre Mouse
	if (Input::isButtonPressed(GLFW_MOUSE_BUTTON_1)) { Input::captureMouse(); }
	if (Input::isKeyPressed(GLFW_KEY_ESCAPE)) { Input::releaseMouse(); }

	// Movement
	if (Input::isKeyPressed(GLFW_KEY_W)) { _camera->move(_camera->getForward(), cameraSpeed); }
	if (Input::isKeyPressed(GLFW_KEY_S)) { _camera->move(_camera->getForward(), -cameraSpeed); }
	if (Input::isKeyPressed(GLFW_KEY_A)) { _camera->move(_camera->getRight(), -cameraSpeed); }
	if (Input::isKeyPressed(GLFW_KEY_D)) { _camera->move(_camera->getRight(), cameraSpeed); }

	// Movement 2
	if (Input::isKeyPressed(GLFW_KEY_UP)) { _camera->move(vec3f(0, 1, 0), cameraSpeed); }
	if (Input::isKeyPressed(GLFW_KEY_DOWN)) { _camera->move(vec3f(0, -1, 0), cameraSpeed); }
	if (Input::isKeyPressed(GLFW_KEY_LEFT)) { _camera->move(vec3f(-1, 0, 0), cameraSpeed);  }
	if (Input::isKeyPressed(GLFW_KEY_RIGHT)) { _camera->move(vec3f(1, 0, 0), cameraSpeed); }
	if (Input::isKeyPressed(GLFW_KEY_G)) { _camera->move(vec3f(0, 0, 1), cameraSpeed); }
	if (Input::isKeyPressed(GLFW_KEY_H)) { _camera->move(vec3f(0, 0, -1), cameraSpeed); }

}

void TestScene::onRender()
{
	_layer->render();
}