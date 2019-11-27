#include <string>
#include <unordered_map>
#include <algorithm>

#include "Data.h"
#include "OpenGL.h"
#include "Logger.h"

using namespace std;
using namespace OpenGL;

namespace
{
    GLuint LoadShaders(const char* const vertex, const char* const fragment)
    {
        auto logger = Logger::Logger::Instance();

        // Create the shaders
        GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

        auto CheckCompilation = [](GLuint shaderID)
        {
            GLint result = GL_FALSE;
            int infoLogLength;
            glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
            glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
            if (infoLogLength > 0) 
            {
                std::vector<char> errorMessage(infoLogLength + 1);
                glGetShaderInfoLog(shaderID, infoLogLength, NULL, &errorMessage[0]);
                return std::string(errorMessage.begin(), errorMessage.end());
            }
            return std::string();
        };

        // Compile Vertex Shader
        glShaderSource(vertexShaderID, 1, &vertex, NULL);
        glCompileShader(vertexShaderID);

        // Check Vertex Shader
        auto vertexError = CheckCompilation(vertexShaderID);
        if (!vertexError.empty())
        {
            logger->Error(vertexError.c_str());
            return 0;
        }

        // Compile Fragment Shader
        glShaderSource(fragmentShaderID, 1, &fragment, NULL);
        glCompileShader(fragmentShaderID);

        // Check Fragment Shader
        auto fragmentError = CheckCompilation(fragmentShaderID);
        if (!fragmentError.empty())
        {
            logger->Error(fragmentError.c_str());
            glDeleteShader(vertexShaderID);
            return 0;
        }

        // Link the program
        GLuint programID = glCreateProgram();
        glAttachShader(programID, vertexShaderID);
        glAttachShader(programID, fragmentShaderID);
        glLinkProgram(programID);

        glDetachShader(programID, vertexShaderID);
        glDetachShader(programID, fragmentShaderID);

        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);

        // Check the program
        GLint result = GL_FALSE;
        int infoLogLength;
        glGetProgramiv(programID, GL_LINK_STATUS, &result);
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            std::vector<char> errorMessage(infoLogLength + 1);
            glGetProgramInfoLog(programID, infoLogLength, NULL, &errorMessage[0]);
            logger->Error(errorMessage.data());
            glDeleteProgram(programID);
            programID = 0;
        }

        return programID;
    }

    static Shaders shaders;
}

Shaders::Shaders()
    : m_selected(Type::None)
{
}

Shaders::Type Shaders::Select(Type shader)
{
    Type prev = shaders.m_selected;

    if (shaders.m_selected != shader)
    {
        auto AddAndSelectShader = [&](Type shader)
        {
            const char* const vertex = reinterpret_cast<char*>(shaders.m_shaderSources[Type::Text].first.data());
            const char* const fragment = reinterpret_cast<char*>(shaders.m_shaderSources[Type::Text].second.data());
            GLuint programId = LoadShaders(vertex, fragment);
            if (programId == 0)
            {
                shader = Type::None;
            }
            shaders.m_shaders.emplace(shader, programId);
            shaders.m_selected = shader;
        };

        auto iter = shaders.m_shaders.find(shader);
        if (iter == shaders.m_shaders.end())
        {
            switch (shader)
            {
            default:
                // log this?
            case Type::None:
                shaders.m_shaders.emplace(Type::None, 0);
                shaders.m_selected = Type::None;
                break;
            case Type::Text:
                AddAndSelectShader(Type::Text);
                break;
            }
            iter = shaders.m_shaders.find(shaders.m_selected);
        }
        else
        {
            shaders.m_selected = shader;
        }
        glUseProgram(iter->second);
    }

    return prev;
}

int Shaders::GetUniformLocation(const char* const name)
{
    // store this value?
    auto iter = shaders.m_shaders.find(shaders.m_selected);
    assert(iter != shaders.m_shaders.end());
    return glGetUniformLocation(iter->second, name);
}

void Shaders::LoadShaderSources(Data& data)
{
    shaders.m_shaderSources[Type::Text] = data[std::string("shaders")].GetShader("text");
}
