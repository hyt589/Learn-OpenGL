#pragma once

#include <iostream>
#include <GL/gl.h>
#include <glm-0.9.9.6/glm/glm.hpp>
#include <glm-0.9.9.6/glm/gtc/type_ptr.hpp>

class Program
{
private:
    unsigned int m_program_id;
    unsigned int m_vertex_shader_id;
    unsigned int m_frag_shader_id;

public:
    Program(unsigned int vs, unsigned int fs)
        : m_vertex_shader_id(vs), m_frag_shader_id(fs)
    {
        m_program_id = glCreateProgram();
        glAttachShader(m_program_id, vs);
        glAttachShader(m_program_id, fs);
        glLinkProgram(m_program_id);
        int success;
        glGetProgramiv(m_program_id, GL_LINK_STATUS, &success);
        if (!success)
        {
            int length;
            glGetProgramiv(m_program_id, GL_INFO_LOG_LENGTH, &length);
            char *message = (char *)alloca(sizeof(char) * length);
            glGetProgramInfoLog(m_program_id, length, &length, message);
            std::cout << "[ERROR]::PROGRAM::LINKING_FAILED" << '\n';
            std::cout << message << '\n';
        }
        else
        {
            std::cout << "[INFO]::PROGRAM::LINKING_SUCCESS" << '\n';
        }
        glDeleteShader(vs);
        glDeleteShader(fs);
    }

    Program() = delete;

    void Use()
    {
        glUseProgram(m_program_id);
    }

    void setUniformMat4(const std::string name, glm::mat4 value){
        int uniformLoc = glGetUniformLocation(m_program_id, name.c_str());
        if (uniformLoc == -1)
        {
            std::cout << "[ERROR]::UNIFORM::NOT_FOUND::" << name << '\n';
            return;
        }
        glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(value));
    }

    unsigned int ID()
    {
        return m_program_id;
    }

    ~Program(){

    };
};
