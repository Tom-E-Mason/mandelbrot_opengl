
#pragma once

#include <string>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class shader
{
public:
    shader(std::string path, uint32_t type)
    {
        auto ifs = std::fstream(path);

        std::string buf;
        while (std::getline(ifs, buf))
        {
            m_source += buf;
            m_source += '\n';
        }

        m_id = glCreateShader(type);
        const char* src = m_source.c_str();

        glShaderSource(m_id, 1, &src, NULL);
        glCompileShader(m_id);

        int result;
        glGetShaderiv(m_id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            int length;
            glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &length);

            std::string message;
            message.resize(length);

            glGetShaderInfoLog(m_id, length, &length, message.data());

            std::cout << message << '\n' << '\n' << '\n';
        }
    }

    const char* c_str() const { return m_source.c_str(); }
    auto string() const { return m_source; }
    auto id() const { return m_id; }

private:
    uint32_t m_id;
    std::string m_source;
};