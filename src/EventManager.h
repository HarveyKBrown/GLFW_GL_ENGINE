#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "Engine.h"
#include <map>
#include <functional>

class EventManager
{
	private:

	public:
		enum Code { quit, jump, up, down, left, right };

		static void handleEvents(GLFWwindow *window, int key, int scancode, int action, int mods);

		static void registerEvent(int eventCode, std::function<void()> f);
};

#endif
