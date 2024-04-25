#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstddef>
#include <gtx/quaternion.hpp>

#define GL_FIND_ERROR() \
    do { \
        GLenum err; \
        while((err = glGetError()) != GL_NO_ERROR) { \
            fprintf(stderr, "OpenGL error %d in file %s at line %d\n", err, __FILE__, __LINE__); \
        } \
    } while(0)

GLFWwindow* init(int width, int height, std::string name);

extern GLFWwindow* MainWindow;
extern bool firstMouse;
extern bool wireFrame;

class Shader
{
public:
	Shader(const std::string VertexShader, const std::string FragmentShader);
	void Use();
	void Update();
	unsigned int ID;
private:
	unsigned int CompileShader(unsigned int type, const std::string source);
	std::string SourceLocation;
	friend Shader CreateShaderFromFile(std::string FilePath);
	friend class Mesh;
	double lastToggleTime = 0.0;
	const double toggleCooldown = 0.5;
};

Shader CreateShaderFromFile(std::string FilePath);//creates shader from a file

//for pitch and yaw movement
extern float lastX, lastY;

class Camera
{
public:
	glm::vec3 cameraPos;
	float pitch=0.0;
	float yaw = 0.0;
	Camera(glm::vec3 Position);
	void Update();//define later
	void setMainCameraFalse();
	void setMainCameraTrue();
	void Update(Shader shader);
	~Camera();
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
};

extern unsigned int VAO, VBO, EBO;
struct Vertex 
{
public:
	Vertex(glm::vec3 position, glm::vec2 textCords, glm::vec3 normals);
	glm::vec3 position;
	glm::vec2 texCoords;
	glm::vec3 normals;
};
class Mesh 
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Shader& shader);
	Shader *shader;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	void Draw();
};

class Texture
{
private:
	unsigned int ID;
	std::string filePath;
	unsigned char* localBuffer;
	int width, height, BPP;
public:
	Texture(const std::string filePath);
	~Texture();

	void Bind(unsigned int slot = 0);
	void UnBind();

	int GetWidth();
	int GetHeight();
};

extern std::vector<Camera*> MainCamera;//Do not append to this list

void keyCallBack(GLFWwindow *window, float DeltaTime);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);