#include <glad/glad.h>
#include <glm-0.9.9.6/glm/glm.hpp>
#include <glm-0.9.9.6/glm/gtc/matrix_transform.hpp>
#include <glm-0.9.9.6/glm/gtc/type_ptr.hpp>

class Camera
{
private:
    glm::vec3 m_cameraPos;
    glm::vec3 m_cameraTarget;
    glm::vec3 m_cameraUp;

    glm::vec3 m_cameraFront;
    glm::vec3 m_cameraRight;

    bool firstMouse = true;
    double lastX;
    double lastY;
    float yaw = -90;
    float pitch = 0;

    

    glm::vec3 Right()
    {
        return glm::normalize(glm::cross(m_cameraFront, m_cameraUp));
    }

    glm::vec3 Dir()
    {
        return m_cameraPos + m_cameraFront;
    }

public:
    static Camera theCamera;
    Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 up)
        : m_cameraPos(pos), m_cameraTarget(target), m_cameraUp(glm::normalize(up))
    {
        m_cameraFront = glm::normalize(m_cameraTarget - m_cameraPos);
        m_cameraRight = glm::normalize(glm::cross(m_cameraUp, m_cameraFront));
    }

    ~Camera() {}

    Camera() = delete;

    glm::mat4 lookAt()
    {
        return glm::lookAt(m_cameraPos, Dir(), m_cameraUp);
    }

    void moveForward(float speed)
    {
        m_cameraPos += speed * m_cameraFront;
    }

    void moveBackward(float speed)
    {
        m_cameraPos -= speed * m_cameraFront;
    }

    void moveRight(float speed)
    {
        m_cameraPos += speed * Right();
    }

    void moveLeft(float speed)
    {
        m_cameraPos -= speed * Right();
    }

    void moveUp(float speed)
    {
        m_cameraPos += speed * m_cameraUp;
    }

    void moveDown(float speed)
    {
        m_cameraPos -= speed * m_cameraUp;
    }

    void steer(glm::vec3 steer)
    {
        m_cameraFront = steer;
    }

    glm::vec3 front()
    {
        return glm::vec3(m_cameraFront);
    }

    void mouse_callback(GLFWwindow *window, double xpos, double ypos)
    {

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.05;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        this->steer(glm::normalize(direction));
    }

    static void mouse_callback_wrapper(GLFWwindow *window, double xpos, double ypos){
        theCamera.mouse_callback(window, xpos, ypos);
    }
};