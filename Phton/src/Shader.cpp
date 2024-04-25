#include "Photon.h"

unsigned int Shader::CompileShader(unsigned int type, const std::string source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	GL_FIND_ERROR();

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout <<"Failed to compile shader\n" << message << std::endl;
	}

	return id;
}

Shader::Shader(const std::string VertexShader, const std::string FragmentShader) 
{
	unsigned int program = glCreateProgram();
	GL_FIND_ERROR();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, VertexShader);
	GL_FIND_ERROR();
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, FragmentShader);
	GL_FIND_ERROR();

	glAttachShader(program, vs);
	GL_FIND_ERROR();
	glAttachShader(program, fs);
	GL_FIND_ERROR();
	glLinkProgram(program);
	GL_FIND_ERROR();
	glValidateProgram(program);
	GL_FIND_ERROR();

	glDeleteShader(vs);
	GL_FIND_ERROR();
	glDeleteShader(fs);
	GL_FIND_ERROR();

	this->ID = program;
	GL_FIND_ERROR();
}

void Shader::Use()
{
	glUseProgram(this->ID);
}

void Shader::Update()
{
	if ((glfwGetKey(MainWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) && (glfwGetKey(MainWindow, GLFW_KEY_R) == GLFW_PRESS))
	{
		double currentTime = glfwGetTime();
		if (currentTime - this->lastToggleTime >= this->toggleCooldown) {
			std::cout << "Hot reloded\n";
			glDeleteProgram(this->ID);
			this->ID = CreateShaderFromFile(this->SourceLocation).ID;
			this->Use();


			glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1920 / (float)1080, 0.1f, 100.0f);
			int projectionLocation = glGetUniformLocation(this->ID, "projection");
			if(projectionLocation==-1)
			{
				std::cout << "The current shader does not contain Unifor projection\n";
			}
			else 
			{
				glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
			}
			GL_FIND_ERROR();

			lastToggleTime = currentTime;
		}
	}
}

Shader CreateShaderFromFile(std::string FilePath) 
{
	std::ifstream stream(FilePath);
	if(!stream.is_open())
	{
		std::cout << "Failed to open file\n";
		return Shader("", "");
	}
	std::string line;
	std::stringstream ss[2];

	enum ShaderType
	{
		NONE=-1, VERTEX=0, FRAGMENT=1
	};
	ShaderType type=NONE;
	while(std::getline(stream, line))
	{
		if(line.find("#shader")!=std::string::npos)
		{
			if(line.find("vertex") != std::string::npos)
			{
				type = VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	std::cout << ss[0].str()<<ss[1].str();
	Shader shader(ss[0].str(), ss[1].str());
	shader.SourceLocation = FilePath;
	return shader;
}