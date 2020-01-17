#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <glad/glad.h>

class Shader
{
private:
    std::string m_path;
    std::string m_src;
    int m_type;
    unsigned int m_id;
    std::string load_shader(std::string path);

public:
    Shader(std::string path, int type);
    unsigned int ID();
    ~Shader();
    Shader() = delete;
};

Shader::Shader(std::string path, int type)
    : m_path(path), m_type(type), m_src(load_shader(path))
{
    const char * typeName = (type == GL_VERTEX_SHADER)
                           ? "VERTEX"
                           : (type == GL_FRAGMENT_SHADER)
                                 ? "FRAGMENT"
                                 : "UNKNOWN";
    const char * src = m_src.c_str();
    m_id = glCreateShader(type);
    glShaderSource(m_id, 1, &src, NULL);
    glCompileShader(m_id);
    int success;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        int length;
        glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *)alloca(sizeof(char) * length);
        glGetShaderInfoLog(m_id, length, &length, message);
        std::cout << "[ERROR]::SHADER::" << typeName << "::COMPILATION_FAILED" << std::endl
                  << message << std::endl;
    } else
    {
        std::cout << "[INFO]::SHADER::" << typeName << "::COMPILED" << '\n';
    }
    
    
}

unsigned int Shader::ID(){
    return this->m_id;
}

Shader::~Shader()
{
}

std::string Shader::load_shader(std::string path)
{
    // std::string shaderSource;
    // std::ifstream file;
    // file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    // try
    // {
    //     std::stringstream sourceStream;
    //     file.open(path);
    //     sourceStream << file.rdbuf();
    //     shaderSource == sourceStream.str();
    // }
    // catch(std::ifstream::failure e)
    // {
    //     std::cerr << "[ERROR]::SHADER::SOURCE::LOADING FAILED" << '\n';
    //     std::cerr << "  Path: " << path << '\n';
    // }
    
    // return shaderSource;

    std::string line, text;
    std::ifstream in(path);
    while (std::getline(in, line))
    {
        text += line + "\n";
    }
    return text;
}