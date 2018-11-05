#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Function to call when glfw encounters an error
void errorCallback(int error, const char* description);

void errorCallback(int error, const char* description)
{
	std::cerr << "Error: " << description << std::endl;
}

int main(int argc, char** argv)
{
	glfwSetErrorCallback(errorCallback);

	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GL context" << std::endl;
	}


	int count;
	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode *mode = glfwGetVideoMode(monitor);
	const GLFWvidmode *modes = glfwGetVideoModes(monitor, &count);

	std::cout << "Available sizes:" << std::endl;
	for (int i = 0; i < count; ++i)
	{
		std::cout << modes[i].width << "x" << modes[i].height << std::endl;
	}
	std::cout << std::endl;

	int height;
	int width;
	glfwGetMonitorPhysicalSize(monitor, &width, &height);

	std::cout << "Physical size: " << width << "mm x" << height << "mm" << std::endl;

	//GLFWwindow *window = glfwCreateWindow(mode->width, mode->height, "Window", monitor, NULL);
	GLFWwindow *window = glfwCreateWindow(640, 480, "Window", NULL, NULL);
	if (!window)
	{
		std::cerr << "Failed to create window" << std::endl;
	}

	glfwMakeContextCurrent(window);
	glewInit();

	glfwTerminate();
	return 0;
}