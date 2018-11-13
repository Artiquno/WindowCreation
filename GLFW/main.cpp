#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "src/definitions.h"
#include "src/window/window.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

glm::mat4 view;
glm::mat4 projection;

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

// Load texture for the square
void loadTexture(const char *path, unsigned int texture);

// Creates a spaceship and sends it to
// invade the USA ulimately failing and
// not even bothering to look at the other
// parts of the world
void showHelp();

void loadTexture(const char *path, unsigned int texture)
{
	// Load texture
	int width;
	int height;
	int nrChannels;
	unsigned short *texData = stbi_load_16(path, &width, &height, &nrChannels, 0);
	if (!texData)
	{
		std::cerr << "Could not open image " << path << std::endl;
		return;
	}
	
	glBindTexture(GL_TEXTURE_2D, texture);

	// Set parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Default is repeat
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture data and create mipmaps
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_SHORT, texData);
	glGenerateMipmap(GL_TEXTURE_2D);
	// Texture is free! Master gave pointer to Texture!
	stbi_image_free(texData);
}

void resizeCallback(GLFWwindow *window, int width, int height)
{
	// Otherwise we get 0/0 for the perspective ratio
	// Or 0/height but i don't want a 0 width window...
	if (!(width && height))
	{
		return;
	}
	options->dimensions->width = width;
	options->dimensions->height = height;

	projection = glm::perspective(45.0f, options->dimensions->width / (float)options->dimensions->height, 0.1f, 100.0f);

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
		/*case GLFW_KEY_W:
			changeMode(window);
			break;*/
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
			break;
		case GLFW_KEY_W:
		case GLFW_KEY_A:
		case GLFW_KEY_S:
		case GLFW_KEY_D:
		case GLFW_KEY_E:
		case GLFW_KEY_Q:
			break;
		default:
			const char *key_name = glfwGetKeyName(key, scancode);
			std::cout << key << ": ";
			if (key_name)
			{
				std::cout << key_name;
			}
			else
			{
				std::cout << scancode;
			}
			std::cout << std::endl;
			break;
		}
	}
}

unsigned int texture;
void dropCallback(GLFWwindow *window, int count, const char **paths)
{
	//int width;
	//int height;
	//int bpp;
	//unsigned char *pixels = stbi_load(paths[0], &width, &height, &bpp, 4);	// There has to be a way for a non-static 4?

	//GLFWimage *customCursorImage = new GLFWimage;
	//customCursorImage->height = height;
	//customCursorImage->width = width;
	//customCursorImage->pixels = pixels;

	//GLFWcursor *cursor = glfwCreateCursor(customCursorImage, 0, 0);
	//glfwSetCursor(window, cursor);
	//stbi_image_free(pixels);

	loadTexture(paths[0], texture);

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
			exit(ExitStatus::Normal);
			break;
		default:
			std::cerr << "Flag " << flag << " is not a valid flag" << std::endl;
			showHelp();
			exit(ExitStatus::InvalidFlag);
		}
		++flag;	// Move to the next character
	}
}

struct Mesh
{
	float *verts;
	unsigned int *indices;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;
};

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
	// All my images were upside down
	stbi_set_flip_vertically_on_load(1);
	parseArguments(argc, argv);

	Window::Window windowClass("Window");
	windowClass.getInputManager()->registerKeyCallback(keyCallback);
	GLFWwindow *window = windowClass.getWindow();

	// ToDo: Allow more than one handler on the user pointer
	// Create a manager containing all other managers and use that?
	//glfwSetWindowUserPointer(window, NULL);

	glewInit();

	glfwSetDropCallback(window, dropCallback);
	glfwSetWindowSizeCallback(window, resizeCallback);

	// TODO: Maybe organize the code a bit? No? Ok...
	int fileError;
	// Read vertex shader
	FILE *vertexShaderFile = NULL;
	fileError = fopen_s(&vertexShaderFile, "shaders/vertex_shader.glsl", "r");
	if (fileError)
	{
		std::cerr << "Failed to open file " << fileError << std::endl;
		exit(ExitStatus::FileOpenError);
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
		exit(ExitStatus::FileReadError);
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
		exit(ExitStatus::VertexShaderError);
	}

	// Read fragment shader file
	FILE *fFragmentShader;
	fileError = fopen_s(&fFragmentShader, "shaders/fragment_shader.glsl", "r");
	if (fileError)
	{
		std::cerr << "Failed to open file: " << fileError << std::endl;
		exit(ExitStatus::FileOpenError);
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
		exit(ExitStatus::FileReadError);
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
		exit(ExitStatus::FragmentShaderError);
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
		exit(ExitStatus::ShaderProgramLinkError);
	}

	// Cleanup
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Create vertex objects
	float verts[] = {
		//     Position         Colors        Tex Coords
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
	};
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2
	};

	glGenTextures(1, &texture);

	loadTexture("container.jpg", texture);

	// Initialize VAO
	unsigned int vao;
	glGenVertexArrays(1, &vao);

	// Yes yes, organize, make a class, blah blah
	// ToDo: Find a way to set UV for each face
	float cubeVerts[] = {
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	};

	unsigned int cubeIndices[] = {
		0, 3, 1,
		1, 3, 2,

		0, 1, 5,
		0, 5, 4,

		1, 6, 5,
		1, 2, 6,

		2, 3, 7,
		2, 7, 6,

		0, 7, 3,
		0, 4, 7,

		4, 5, 6,
		4, 6, 7
	};

	unsigned int cubeVao;
	glGenVertexArrays(1, &cubeVao);

	unsigned int cubeVbo;
	glGenBuffers(1, &cubeVbo);

	unsigned int cubeEbo;
	glGenBuffers(1, &cubeEbo);

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	// Pass data to ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Pass vertex attributes
	//int vertCount = sizeof(verts) / 8;
	int stride = 8 * sizeof(float);
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
	// Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	// Tex coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);

	// Pass the cube data
	glBindVertexArray(cubeVao);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);

	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);

	// Wireframe!
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Seems to limit it at (roughly) 60fps
	glfwSwapInterval(1);

	GLint colorLoc = glGetUniformLocation(shaderProgram, "color");

	GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
	GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
	GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

	glEnable(GL_PROGRAM_POINT_SIZE);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	// Moving the camera 3.0f towards +Z
	// The best way to move a spaceship is
	// by moving the whole universe around it instead
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	projection = glm::perspective(45.0f, (float)options->dimensions->width / (float)options->dimensions->height, 0.1f, 100.0f);

	glfwSetTime(0.0);
	float lastTime = 0;
	while (!glfwWindowShouldClose(window))
	{
		float deltaTime = glfwGetTime() - lastTime;
		lastTime = glfwGetTime();

		float frameRate = 1.0f / deltaTime;	// Is this accurate?
		//std::cout << frameRate << "fps" << std::endl;

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 transform;
		transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, -1.0f));
		transform = glm::rotate(transform, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.0f));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));
		GLfloat col[] = { 1.0f, 1.0f, 1.0f };
		glUniform3fv(colorLoc, 1, col);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glBindVertexArray(vao);
		glBindTexture(GL_TEXTURE_2D, texture);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

		glm::mat4 trans;
		trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
		trans = glm::rotate(trans, -(float)((glfwGetTime() * 1) + glm::radians(0.0)), glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 1.0f));
		//trans = glm::translate(trans, glm::vec3(sin(glfwGetTime()) / 0.75f, 0.0f, 0.0f));
		GLfloat col2[] = { 1.0f, 0.0f, 0.0f };
		glUniform3fv(colorLoc, 1, col2);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(trans));
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

		GLfloat col3[] = { 0.0f, 1.0f, 0.0f };
		glUniform3fv(colorLoc, 1, col3);
		glDrawElements(GL_POINTS, sizeof(indices), GL_UNSIGNED_INT, 0);

		//glBindVertexArray(0);	// Probably when you want to draw more than one object

		glBindVertexArray(cubeVao);

		glm::mat4 transCube;
		transCube = glm::translate(transCube, glm::vec3(-0.5f, -0.5f, 0.0f));
		transCube = glm::scale(transCube, glm::vec3(0.5f, 0.5f, 0.5f));
		transCube = glm::rotate(transCube, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		transCube = glm::rotate(transCube, (float)glfwGetTime(), glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transCube));
		GLfloat cubeCol[] = { 0.0f, 1.0f, 1.0f };
		glUniform3fv(colorLoc, 1, cubeCol);

		glDrawElements(GL_TRIANGLES, sizeof(cubeIndices), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);	// Probably when you want to draw more than one object

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}