#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H

#include <qopenglfunctions_4_5_core.h>
#include "glm.hpp"

namespace graphics {
    class GLSLProgram : public QOpenGLFunctions_4_5_Core
    {
    private:
        std::string vertexSourceCode;
        std::string fragmentSourceCode;

        GLuint vertexShader = 0;
        GLuint fragmentShader = 0;

        GLuint program = 0;

    public:
        GLSLProgram();
        GLSLProgram(const char* vertexPath, const char* fragmentPath);

        void SetSources(const char* vertexSource, const char* fragmentSource);

        void Prepare();
        GLuint GetUniform(const GLchar* name);
        GLuint GetAttribute(const GLchar* name);
        void LoadUniformMat4x4(GLuint uniform, const glm::mat4x4& data);
    };
}

#endif // GLSLPROGRAM_H
