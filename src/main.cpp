#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include "config.cpp"
#include <list>
#include "stb_image.h"
#include "image.cpp"
#include <glm-0.9.9.6/glm/glm.hpp>
#include <glm-0.9.9.6/glm/gtc/matrix_transform.hpp>
#include <glm-0.9.9.6/glm/gtc/type_ptr.hpp>
#include <shader.hpp>
#include <program.hpp>
#include <camera.hpp>

using JSON = nlohmann::json;

Camera Camera::theCamera = Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

static float currentFrame = 0.0f;
static float lastFrame = 0.0f;
static float deltaTime = 0.0f;

#define WINDOW_WIDTH config["windowWidth"]
#define WINDOW_HEIGHT config["windowHeight"]
#define VERTEX_SHADER_PATH config["vertexShaderPath"]
#define FRAGMENT_SHADER_PATH config["fragmentShaderPath"]
#define TEXTURE_PATHS config["texturePaths"]
#define LIGHT_SOURCE_SHADER_PATH config["lsFragShaderPath"]

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

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        Camera::theCamera.moveForward((float)config["cameraSpeed"] * deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        Camera::theCamera.moveBackward((float)config["cameraSpeed"] * deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        Camera::theCamera.moveLeft((float)config["cameraSpeed"] * deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        Camera::theCamera.moveRight((float)config["cameraSpeed"] * deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        Camera::theCamera.moveUp((float)config["cameraSpeed"] * deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        Camera::theCamera.moveDown((float)config["cameraSpeed"] * deltaTime);
    }
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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, &Camera::mouse_callback_wrapper);

    Shader vertexShader(VERTEX_SHADER_PATH, GL_VERTEX_SHADER);
    Shader fragmentShader(FRAGMENT_SHADER_PATH, GL_FRAGMENT_SHADER);
    Program program(vertexShader.ID(), fragmentShader.ID());

    Shader lightSourceVertexShader(VERTEX_SHADER_PATH, GL_VERTEX_SHADER);
    Shader lightSourceFragShader(LIGHT_SOURCE_SHADER_PATH, GL_FRAGMENT_SHADER);
    Program lsProgram(lightSourceVertexShader.ID(), lightSourceFragShader.ID());

    // float vertices[] = {
    //     // positions          // colors           // texture coords
    //     0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
    //     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
    //     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
    //     -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    // };

    // float vertices[] = {
    //     -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    //     0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    //     0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    //     0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    //     -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    //     -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    //     -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    //     0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    //     0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    //     0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    //     -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
    //     -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

    //     -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    //     -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    //     -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    //     -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    //     -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    //     -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    //     0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    //     0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    //     0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    //     0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    //     -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    //     0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
    //     0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    //     0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    //     -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    //     -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    //     -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    //     0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    //     0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    //     0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    //     -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
    //     -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); //safe to unbind after vertexAttribPointer is set

    TextureCreator textureCreator(program.ID());
    unsigned int texture_0 = textureCreator.createTexture(TEXTURE_PATHS[0]);
    unsigned int texture_1 = textureCreator.createTexture(TEXTURE_PATHS[1]);

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};

    unsigned int lightVao;
    glGenVertexArrays(1, &lightVao);
    glBindVertexArray(lightVao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glm::vec3 lightPos(0.0f, 1.0f, -2.0f);

    //render loop
    while (!glfwWindowShouldClose(window))
    {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        glm::mat4 view = Camera::theCamera.lookAt();
        glm::mat4 projection = glm::perspective(glm::radians((float)config["fov"]), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
        glm::mat4 normalModel = glm::transpose(glm::inverse(model));

        program.Use();
        program.setUniformMat4("model", model);
        program.setUniformMat4("view", view);
        program.setUniformMat4("normalModel", normalModel);
        program.setUniformMat4("projection", projection);
        program.setUniformVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
        program.setUniformVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        program.setUniformVec3("lightPos", lightPos);
        program.setUniformVec3("camPos", Camera::theCamera.getPos());

        glBindVertexArray(vao);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        for (unsigned int i = 0; i < 10; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i + 20;
            model = glm::rotate(model, glm::radians(angle) * (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
            normalModel = glm::transpose(glm::inverse(model));
            program.setUniformMat4("model", model);
            program.setUniformMat4("normalModel", normalModel);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        // glBindTexture(GL_TEXTURE_2D, texture_id);
        // glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        // program.setUniformMat4("model", model);
        lsProgram.Use();
        lsProgram.setUniformMat4("model", model);
        lsProgram.setUniformMat4("view", view);
        lsProgram.setUniformMat4("projection", projection);
        glBindVertexArray(lightVao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // terminate glfw before exiting
    glfwTerminate();
    return 0;
}
