#include "Engine.h"

Engine *engine = NULL;

int main ( int argc, char* args[] )
{
	engine = new Engine();
	engine->init("GLFW Engine Example", 500, 500);
	engine->initShaders();

	while (engine->Running())
	{
		engine->calculateDeltaTime();
		engine->update();
		engine->render();
	}
}
