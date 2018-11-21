#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "src/definitions.h"
#include "src/window/window.h"
#include "src/utils/command_parser.h"
#include "src/shader/shader_program.h"

#include "src/model/mesh/vertex.h"
#include "src/model/mesh/mesh.h"
#include "src/model/model.h"

#include "src/camera/camera.h"

#include "src/utils/image.h"
#include "src/utils/time.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

glm::mat4 view;
glm::mat4 projection;

Utils::CommandParser parser;

// Function to call on key input
// Moved to InputManager but i'm leaving this here
// to prove a point NYEH HEH HEH HEH!
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

// On drop callback
void dropCallback(GLFWwindow *window, int count, const char **paths);

// Callback for window resizing
void resizeCallback(GLFWwindow *window, int width, int height);

void resizeCallback(GLFWwindow *window, int width, int height)
{
	// Otherwise we get 0/0 for the perspective ratio
	// Or 0/height but i don't want a 0 width window...
	if (!(width && height))
	{
		return;
	}
	Utils::CommandParser::Options options = parser.getOptions();
	options.dimensions->width = width;
	options.dimensions->height = height;

	projection = glm::perspective(45.0f, options.dimensions->width / (float)options.dimensions->height, 0.1f, 100.0f);

	glViewport(0, 0, width, height);
}

unsigned int currRes = 0;
int modeCount;
const GLFWvidmode *modes;
void changeMode(GLFWwindow *window)
{
	currRes = (currRes + 2) % modeCount;
	GLFWvidmode newMode = modes[currRes];

	// To fullscreen or not to fullscreen?
	GLFWmonitor *monitor = Utils::CommandParser::getOptions().fullscreen ? glfwGetPrimaryMonitor() : NULL;
	glfwSetWindowMonitor(window, monitor, 100, 100, newMode.width, newMode.height, newMode.refreshRate);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		Window::Window *windowManager = static_cast<Window::Window *>(glfwGetWindowUserPointer(window));
		Camera::Camera *camera = windowManager->getCamera();
		float speed = 10.0f;
		float angularSpeed = 90.0f;

		if (key == GLFW_KEY_A)
		{
			camera->translate(glm::vec3(-1.0f, 0.0f, 0.0f) * Time::deltaTime() * speed);
		}
		else if (key == GLFW_KEY_D)
		{
			camera->translate(glm::vec3(1.0f, 0.0f, 0.0f) * Time::deltaTime() * speed);
		}

		if (key == GLFW_KEY_W)
		{
			camera->translate(glm::vec3(0.0f, 0.0f, -1.0f) * Time::deltaTime() * speed);
		}
		else if (key == GLFW_KEY_S)
		{
			camera->translate(glm::vec3(0.0f, 0.0f, 1.0f) * Time::deltaTime() * speed);
		}

		if (key == GLFW_KEY_E)
		{
			camera->translate(glm::vec3(0.0f, 1.0f, 0.0f) * Time::deltaTime() * speed);
		}
		else if (key == GLFW_KEY_Q)
		{
			camera->translate(glm::vec3(0.0f, -1.0f, 0.0f) * Time::deltaTime() * speed);
		}

		if (key == GLFW_KEY_LEFT)
		{
			camera->rotate(0.0f, -1.0f * Time::deltaTime() * angularSpeed);
		}
		else if (key == GLFW_KEY_RIGHT)
		{
			camera->rotate(0.0f, 1.0f * Time::deltaTime() * angularSpeed);
		}

		if (key == GLFW_KEY_UP)
		{
			camera->rotate(1.0f * Time::deltaTime() * angularSpeed, 0.0f);
		}
		else if (key == GLFW_KEY_DOWN)
		{
			camera->rotate(-1.0f * Time::deltaTime() * angularSpeed, 0.0f);
		}
	}
}

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

	//loadTexture(paths[0], texture);

	for (int i = 0; i < count; ++i)
	{
		std::cout << paths[i] << std::endl;
	}
}

int main(int argc, char** argv)
{
	// All my images were upside down
	stbi_set_flip_vertically_on_load(1);

	parser.parse(argc, argv);
	Utils::CommandParser::Options options = parser.getOptions();

	Window::Window windowClass("Window",
		Camera::Camera(options.dimensions->width, options.dimensions->height),
		false, options.dimensions->width, options.dimensions->height);
	Camera::Camera *camera = windowClass.getCamera();
	windowClass.getInputManager()->registerKeyCallback(keyCallback);
	GLFWwindow *window = windowClass.getWindow();

	// ToDo: Allow more than one handler on the user pointer
	// Create a manager containing all other managers and use that?
	//glfwSetWindowUserPointer(window, NULL);

	glfwSetDropCallback(window, dropCallback);
	glfwSetWindowSizeCallback(window, resizeCallback);

	// TODO: Maybe organize the code a bit? No? Ok...

	// Create shader program
	Shader::Program program("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
	Shader::Program axesProgram("shaders/axes_v_shader.glsl", "shaders/axes_f_shader.glsl");

	// Create vertex objects
	std::vector<glm::vec3> verts = {
		glm::vec3( 0.5f,  0.5f, 0.0f),
		glm::vec3( 0.5f, -0.5f, 0.0f),
		glm::vec3(-0.5f, -0.5f, 0.0f),
		glm::vec3(-0.5f,  0.5f, 0.0f),
	};

	std::vector<glm::vec4> colors = {
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
	};

	std::vector<glm::vec2> coords = {
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
	};

	std::vector<Model::Vertex> vertices;
	for (int i = 0; i < verts.size(); ++i)
	{
		Model::Vertex v;
		v.position = verts[i];
		v.color = colors[i];
		v.textureCoords = coords[i];

		vertices.push_back(v);
	}
	std::vector<unsigned int> indices = {
		0, 3, 1,
		1, 3, 2
	};
	
	Model::Mesh planeMesh(vertices, indices);

	Model::Model plane1("Plane 1", planeMesh, program);
	plane1.addTexture("container.jpg");
	plane1.addTexture("rafiki.jpg");

	plane1.translate(glm::vec3(0.5f, -0.5f, -1.0f));
	plane1.rotate(glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	Model::Model plane2("Plane 2", planeMesh, program);
	plane2.addTexture("container.jpg");
	plane2.addTexture("rafiki.jpg");

	plane2.translate(glm::vec3(-0.5f, 0.5f, 0.0f));

	// Yes yes, organize, make a class, blah blah
	// ToDo: Find a way to set UV for each face
	std::vector<glm::vec3> cubeVerts = {
		glm::vec3( 0.5f,  0.5f,  0.5f),
		glm::vec3( 0.5f, -0.5f,  0.5f),
		glm::vec3(-0.5f, -0.5f,  0.5f),
		glm::vec3(-0.5f,  0.5f,  0.5f),
		glm::vec3( 0.5f,  0.5f, -0.5f),
		glm::vec3( 0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(-0.5f,  0.5f, -0.5f),
	};

	std::vector<glm::vec4> cubeCol = {
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 0.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 0.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 0.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 0.0f),
	};

	std::vector<glm::vec2> cubeUV = {
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
	};

	std::vector<Model::Vertex> cubeVertices(8);
	for (int i = 0; i < cubeVertices.size(); ++i)
	{
		Model::Vertex v;
		v.position = cubeVerts[i];
		v.color = cubeCol[i];
		v.textureCoords = cubeUV[i];
		cubeVertices[i] = v;
	}
	std::vector<unsigned int> cubeIndices = {
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

	Model::Mesh cubeMesh(cubeVertices, cubeIndices);
	Model::Model cubeModel("Cube", cubeMesh, program);
	cubeModel.addTexture("rafiki.jpg");
	cubeModel.addTexture("container.jpg");

	cubeModel.translate(glm::vec3(0.0f, 0.0f, 1.0f));
	//cubeModel.rotate(glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	Model::Model ground("Ground", planeMesh, program);
	ground.addTexture("container.jpg");
	ground.translate(glm::vec3(0.0f, -2.0f, 0.0f));
	ground.rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	ground.scale(20.0f);

	std::vector<Model::Model *> models = {
		&ground,
		&plane1,
		&plane2,
		&cubeModel
	};

	std::vector<glm::vec3> cVerts = {
		glm::vec3(1.0f,  0.0f,  0.0f),
		glm::vec3(-1.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  1.0f,  0.0f),
		glm::vec3(0.0f, -1.0f,  0.0f),
		glm::vec3(0.0f,  0.0f,  1.0f),
		glm::vec3(0.0f,  0.0f, -1.0f)
	};
	std::vector<Model::Vertex> centerVerts;
	for (glm::vec3 v : cVerts)
	{
		Model::Vertex vv;
		vv.position = v;
		centerVerts.push_back(vv);
	}

	std::vector<unsigned int> centerIndex = { 0, 1, 2, 3, 4, 5 };
	Model::Mesh center(centerVerts, centerIndex);
	Model::Model axes("Axes", center, axesProgram);
	//axes.scale(1.0f);

	program.setInt("tex1", 0);
	program.setInt("tex2", 1);

	while (!glfwWindowShouldClose(window))
	{
		Time::update();
		std::cout << Time::frameRate() << std::endl;
		// Will be moved to the window class later
		windowClass.time = glfwGetTime();

		//camera->rotate(deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));

		glm::vec3 camForward = camera->getDirection();

		// These should probably be set on the camera code
		program.setMatrix4f("view", 1, GL_FALSE, camera->getViewMatrix());
		program.setMatrix4f("projection", 1, GL_FALSE, camera->getProjectionMatrix());

		axesProgram.setMatrix4f("view", 1, GL_FALSE, camera->getViewMatrix());
		axesProgram.setMatrix4f("projection", 1, GL_FALSE, camera->getProjectionMatrix());

		glUseProgram(0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		axes.drawRaw(GL_LINES);

		// N deg / sec?
		plane1.rotate(glm::radians(45.0f * Time::deltaTime()), glm::vec3(0.0f, 0.0f, 1.0f));
		plane2.rotate(glm::radians(-90.0f * Time::deltaTime()), glm::vec3(0.0f, 1.0f, 0.0f));
		cubeModel.translate(glm::vec3(Time::deltaTime(), sin(Time::getTime()) / 50.0f, 0.0f));
		cubeModel.rotate(glm::radians(30.0f * Time::deltaTime()), glm::vec3(0.0f, 0.0f, 1.0f));
		
		for (Model::Model *model : models)
		{
			model->draw();
			model->drawVerts();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}