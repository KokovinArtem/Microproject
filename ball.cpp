

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

#include "stb_image.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const float radius = 0.3f;

//camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

float lastX = 400, lastY = 300;
float yaw = -90.0f;
float pitch = 0.0f;

bool firstMouse = true;

//camera speed calibration

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame



float fov = 45.0f;

//our shaders

/*const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\n\0";
*/
int main()
{

    //glfw: initialize and configure
    //------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); for MacOS
    //glfw windiw creatiton
    //---------------------
    GLFWwindow* window = glfwCreateWindow(800, 600, "Ball", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    //================================
    //build and compile shader program
    //--------------------------------
    //vertex shader
    /*nsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
        {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
        {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
        {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);*/ //SInce we have written a special header file to manage with shaders

    //Textures

    int width, height, nrChannels;
    unsigned char *data = stbi_load("/home/artemkokovin/Docs/micropoject/res/container.jpg", &width, &height, &nrChannels, 0);

    unsigned int texture1, texture2, texture3, texture4;

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);
    glGenTextures(1, &texture4);
    glBindTexture(GL_TEXTURE_2D, texture4);
    data = stbi_load("/home/artemkokovin/Docs/microproject/res/ball.jpg", &width, &height, &nrChannels, 0);


    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("/home/artemkokovin/Docs/microproject/res//awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glGenTextures(1, &texture3);
    glBindTexture(GL_TEXTURE_2D, texture3);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load("/home/artemkokovin/Docs/microproject/res/floor.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //build and compile our shader program
    //-----------------------------------
    glEnable(GL_DEPTH_TEST);

    Shader ourShader("/home/artemkokovin/Docs/microproject/shader.vs", "/home/artemkokovin/Docs/microproject/shader.fs");
    Shader ballShader("/home/artemkokovin/Docs/microproject/floor_shader.vs", "/home/artemkokovin/Docs/microproject/floor_shader.fs");
    Shader floorShader("/home/artemkokovin/Docs/microproject/floor_shader.vs", "/home/artemkokovin/Docs/microproject/floor_shader.fs");
    Shader trajShader("/home/artemkokovin/Docs/microproject/trajectory.vs", "/home/artemkokovin/Docs/microproject/trajectory.fs");
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float sphere_coords[] = {
            0, 1, 0, 0, 0,
            0, 1, 0, 0.1, 0,
            0, 1, 0, 0.2, 0,
            -0, 1, 0, 0.3, 0,
            -0, 1, 0, 0.4, 0,
            -0, 1, 0, 0.5, 0,
            -0, 1, -0, 0.6, 0,
            -0, 1, -0, 0.7, 0,
            0, 1, -0, 0.8, 0,
            0, 1, -0, 0.9, 0,
            0, 1, -0, 1, 0,
            0.309017, 0.951057, 0, 0, 0.1,
            0.25, 0.951057, 0.181636, 0.1, 0.1,
            0.0954915, 0.951057, 0.293893, 0.2, 0.1,
            -0.0954915, 0.951057, 0.293893, 0.3, 0.1,
            -0.25, 0.951057, 0.181636, 0.4, 0.1,
            -0.309017, 0.951057, 3.78437e-17, 0.5, 0.1,
            -0.25, 0.951057, -0.181636, 0.6, 0.1,
            -0.0954915, 0.951057, -0.293893, 0.7, 0.1,
            0.0954915, 0.951057, -0.293893, 0.8, 0.1,
            0.25, 0.951057, -0.181636, 0.9, 0.1,
            0.309017, 0.951057, -7.56873e-17, 1, 0.1,
            0.587785, 0.809017, 0, 0, 0.2,
            0.475528, 0.809017, 0.345491, 0.1, 0.2,
            0.181636, 0.809017, 0.559017, 0.2, 0.2,
            -0.181636, 0.809017, 0.559017, 0.3, 0.2,
            -0.475528, 0.809017, 0.345491, 0.4, 0.2,
            -0.587785, 0.809017, 7.19829e-17, 0.5, 0.2,
            -0.475528, 0.809017, -0.345492, 0.6, 0.2,
            -0.181636, 0.809017, -0.559017, 0.7, 0.2,
            0.181636, 0.809017, -0.559017, 0.8, 0.2,
            0.475528, 0.809017, -0.345492, 0.9, 0.2,
            0.587785, 0.809017, -1.43966e-16, 1, 0.2,
            0.809017, 0.587785, 0, 0, 0.3,
            0.654509, 0.587785, 0.475528, 0.1, 0.3,
            0.25, 0.587785, 0.769421, 0.2, 0.3,
            -0.25, 0.587785, 0.769421, 0.3, 0.3,
            -0.654509, 0.587785, 0.475528, 0.4, 0.3,
            -0.809017, 0.587785, 9.9076e-17, 0.5, 0.3,
            -0.654508, 0.587785, -0.475528, 0.6, 0.3,
            -0.25, 0.587785, -0.769421, 0.7, 0.3,
            0.25, 0.587785, -0.769421, 0.8, 0.3,
            0.654508, 0.587785, -0.475528, 0.9, 0.3,
            0.809017, 0.587785, -1.98152e-16, 1, 0.3,
            0.951057, 0.309017, 0, 0, 0.4,
            0.769421, 0.309017, 0.559017, 0.1, 0.4,
            0.293893, 0.309017, 0.904509, 0.2, 0.4,
            -0.293893, 0.309017, 0.904508, 0.3, 0.4,
            -0.769421, 0.309017, 0.559017, 0.4, 0.4,
            -0.951057, 0.309017, 1.16471e-16, 0.5, 0.4,
            -0.769421, 0.309017, -0.559017, 0.6, 0.4,
            -0.293893, 0.309017, -0.904508, 0.7, 0.4,
            0.293893, 0.309017, -0.904508, 0.8, 0.4,
            0.769421, 0.309017, -0.559017, 0.9, 0.4,
            0.951057, 0.309017, -2.32942e-16, 1, 0.4,
            1, 6.12323e-17, 0, 0, 0.5,
            0.809017, 6.12323e-17, 0.587785, 0.1, 0.5,
            0.309017, 6.12323e-17, 0.951057, 0.2, 0.5,
            -0.309017, 6.12323e-17, 0.951056, 0.3, 0.5,
            -0.809017, 6.12323e-17, 0.587785, 0.4, 0.5,
            -1, 6.12323e-17, 1.22465e-16, 0.5, 0.5,
            -0.809017, 6.12323e-17, -0.587785, 0.6, 0.5,
            -0.309017, 6.12323e-17, -0.951056, 0.7, 0.5,
            0.309017, 6.12323e-17, -0.951056, 0.8, 0.5,
            0.809017, 6.12323e-17, -0.587785, 0.9, 0.5,
            1, 6.12323e-17, -2.44929e-16, 1, 0.5,
            0.951056, -0.309017, 0, 0, 0.6,
            0.769421, -0.309017, 0.559017, 0.1, 0.6,
            0.293893, -0.309017, 0.904508, 0.2, 0.6,
            -0.293893, -0.309017, 0.904508, 0.3, 0.6,
            -0.769421, -0.309017, 0.559017, 0.4, 0.6,
            -0.951056, -0.309017, 1.16471e-16, 0.5, 0.6,
            -0.769421, -0.309017, -0.559017, 0.6, 0.6,
            -0.293893, -0.309017, -0.904508, 0.7, 0.6,
            0.293893, -0.309017, -0.904508, 0.8, 0.6,
            0.769421, -0.309017, -0.559017, 0.9, 0.6,
            0.951056, -0.309017, -2.32942e-16, 1, 0.6,
            0.809017, -0.587785, 0, 0, 0.7,
            0.654509, -0.587785, 0.475528, 0.1, 0.7,
            0.25, -0.587785, 0.769421, 0.2, 0.7,
            -0.25, -0.587785, 0.769421, 0.3, 0.7,
            -0.654509, -0.587785, 0.475528, 0.4, 0.7,
            -0.809017, -0.587785, 9.9076e-17, 0.5, 0.7,
            -0.654508, -0.587785, -0.475528, 0.6, 0.7,
            -0.25, -0.587785, -0.769421, 0.7, 0.7,
            0.25, -0.587785, -0.769421, 0.8, 0.7,
            0.654508, -0.587785, -0.475528, 0.9, 0.7,
            0.809017, -0.587785, -1.98152e-16, 1, 0.7,
            0.587785, -0.809017, 0, 0, 0.8,
            0.475528, -0.809017, 0.345491, 0.1, 0.8,
            0.181636, -0.809017, 0.559017, 0.2, 0.8,
            -0.181636, -0.809017, 0.559017, 0.3, 0.8,
            -0.475528, -0.809017, 0.345491, 0.4, 0.8,
            -0.587785, -0.809017, 7.19829e-17, 0.5, 0.8,
            -0.475528, -0.809017, -0.345492, 0.6, 0.8,
            -0.181636, -0.809017, -0.559017, 0.7, 0.8,
            0.181636, -0.809017, -0.559017, 0.8, 0.8,
            0.475528, -0.809017, -0.345492, 0.9, 0.8,
            0.587785, -0.809017, -1.43966e-16, 1, 0.8,
            0.309017, -0.951056, 0, 0, 0.9,
            0.25, -0.951056, 0.181636, 0.1, 0.9,
            0.0954915, -0.951056, 0.293893, 0.2, 0.9,
            -0.0954915, -0.951056, 0.293893, 0.3, 0.9,
            -0.25, -0.951056, 0.181636, 0.4, 0.9,
            -0.309017, -0.951056, 3.78437e-17, 0.5, 0.9,
            -0.25, -0.951056, -0.181636, 0.6, 0.9,
            -0.0954915, -0.951056, -0.293893, 0.7, 0.9,
            0.0954915, -0.951056, -0.293893, 0.8, 0.9,
            0.25, -0.951056, -0.181636, 0.9, 0.9,
            0.309017, -0.951056, -7.56874e-17, 1, 0.9,
            1.22465e-16, -1, 0, 0, 1,
            9.9076e-17, -1, 7.19829e-17, 0.1, 1,
            3.78437e-17, -1, 1.16471e-16, 0.2, 1,
            -3.78437e-17, -1, 1.16471e-16, 0.3, 1,
            -9.9076e-17, -1, 7.19829e-17, 0.4, 1,
            -1.22465e-16, -1, 1.49976e-32, 0.5, 1,
            -9.9076e-17, -1, -7.19829e-17, 0.6, 1,
            -3.78437e-17, -1, -1.16471e-16, 0.7, 1,
            3.78437e-17, -1, -1.16471e-16, 0.8, 1,
            9.9076e-17, -1, -7.19829e-17, 0.9, 1,
            1.22465e-16, -1, -2.99952e-32, 1, 1
    }; // now with UV
    unsigned int sphere_indices[]{
            11, 0, 1, 11, 1, 12,
            12, 1, 2, 12, 2, 13,
            13, 2, 3, 13, 3, 14,
            14, 3, 4, 14, 4, 15,
            15, 4, 5, 15, 5, 16,
            16, 5, 6, 16, 6, 17,
            17, 6, 7, 17, 7, 18,
            18, 7, 8, 18, 8, 19,
            19, 8, 9, 19, 9, 20,
            20, 9, 10, 20, 10, 21,
            22, 11, 12, 22, 12, 23,
            23, 12, 13, 23, 13, 24,
            24, 13, 14, 24, 14, 25,
            25, 14, 15, 25, 15, 26,
            26, 15, 16, 26, 16, 27,
            27, 16, 17, 27, 17, 28,
            28, 17, 18, 28, 18, 29,
            29, 18, 19, 29, 19, 30,
            30, 19, 20, 30, 20, 31,
            31, 20, 21, 31, 21, 32,
            33, 22, 23, 33, 23, 34,
            34, 23, 24, 34, 24, 35,
            35, 24, 25, 35, 25, 36,
            36, 25, 26, 36, 26, 37,
            37, 26, 27, 37, 27, 38,
            38, 27, 28, 38, 28, 39,
            39, 28, 29, 39, 29, 40,
            40, 29, 30, 40, 30, 41,
            41, 30, 31, 41, 31, 42,
            42, 31, 32, 42, 32, 43,
            44, 33, 34, 44, 34, 45,
            45, 34, 35, 45, 35, 46,
            46, 35, 36, 46, 36, 47,
            47, 36, 37, 47, 37, 48,
            48, 37, 38, 48, 38, 49,
            49, 38, 39, 49, 39, 50,
            50, 39, 40, 50, 40, 51,
            51, 40, 41, 51, 41, 52,
            52, 41, 42, 52, 42, 53,
            53, 42, 43, 53, 43, 54,
            55, 44, 45, 55, 45, 56,
            56, 45, 46, 56, 46, 57,
            57, 46, 47, 57, 47, 58,
            58, 47, 48, 58, 48, 59,
            59, 48, 49, 59, 49, 60,
            60, 49, 50, 60, 50, 61,
            61, 50, 51, 61, 51, 62,
            62, 51, 52, 62, 52, 63,
            63, 52, 53, 63, 53, 64,
            64, 53, 54, 64, 54, 65,
            66, 55, 56, 66, 56, 67,
            67, 56, 57, 67, 57, 68,
            68, 57, 58, 68, 58, 69,
            69, 58, 59, 69, 59, 70,
            70, 59, 60, 70, 60, 71,
            71, 60, 61, 71, 61, 72,
            72, 61, 62, 72, 62, 73,
            73, 62, 63, 73, 63, 74,
            74, 63, 64, 74, 64, 75,
            75, 64, 65, 75, 65, 76,
            77, 66, 67, 77, 67, 78,
            78, 67, 68, 78, 68, 79,
            79, 68, 69, 79, 69, 80,
            80, 69, 70, 80, 70, 81,
            81, 70, 71, 81, 71, 82,
            82, 71, 72, 82, 72, 83,
            83, 72, 73, 83, 73, 84,
            84, 73, 74, 84, 74, 85,
            85, 74, 75, 85, 75, 86,
            86, 75, 76, 86, 76, 87,
            88, 77, 78, 88, 78, 89,
            89, 78, 79, 89, 79, 90,
            90, 79, 80, 90, 80, 91,
            91, 80, 81, 91, 81, 92,
            92, 81, 82, 92, 82, 93,
            93, 82, 83, 93, 83, 94,
            94, 83, 84, 94, 84, 95,
            95, 84, 85, 95, 85, 96,
            96, 85, 86, 96, 86, 97,
            97, 86, 87, 97, 87, 98,
            99, 88, 89, 99, 89, 100,
            100, 89, 90, 100, 90, 101,
            101, 90, 91, 101, 91, 102,
            102, 91, 92, 102, 92, 103,
            103, 92, 93, 103, 93, 104,
            104, 93, 94, 104, 94, 105,
            105, 94, 95, 105, 95, 106,
            106, 95, 96, 106, 96, 107,
            107, 96, 97, 107, 97, 108,
            108, 97, 98, 108, 98, 109,
            110, 99, 100, 110, 100, 111,
            111, 100, 101, 111, 101, 112,
            112, 101, 102, 112, 102, 113,
            113, 102, 103, 113, 103, 114,
            114, 103, 104, 114, 104, 115,
            115, 104, 105, 115, 105, 116,
            116, 105, 106, 116, 106, 117,
            117, 106, 107, 117, 107, 118,
            118, 107, 108, 118, 108, 119,
            119, 108, 109, 119, 109, 120
    };

    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    /*glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };*/

    float floor[] = {
            -10.0f, 0.0f, -10.0f,  0.0f, 0.0f,
            -10.0f, 0.0f, 10.0f,  10.0f, 0.0f,
            10.0f, 0.0f, -10.0f,  0.0f, 10.0f,
            -10.0f, 0.0f, 10.0f,  10.0f, 0.0f,
            10.0f, 0.0f, -10.0f,  0.0f, 10.0f,
            10.0f, 0.0f, 10.0f,  10.0f, 10.0f,
    };

    /*unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };*/

    //physics matrices

    glm::vec3 start(0.0f, 1.0f, 0.0f);

    float beta = 0.005f;

    glm::vec3 velocity(0.0f, 0.0f, 0.2f);
    glm::vec3 g_acceleration(0.0f, -2.5f, 0.0f);
    glm::vec3 angular_velocity(0.0f, 1.0f, 0.0f);
    glm::vec3 friction_acceleration = -beta * velocity;
    glm::vec3 acceleration = g_acceleration + friction_acceleration;
    glm::vec3 trajectory = start;
    glm::vec3 trajectory_un = start;
    glm::vec3 velocity_un = velocity;
    glm::vec3 acceleration_un = acceleration;

    unsigned int VBOs[2], VAOs[2], EBO;
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);
    glGenBuffers(1, &EBO);
    //glGenBuffers(2, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_coords), sphere_coords, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_indices), sphere_indices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));


    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    //glEnableVertexAttribArray(2);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);	// Vertex attributes stay the same
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));	// Vertex attributes stay the same
    glEnableVertexAttribArray(1);

    // trajectory
    unsigned int VAO_trajectory, VBO_trajectory;
    glGenVertexArrays(1, &VAO_trajectory);
    glGenBuffers(1, &VBO_trajectory);
    glBindVertexArray(VAO_trajectory);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_trajectory);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glBindVertexArray(0);

    // trajectory_nf
    unsigned int VAO_trajectory_nf, VBO_trajectory_nf;
    glGenVertexArrays(1, &VAO_trajectory_nf);
    glGenBuffers(1, &VBO_trajectory_nf);
    glBindVertexArray(VAO_trajectory_nf);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_trajectory_nf);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glBindVertexArray(0);


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
    // or set it via the texture class
    ourShader.setInt("texture2", 1);

    trajShader.use();

    trajShader.setVec4("color", glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));

    floorShader.use();
    floorShader.setInt("texture1", 2);
    ballShader.use();
    ballShader.setInt("texture1", 3);

    std::vector<float> trajectory_affected;
    std::vector<float> trajectory_unaffected;
    unsigned int i = 0;

    //render loop
    while(!glfwWindowShouldClose(window))
    {
        //input
        processInput(window);


        //renderning commands here

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture3);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, texture4);

        //drawing

        ballShader.use();

        //physics calculationsa

        friction_acceleration = -beta * velocity;
        acceleration = g_acceleration + friction_acceleration;
        trajectory += deltaTime * velocity;
        velocity += acceleration * deltaTime;
        acceleration_un = g_acceleration;
        trajectory_un += velocity_un * deltaTime;
        velocity_un += acceleration_un * deltaTime;





        //create transformations
        glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);
        //float camX = sin(glfwGetTime()) * radius;
        //float camZ = cos(glfwGetTime()) * radius;
        //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        //view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);

        projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // retrieve the matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(ballShader.ID, "model");
        unsigned int viewLoc  = glGetUniformLocation(ballShader.ID, "view");
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ballShader.setMat4("projection", projection);

        floorShader.use();

        // retrieve the matrix uniform locations
        modelLoc = glGetUniformLocation(floorShader.ID, "model");
        viewLoc  = glGetUniformLocation(floorShader.ID, "view");
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("projection", projection);

        trajShader.use();
        modelLoc = glGetUniformLocation(trajShader.ID, "model");
        viewLoc  = glGetUniformLocation(trajShader.ID, "view");
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        trajShader.setMat4("projection", projection);


        glBindVertexArray(VAO_trajectory);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_trajectory);
        glBufferData(GL_ARRAY_BUFFER, trajectory_affected.size() * sizeof(float), &trajectory_affected[0], GL_DYNAMIC_DRAW);
        trajShader.setVec4("color", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        glDrawArrays(GL_LINE_STRIP, 0, i);
        glBindVertexArray(0);

        glBindVertexArray(VAO_trajectory_nf);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_trajectory_nf);
        glBufferData(GL_ARRAY_BUFFER, trajectory_unaffected.size() * sizeof(float), &trajectory_unaffected[0], GL_DYNAMIC_DRAW);
        trajShader.setVec4("color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        glDrawArrays(GL_LINE_STRIP, 0, i);
        glBindVertexArray(0);

        floorShader.use();





        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        ballShader.use();
        glBindVertexArray(VAOs[0]);
        model = glm::translate(model, trajectory);
        glm::mat4 model_un = glm::translate(glm::mat4(1.0f), trajectory_un);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(180.0f), angular_velocity);
        model = glm::scale(model, glm::vec3(radius));
        model_un = glm::scale(model_un, glm::vec3(radius));
        if (((model * ((glm::vec4(start, 1.0f)) /*+ deltaTime * glm::vec4(velocity, 1.0f)*/)).y) <= 0.3f + radius)
        {
            //model = glm::translate(model, glm::vec3(0.0f, (model * glm::vec4(trajectory, 1.0f)).y + 0.2f + radius, 0.0f));

            //velocity_un = glm::vec3(velocity_un.x, -velocity_un.y, velocity_un.z);
            velocity = glm::vec3(velocity.x, -velocity.y, velocity.z);
        }

        if (((model_un * ((glm::vec4(start, 1.0f)) /*+ deltaTime * glm::vec4(velocity, 1.0f)*/)).y) <= 0.3f + radius)
        {
            //model = glm::translate(model, glm::vec3(0.0f, (model * glm::vec4(trajectory, 1.0f)).y + 0.2f + radius, 0.0f));

            velocity_un = glm::vec3(velocity_un.x, -velocity_un.y, velocity_un.z);
            //velocity = glm::vec3(velocity.x, -velocity.y, velocity.z);
        }
        ballShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, sizeof(sphere_indices), GL_UNSIGNED_INT, 0);

        if (i < 10000000)
        {
            trajectory_affected.push_back(trajectory.x);
            trajectory_affected.push_back(trajectory.y);
            trajectory_affected.push_back(trajectory.z);
            trajectory_unaffected.push_back(trajectory_un.x);
            trajectory_unaffected.push_back(trajectory_un.y);
            trajectory_unaffected.push_back(trajectory_un.z);
            i++;
        }


        // glBindVertexArray(0); // no need to unbind it every time
        //check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        //buffer clearing
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float currentFrame = glfwGetTime();
        deltaTime = (currentFrame - lastFrame) * 0.5f;
        lastFrame = currentFrame;

    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, VAOs);
    glDeleteBuffers(1, VAOs);
    //glDeleteBuffers(1, &EBO);

    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

}

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


    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch =  89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}