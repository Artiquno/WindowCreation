#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "src/definitions.h"
#include "src/window/window.h"
#include "src/utils/command_parser.h"
#include "src/shader/shader_program.h"

#include "src/model/mesh/vertex.h"
#include "src/model/mesh/mesh.h"
#include "src/model/model.h"

#include "src/utils/image.h"

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

// Load texture for the square
void loadTexture(const char *path, unsigned int texture);

void loadTexture(const char *path, unsigned int texture)
{
	// Load texture
	Utils::Image image(path);
	stbi_us *pixels = image.getPixels();
	
	glBindTexture(GL_TEXTURE_2D, texture);

	// Set parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Default is repeat
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture data and create mipmaps
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getWidth(), image.getHeight(), 0, GL_RGB, GL_UNSIGNED_SHORT, pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	// Texture is free! Master gave pointer to Texture!
	//stbi_image_free(pixels);
}

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

int main(int argc, char** argv)
{
	// All my images were upside down
	stbi_set_flip_vertically_on_load(1);

	parser.parse(argc, argv);
	Utils::CommandParser::Options options = parser.getOptions();

	Window::Window windowClass("Window", false, options.dimensions->width, options.dimensions->height);
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

	glGenTextures(1, &texture);

	unsigned int rafiki;
	glGenTextures(1, &rafiki);

	loadTexture("container.jpg", texture);
	loadTexture("rafiki.jpg", rafiki);

	// Create vertex objects
	float verts[] = {
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
	};

	float colors[] = {
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
	};

	float coords[] = {
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
	};

	int size = sizeof(verts) / (sizeof(float) * 3);
	std::vector<Model::Vertex> vertices;
	for (int i = 0; i < size; ++i)
	{
		Model::Vertex v;
		v.position = glm::vec3(verts[i * 3], verts[i * 3 + 1], verts[i * 3 + 2]);
		v.color = glm::vec4(colors[i * 4], colors[i * 4 + 1], colors[i * 4 + 2], colors[i * 4 + 3]);
		v.textureCoords = glm::vec2(coords[i * 2], coords[i * 2 + 1]);

		vertices.push_back(v);
	}
	std::vector<unsigned int> indices = {
		0, 3, 1,
		1, 3, 2
	};
	
	Model::Mesh plane(vertices, indices);

	Model::Model plane1Model("Plane 1", plane, program);
	plane1Model.addTexture("container.jpg");
	plane1Model.addTexture("rafiki.jpg");

	plane1Model.translate(glm::vec3(0.5f, -0.5f, -1.0f));
	plane1Model.rotate(glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	Model::Model plane2Model("Plane 2", plane, program);
	plane2Model.addTexture("container.jpg");
	plane2Model.addTexture("rafiki.jpg");

	plane2Model.translate(glm::vec3(-0.5f, 0.5f, 0.0f));

	// Yes yes, organize, make a class, blah blah
	// ToDo: Find a way to set UV for each face
	float cubeVerts[] = {
		 0.5f,  0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	float cubeCol[] = {
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.0f,
	};

	float cubeUV[] = {
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
	};

	std::vector<Model::Vertex> cubeVertices(8);
	for (int i = 0; i < cubeVertices.size(); ++i)
	{
		Model::Vertex v;
		v.position = glm::vec3(cubeVerts[i * 3], cubeVerts[i * 3 + 1], cubeVerts[i * 3 + 2]);
		v.color = glm::vec4(cubeCol[i * 4], cubeCol[i * 4 + 1], cubeCol[i * 4 + 2], cubeCol[i * 4 + 3]);
		v.textureCoords = glm::vec2(cubeUV[i * 2], cubeUV[i * 2 + 1]);
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

	Model::Mesh cube(cubeVertices, cubeIndices);
	Model::Model cubeModel("Cube", cube, program);
	cubeModel.addTexture("rafiki.jpg");
	cubeModel.addTexture("container.jpg");

	cubeModel.translate(glm::vec3(0.0f, 0.0f, 1.0f));
	//cubeModel.rotate(glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	Model::Model ground("Ground", plane, program);
	ground.addTexture("container.jpg");
	ground.translate(glm::vec3(0.0f, -2.0f, 0.0f));
	ground.rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	ground.scale(20.0f);

	std::vector<Model::Model *> models = {
		&ground,
		&plane1Model,
		&plane2Model,
		&cubeModel
	};

	Model::Vertex centerVertex;
	centerVertex.position = glm::vec3(0.0f, 0.0f, 0.0f);
	centerVertex.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	Model::Vertex xVertex;
	xVertex.position = glm::vec3(1.0f, 0.0f, 0.0f);
	xVertex.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	Model::Vertex yVertex;
	yVertex.position = glm::vec3(0.0f, 1.0f, 0.0f);
	yVertex.color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	Model::Vertex zVertex;
	zVertex.position = glm::vec3(0.0f, 0.0f, 1.0f);
	zVertex.color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	std::vector<Model::Vertex> centerVerts = { centerVertex, xVertex, yVertex, zVertex };
	std::vector<unsigned int> centerIndex = { 0, 1, 0, 2, 0, 3 };
	Model::Mesh center(centerVerts, centerIndex);

	// Moving the camera 3.0f towards +Z
	// The best way to move a spaceship is
	// by moving the whole universe around it instead
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -6.0f));
	view = glm::rotate(view, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::rotate(view, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	projection = glm::perspective(45.0f, (float)options.dimensions->width / (float)options.dimensions->height, 0.1f, 100.0f);

	program.use();
	program.setInt("tex1", 0);
	program.setInt("tex2", 1);

	float lastTime = 0;
	while (!glfwWindowShouldClose(window))
	{
		float deltaTime = glfwGetTime() - lastTime;
		lastTime = glfwGetTime();

		float frameRate = 1.0f / deltaTime;	// Is this accurate?
		//std::cout << frameRate << "fps" << std::endl;

		glm::vec3 camForward(view[0][2], view[1][2], view[2][2]);
		program.setMatrix4f("view", 1, GL_FALSE, view);
		program.setMatrix4f("projection", 1, GL_FALSE, projection);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program.use();

		glm::mat4 centerTransform;
		centerTransform = glm::scale(centerTransform, glm::vec3(2.0f));
		glBindVertexArray(center.getVao());

		program.setMatrix4f("model", 1, GL_FALSE, centerTransform);
		center.draw(GL_LINES);
		glBindVertexArray(0);

		plane1Model.rotate(deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));
		plane2Model.rotate(-deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
		cubeModel.rotate(deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));
		
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