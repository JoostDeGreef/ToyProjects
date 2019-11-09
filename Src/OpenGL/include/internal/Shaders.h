#pragma once

namespace OpenGL
{
    class Shaders
    {
    public:
        enum class Type
        {
            None,
            Text,
        };

        Shaders();

        static Type Select(Type shader);
        static int GetUniformLocation(const char* const name);
        static void LoadShaderSources(Data& data);
    private:
        Type m_selected;
        std::map<Type, unsigned int> m_shaders;
        std::map<Type, std::pair<Data::Bytes,Data::Bytes>> m_shaderSources;
    };
}; // namespace Viewer

