#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// If only there was a data structure that could
// make simple mappings from readable name to number...
#define EXIT_STATUS_NORMAL 0

#define EXIT_STATUS_INVALID_FLAG 1

#define EXIT_STATUS_VERTEX_SHADER_ERROR 2
#define EXIT_STATUS_FRAGMENT_SHADER_ERROR 3
#define EXIT_STATUS_SHADER_PROGRAM_LINK_ERROR 4

#define EXIT_STATUS_FAILED_WINDOW_CREATION 5

#define EXIT_STATUS_FILE_READ_ERROR 6
#define EXIT_STATUS_FILE_OPEN_ERROR 7

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

// On drop callback
void dropCallback(GLFWwindow *window, int count, const char **paths);

// Function to call when glfw encounters an error
void errorCallback(int error, const char *description);

// Callback for window resizing
void resizeCallback(GLFWwindow *window, int width, int height);

// Parse command line arguments and apply them
void parseArguments(int argc, char **argv);

// Parse a long flag
void longFlag(char **argv);

// Parse a short flag
void shortFlag(char **argv);

// Apply dimensions to the options
// Dimensions should be in the form WxH
void parseDimensions(char **argv);

// Creates a spaceship and sends it to
// invade the USA ulimately failing and
// not even bothering to look at the other
// parts of the world
void showHelp();

void resizeCallback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void showHelp()
{
	std::cout << "Hi! I'm helping!" << std::endl;
}

unsigned int currRes = 0;
int modeCount;
const GLFWvidmode *modes;
void changeMode(GLFWwindow *window)
{
	currRes = (currRes + 2) % modeCount;
	GLFWvidmode newMode = modes[currRes];

	// To fullscreen or not to fullscreen?
	GLFWmonitor *monitor = options->fullscreen ? glfwGetPrimaryMonitor() : NULL;
	glfwSetWindowMonitor(window, monitor, 100, 100, newMode.width, newMode.height, newMode.refreshRate);
}

void changeRefreshRate(GLFWwindow *window)
{

}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)	// Only on (probably) key up
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		case GLFW_KEY_V:
			std::cout << glfwGetClipboardString(window) << std::endl;
			break;
		case GLFW_KEY_C:
			glfwSetClipboardString(window, "Yaaay I'm a clipboard!");
			break;
		case GLFW_KEY_W:
			changeMode(window);
			break;
		case GLFW_KEY_R:
			changeRefreshRate(window);
			break;
		case GLFW_KEY_H:
			glfwHideWindow(window);
			break;
		case GLFW_KEY_M:
			if (mods & GLFW_MOD_SHIFT)
			{
				glfwMaximizeWindow(window);
			}
			else
			{
				glfwIconifyWindow(window);
			}
		default:
			// Do nothing
			std::cout << key << std::endl;
			break;
		}
	}
}

void dropCallback(GLFWwindow *window, int count, const char **paths)
{
	int width;
	int height;
	int bpp;
	unsigned char *pixels = stbi_load(paths[0], &width, &height, &bpp, 4);	// There has to be a way for a non-static 4?

	GLFWimage *customCursorImage = new GLFWimage;
	customCursorImage->height = height;
	customCursorImage->width = width;
	customCursorImage->pixels = pixels;

	GLFWcursor *cursor = glfwCreateCursor(customCursorImage, 0, 0);
	glfwSetCursor(window, cursor);

	for (int i = 0; i < count; ++i)
	{
		std::cout << paths[i] << std::endl;
	}
}

void errorCallback(int error, const char *description)
{
	std::cerr << "Error: " << description << std::endl;
}

void longFlag(char** argv)
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
			exit(EXIT_STATUS_NORMAL);
			break;
		default:
			std::cerr << "Flag " << flag << " is not a valid flag" << std::endl;
			showHelp();
			exit(EXIT_STATUS_INVALID_FLAG);
		}
		++flag;	// Move to the next character
	}
}

void parseArguments(int argc, char **argv)
{
	options->programName = argv++[0];	// Don't know why I need this but it doesn't hurt?
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
		++argv;	// Move to the next argument
	}

	// If no dimensions are provided set window to fullscreen
	if (options->dimensions == NULL)
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
	// Probably bad way to do it but it's simple and it works?
	char** next = new char*[1];
	const char *dims = strtok_s(*argv, "x", next);
	options->dimensions = new Dimensions;
	options->dimensions->width = (int)strtol(dims, NULL, 10);
	options->dimensions->height = (int)strtol(*next, NULL, 10);
}

int main(int argc, char** argv)
{
	char infoLog[512];

	parseArguments(argc, argv);
	glfwSetErrorCallback(errorCallback);

	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GL context" << std::endl;
	}

	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode *mode = glfwGetVideoMode(monitor);
	modes = glfwGetVideoModes(monitor, &modeCount);

	std::cout << "Available sizes:" << std::endl;
	for (int i = 0; i < modeCount; ++i)
	{
		std::cout << modes[i].width << "x" << modes[i].height << " " << modes[i].refreshRate << "Hz" << std::endl;
	}
	std::cout << std::endl;

	GLFWwindow *window;
	if (options->fullscreen)
	{
		window = glfwCreateWindow(mode->width, mode->height, "Window", monitor, NULL);
	}
	else
	{
		// I just love "Always on top"
		glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
		window = glfwCreateWindow(options->dimensions->width, options->dimensions->height, "Window", NULL, NULL);
		//glfwMaximizeWindow(window);
	}
	if (!window)
	{
		std::cerr << "Failed to create window" << std::endl;
		exit(EXIT_STATUS_FAILED_WINDOW_CREATION);
	}

	glfwMakeContextCurrent(window);
	glewInit();

	glfwSetKeyCallback(window, keyCallback);
	glfwSetDropCallback(window, dropCallback);
	glfwSetWindowSizeCallback(window, resizeCallback);

	// TODO: Maybe organize the code a bit? No? Ok...
	int fileError;
	// Read vertex shader
	FILE *vertexShaderFile = NULL;
	fileError = fopen_s(&vertexShaderFile, "vertex_shader.glsl", "r");
	if (fileError)
	{
		std::cerr << "Failed to open file " << fileError << std::endl;
		exit(EXIT_STATUS_FILE_OPEN_ERROR);
	}

	fseek(vertexShaderFile, 0, SEEK_END);
	int size = ftell(vertexShaderFile);
	fseek(vertexShaderFile, 0, SEEK_SET);

	char* vertexShaderSource = new char[size];
	fread(vertexShaderSource, sizeof(char), size, vertexShaderFile);
	fclose(vertexShaderFile);
	if (vertexShaderSource == NULL)
	{
		std::cerr << "Could not load vertex shader" << std::endl;
		exit(EXIT_STATUS_FILE_READ_ERROR);
	}

	// Create vertex shader program
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, &size);
	glCompileShader(vertexShader);
	free(vertexShaderSource);

	// Get compile info
	int success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
		std::cout << "Failed to compile vertex shader:" << std::endl << infoLog << std::endl;
		exit(EXIT_STATUS_VERTEX_SHADER_ERROR);
	}

	// Read fragment shader file
	FILE *fFragmentShader;
	fileError = fopen_s(&fFragmentShader, "fragment_shader.glsl", "r");
	if (fileError)
	{
		std::cerr << "Failed to open file: " << fileError << std::endl;
		exit(EXIT_STATUS_FILE_OPEN_ERROR);
	}

	fseek(fFragmentShader, 0, SEEK_END);
	int fragSize = ftell(fFragmentShader);
	fseek(fFragmentShader, 0, SEEK_SET);

	char* fragmentShaderSource = new char[fragSize];
	fread(fragmentShaderSource, sizeof(char), fragSize, fFragmentShader);
	fclose(fFragmentShader);
	if (fragmentShaderSource == NULL)
	{
		std::cerr << "Failed to read fragment shader from file" << std::endl;
		exit(EXIT_STATUS_FILE_READ_ERROR);
	}

	// Create and compile fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, &fragSize);
	glCompileShader(fragmentShader);
	free(fragmentShaderSource);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
		std::cout << "Failed to compile fragment shader:" << std::endl << infoLog << std::endl;
		exit(EXIT_STATUS_FRAGMENT_SHADER_ERROR);
	}

	// Create shader program
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check for errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		std::cerr << "Failed to link shader program:" << std::endl << infoLog << std::endl;
		exit(EXIT_STATUS_SHADER_PROGRAM_LINK_ERROR);
	}

	// Cleanup
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Create vertex objects
	float verts[] = {
		//     Position         Colors
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.5f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.5f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	// Initialize VAO
	unsigned int vao;
	glGenVertexArrays(1, &vao);

	// Initialize VBO
	unsigned int vbo;
	glGenBuffers(1, &vbo);

	// Initialize EBO
	unsigned int ebo;
	glGenBuffers(1, &ebo);

	// Use this vao
	// I think the next two buffers get bound to this vao
	// I should make sure whether this is what this means
	glBindVertexArray(vao);

	// Pass data to vbo
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);

	// Pass data to ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Pass vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);

	// Wireframe!
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		float offset = (float)sin(glfwGetTime()) / 2.0f + 0.5f;
		for (int i = 3; i < sizeof(verts) / sizeof(float); ++i)
		{
			if (i % 6 != 0)
			{
				verts[i] = offset;
			}
			// Skip the position data
			else
			{
				i += 2;	// One index is skipped on the loop part
			}
		}
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		// Update vertex data (colors) for the "animation"
		glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);

		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);	// Probably when you want to draw more than one object

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}