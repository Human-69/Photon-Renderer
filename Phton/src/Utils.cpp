#include "Photon.h"

unsigned int VAO, VBO, EBO;

GLFWwindow* init(int width, int height, std::string name)
{
    GLFWwindow* window;
    std::cout << "Making Buffers\n";
    if (!glfwInit())
    {
        std::cout << "Failed To intilize glfw";
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        std::cout << "Failed to create window";
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    MainWindow = window;

    if (glewInit() != GLEW_OK)
        std::cout << "Failed to initilize glew\n";
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    return window;
}

bool wireFrame = false;

//to slow down TAB toggle
double lastToggleTime = 0.0;
const double toggleCooldown = 0.2;

void keyCallBack(GLFWwindow* window, float DeltaTime)
{
    float cameraSpeed = static_cast<float>(2.5*DeltaTime);
    if (MainCamera.size() > 0) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            MainCamera[0]->cameraPos += cameraSpeed * MainCamera[0]->cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            MainCamera[0]->cameraPos -= cameraSpeed * MainCamera[0]->cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            MainCamera[0]->cameraPos -= glm::normalize(glm::cross(MainCamera[0]->cameraFront, MainCamera[0]->cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            MainCamera[0]->cameraPos += glm::normalize(glm::cross(MainCamera[0]->cameraFront, MainCamera[0]->cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) 
        {
            double currentTime = glfwGetTime();
            if (currentTime - lastToggleTime >= toggleCooldown) {
                if (!wireFrame)
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    wireFrame = !wireFrame;
                }
                else
                {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    wireFrame = !wireFrame;
                }
                lastToggleTime = currentTime;
            }
        }
    }
}

float lastX = 400, lastY = 300;
bool firstMouse;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
    if (firstMouse) // initially set to true
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    std::cout << xoffset<< " " << yoffset << std::endl;
    if(MainCamera.size()>0)
    {
        MainCamera[0]->yaw += xoffset;
        MainCamera[0]->pitch += yoffset;
        if (MainCamera[0]->pitch > 89.0f)
            MainCamera[0]->pitch = 89.0f;
        if (MainCamera[0]->pitch < -89.0f)
            MainCamera[0]->pitch = -89.0f;
        glm::vec3 front;
        front.x = cos(glm::radians(MainCamera[0]->yaw)) * cos(glm::radians(MainCamera[0]->pitch));
        front.y = sin(glm::radians(MainCamera[0]->pitch));
        front.z = sin(glm::radians(MainCamera[0]->yaw)) * cos(glm::radians(MainCamera[0]->pitch));
        MainCamera[0]->cameraFront = glm::normalize(front);
    };
};