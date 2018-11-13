#pragma once
#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <GLFW/glfw3.h>
#include <vector>

namespace Window
{

	typedef void(*KeyCallback)(GLFWwindow *window, int key, int scancode, int action, int mods);
	// Will possibly make this an interface later to allow mouse events and other stuff
	class InputManager
	{
	public:
		// ToDo: Remove overloads or implement them
		InputManager();
		// Return all key handlers
		// Is this needed? Maybe should be removed?
		std::vector<KeyCallback> getKeyCallbacks();
		// Add new handler for key input
		void registerKeyCallback(KeyCallback callback);
		// Clear key handlers leaving only the default
		void resetKeyCallbacks();

		// Call all key handlers
		static void processKeyInput(GLFWwindow *window, int key, int scancode, int action, int mods);
	private:
		std::vector<KeyCallback> _keyCallbacks;

		// Do some non-overwritable stuff
		static void defaultKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
	};

}

#endif // !INPUT_MANAGER_H
