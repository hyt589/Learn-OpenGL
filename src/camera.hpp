#include <GL/gl.h>
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

    glm::vec3 Right()
    {
        return glm::normalize(glm::cross(m_cameraFront, m_cameraUp));
    }

    glm::vec3 Dir()
    {
        return m_cameraPos + m_cameraFront;
    }

public:
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
};