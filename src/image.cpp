#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <nlohmann/json.hpp>
#include <glad/glad.h>
#include <iostream>

using JONS = nlohmann::json;

class TextureCreator
{
private:
    int m_textureUnitNumber = GL_TEXTURE0;
    int m_index = 0;
    const unsigned int m_programId;
public:

    TextureCreator(unsigned int program_id) : m_programId(program_id)
    {

    }

    TextureCreator() = delete;

    unsigned int createTexture(std::string path)
    {
        //load image at target location
        int width, height, numChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &numChannels, STBI_rgb);
        if (!data)
        {
            std::cout << "Failed to load texture: " << path << std::endl;
        }

        //create a texture with the image in openGL
        unsigned int texture;
        glGenTextures(1, &texture);
        glActiveTexture(m_textureUnitNumber);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        //generate a  mipmap with the texture
        glGenerateMipmap(GL_TEXTURE_2D);
        //free memory of the loaded image
        stbi_image_free(data);
        //unbind the texture;
        // glBindTexture(GL_TEXTURE_2D, 0);
        m_textureUnitNumber++;
        std::string uniformName = "texture";
        uniformName += std::to_string(m_index);
        glUseProgram(m_programId);
        glUniform1i(glGetUniformLocation(m_programId, uniformName.c_str()), m_index);
        m_index++;
        return texture;
    }
};