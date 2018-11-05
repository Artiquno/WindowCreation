#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

struct Dimensions
{
	int width = 0;
	int height = 0;
};

// Command line argument options
struct Options
{
	char *programName;					// Duh
	bool fullscreen = false;			// Create window in fullscreen mode?
	Dimensions *dimensions = NULL;		// Window dimensions (if not fullscreen)
	bool showHelp = false;				// Print help and exit
} *options = new Options;

// Function to call on key input
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

// Function to call when glfw encounters an error
void errorCallback(int error, const char *description);

// Parse command line arguments and apply them
void parseArguments(int argc, char **argv);

// Parse a long flag
void longFlag(char **arg);

// Parse a short flag
void shortFlag(char **arg);

// Apply dimensions to the options
// Dimensions should be in the form WxH
void parseDimensions(char **argv);

// Creates a spaceship and sends it to
// invade the USA ulimately failing and
// not even bothering to look at the other
// parts of the world
void showHelp();

void showHelp()
{
	std::cout << "Hi! I'm helping!" << std::endl;
	exit(0);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void errorCallback(int error, const char *description)
{
	std::cerr << "Error: " << description << std::endl;
}

void longFlag(char** arg)
{
	std::cerr << "Long flags are not yet implemented." << std::endl;
}
void shortFlag(char** argv)
{
	char *flag = &(*argv)[1];
	while (*flag != '\0')
	{
		switch (*flag)
		{
		case 'f':	options->fullscreen = true; break;
		// Assumes the next argument is the dimensions
		case 'd':	parseDimensions(++argv); break;
		case 'h':
		case '?':
			showHelp();
			break;
		default:
			std::cerr << "Flag " << flag << " is not a valid flag" << std::endl;
			showHelp();
		}
		++flag;
	}
}

void parseArguments(int argc, char **argv)
{
	options->programName = argv++[0];
	while (argv[0] != NULL)
	{
		if (argv[0][0] == '-')
		{
			if (argv[0][1] == '-')
			{
				longFlag(argv);
			}
			else
			{
				shortFlag(argv);
			}
		}
		++argv;
	}

	if (!options->fullscreen && options->dimensions == NULL)
	{
		options->fullscreen = true;
	}
}

void parseDimensions(char **argv)
{
	if (options->dimensions != NULL)
	{
		std::cerr << "Window dimensions already set to " << options->dimensions->width << "x" << options->dimensions->height << std::endl;
		return;
	}
	char** next = new char*[1];
	const char *dims = strtok_s(*argv, "x", next);
	options->dimensions = new Dimensions;
	options->dimensions->width = (int)strtol(dims, NULL, 10);
	options->dimensions->height = (int)strtol(*next, NULL, 10);
}

int main(int argc, char** argv)
{
	parseArguments(argc, argv);
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

	std::cout << "Physical size: " << width << "mm x " << height << "mm" << std::endl;

	GLFWwindow *window;
	if (options->fullscreen)
	{
		window = glfwCreateWindow(mode->width, mode->height, "Window", monitor, NULL);
	}
	else
	{
		window = glfwCreateWindow(options->dimensions->width, options->dimensions->height, "Window", NULL, NULL);
	}
	if (!window)
	{
		std::cerr << "Failed to create window" << std::endl;
	}

	glfwMakeContextCurrent(window);
	glewInit();

	glfwSetKeyCallback(window, keyCallback);

	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
	}

	while (!glfwWindowShouldClose(window))
	{
		double time = glfwGetTime();

		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}