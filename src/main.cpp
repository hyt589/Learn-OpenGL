#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include "config.cpp"
#include <list>
#include "stb_image.h"
#include "image.cpp"

using JSON = nlohmann::json;

JSON config = loadConfig();

#define WINDOW_WIDTH config["windowWidth"]
#define WINDOW_HEIGHT config["windowHeight"]
#define VERTEX_SHADER_PATH config["vertexShaderPath"]
#define FRAGMENT_SHADER_PATH config["fragmentShaderPath"]
#define TEXTURE_PATHS config["texturePaths"]

//resize GL viewport if the window is resized
static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true); //exit if ESC key is pressed
    }
}

static std::string read_file(std::string path)
{
    std::string line, text;
    std::ifstream in(path);
    while (std::getline(in, line))
    {
        text += line + "\n";
    }
    return text;
}

static unsigned int compileShader(int shader_type, std::string source_path)
{
    std::string source = read_file(source_path);
    const char *type = (shader_type == GL_VERTEX_SHADER) ? "VERTEX" : (shader_type == GL_FRAGMENT_SHADER) ? "FRAGMENT" : "UNKNOWN";
    unsigned int shader_id = glCreateShader(shader_type);
    const char *src = source.c_str();
    glShaderSource(shader_id, 1, &src, NULL);
    glCompileShader(shader_id);
    int success;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        int length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *)alloca(sizeof(char) * length);
        glGetShaderInfoLog(shader_id, length, &length, message);
        std::cout << "[ERROR]::SHADER::" << type << "::COMPILATION_FAILED" << std::endl
                  << message << std::endl;
    }
    // std::cout << "[INFO]::SHADER::SOURCE::" << type << std::endl;
    // std::cout << src << std::endl;

    return shader_id;
}

static unsigned int createProgram(std::list<unsigned int> shaders)
{
    unsigned int program_id = glCreateProgram();
    std::list<unsigned int>::iterator iterator;
    for (iterator = shaders.begin(); iterator != shaders.end(); ++iterator)
    {
        glAttachShader(program_id, *iterator);
    }
    glLinkProgram(program_id);
    int success;
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        int length;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *)alloca(sizeof(char) * length);
        glGetProgramInfoLog(program_id, length, &length, message);
    }
    for (iterator = shaders.begin(); iterator != shaders.end(); ++iterator)
    {
        glDeleteShader(*iterator);
    }
    return program_id;
}

int main(int, char **)
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __MACH__ //Run following if on Mac OS
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create a window
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load openGL endpoints after window and context are created
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // initialize GL viewport and set resizing callback
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    unsigned int vertex_shader = compileShader(GL_VERTEX_SHADER, VERTEX_SHADER_PATH);
    unsigned int fragment_shader = compileShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_PATH);
    std::list<unsigned int> shaders;
    shaders.push_front(vertex_shader);
    shaders.push_front(fragment_shader);
    unsigned int program_id = createProgram(shaders);

    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    };

    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao); //bind vao before vbo and ebo

    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //create a vertex buffer object and send vertex data
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0); //safe to unbind after vertexAttribPointer is set

    TextureCreator textureCreator(program_id);
    unsigned int texture_0 = textureCreator.createTexture(TEXTURE_PATHS[0]);
    unsigned int texture_1 = textureCreator.createTexture(TEXTURE_PATHS[1]);
    

    // std::cout << typeid(config["texturePaths"]).name() << std::endl;
    //render loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // int containerTex = glGetUniformLocation(program_id, "containerTexture");
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture_1);

        glUseProgram(program_id);
        // glUniform4f(u_color, r, g, b, 1.0f);
        glBindVertexArray(vao);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindTexture(GL_TEXTURE_2D, texture_id);
        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // terminate glfw before exiting
    glfwTerminate();
    return 0;
}
