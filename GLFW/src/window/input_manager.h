#pragma once
#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

namespace Window
{

	typedef void(*KeyCallback)(GLFWwindow *window);
	typedef void(*MouseCallback)(GLFWwindow *window, double x, double y);
	// Will possibly make this an interface later to allow mouse events and other stuff
	class InputManager
	{
	public:
		// Used for mouse movement
		double lastX;
		double lastY;

		// ToDo: Remove overloads or implement them
		InputManager();
		// Return all key/mouse handlers
		// Is this needed? Maybe should be removed?
		std::vector<KeyCallback> getKeyCallbacks() { return keyCallbacks; }
		std::vector<MouseCallback> getMouseCallbacks() { return mouseCallbacks; }
		// Add new handler for key input
		void registerKeyCallback(KeyCallback callback);
		void registerMouseCallback(MouseCallback callback);
		// Clear key handlers leaving only the default
		void resetKeyCallbacks();
		void resetMouseCallbacks();

		// Call all key handlers
		void processKeyInput(GLFWwindow *window);
		static void processMouseInput(GLFWwindow *window, double x, double y);
	private:
		std::vector<KeyCallback> keyCallbacks;
		std::vector<MouseCallback> mouseCallbacks;

		// Do some non-overwritable stuff
		static void defaultKeyCallback(GLFWwindow *window);
		static void defaultMouseCallback(GLFWwindow *window, double x, double y);
	};

}

#endif // !INPUT_MANAGER_H
