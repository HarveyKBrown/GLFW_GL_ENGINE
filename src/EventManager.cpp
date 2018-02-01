#include "EventManager.h"

/* EventCode -> EventLambda */
std::map<int, std::function<void()>> eventMap;

//controller will resolve actionNames from keys using a master key list
void EventManager::handleEvents(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		eventMap[GLFW_KEY_ESCAPE]();
	}
}

void EventManager::registerEvent(int eventCode, std::function<void()> f)
{
	eventMap[eventCode] = f;
}
