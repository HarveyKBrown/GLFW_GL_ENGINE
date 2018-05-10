#include "EventManager.h"
#include <iostream>

/* EventCode -> EventLambda */
std::map<int, std::function<void()>> eventMap;
bool keys[512];

//controller will resolve actionNames from keys using a master key list
void EventManager::handleEvents(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) keys[key] = true;
	if (action == GLFW_RELEASE) keys[key] = false;
	//if (eventMap[key] && keys[key]) eventMap[key]();
}

void EventManager::runEvents()
{
	for (auto const& i : eventMap)
		if (keys[i.first]) i.second();
}

void EventManager::registerEvent(int eventCode, std::function<void()> f)
{
	eventMap[eventCode] = f;
}
