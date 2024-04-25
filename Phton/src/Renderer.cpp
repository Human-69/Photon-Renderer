#include "Photon.h"

std::vector<Camera*> MainCamera;
GLFWwindow* MainWindow;

Vertex::Vertex(glm::vec3 position, glm::vec2 textCords, glm::vec3 normals)
{
	this->position = position;
	this->texCoords = textCords;
	this->normals = normals;
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Shader& shader)
{
	this->vertices = vertices;
	this->indices = indices;
	this->shader = &shader;

	for(int i=0; i<vertices.size();i++)
	{
		std::cout << this->vertices[i].position.x <<", " << this->vertices[i].position.y <<", "<< this->vertices[i].position.z << std::endl;
	}
	for (int i=0; i < indices.size(); i++)
	{
		std::cout << this->indices[i] << std::endl;
	}
}

void Mesh::Draw()
{
	shader->Update();

	glBindVertexArray(VAO);
	GL_FIND_ERROR();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
	GL_FIND_ERROR();

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	GL_FIND_ERROR();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));
	GL_FIND_ERROR();
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texCoords));
	GL_FIND_ERROR();
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, normals));
	GL_FIND_ERROR();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);
	GL_FIND_ERROR();

	GL_FIND_ERROR();

	shader->Use();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	GL_FIND_ERROR();
}

Camera::Camera(glm::vec3 Position)
{
	cameraPos = Position;
	glfwSetInputMode(MainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Camera::setMainCameraFalse()
{
	for(int i=MainCamera.size(); i>0; i--)
	{
		MainCamera.pop_back();
	}
}

void Camera::setMainCameraTrue()
{
	if(MainCamera.size()==0)
	{
		MainCamera.push_back(this);
	}
	else
	{
		std::cout << "Only one main camera can exist. Remove the old camera before adding a new one\n";
	}
}

Camera::~Camera()
{
	for (int i = 0; i < MainCamera.size(); i++)
	{
		if(MainCamera[i]==this)
		{
			MainCamera.erase(std::next(MainCamera.begin(), i));
		}
	}
}

void Camera::Update(Shader shader)
{
	shader.Use();

	glm::mat4 view = glm::mat4(1.0);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	int location = glGetUniformLocation(shader.ID, "view");
	
	if (location != -1)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(view));
	}
	else
		std::cout << "View does not exist in current shader";
	
}