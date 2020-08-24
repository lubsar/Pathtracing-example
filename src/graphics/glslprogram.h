#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H

#include <QOpenGLFunctions_4_5_Core>

#include "glm.hpp"

namespace graphics {
    class GLSLProgram : private QOpenGLFunctions_4_5_Core
    {
    private:
        GLuint m_programID;
        std::vector<GLint>* m_shaders;
        std::map<std::string, GLint>* m_uniforms;

    public:
        GLSLProgram();
        ~GLSLProgram();

        GLint AddShader(GLenum type, std::string source);
        void BindFragDataLocation(GLint color, const std::string& name);

        GLint SetAttribute(const std::string& name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);

        GLint AddUniform(const std::string& name);
        GLint GetUniform(const std::string& name);

        void SetUniform(const std::string name, glm::mat4x4& value);
        void SetUniform(GLint location, glm::mat4x4& value);
        void SetUniform(const std::string name, glm::vec3& value);
        void SetUniform(GLint location, glm::vec3& value);
        void SetUniform(const std::string name, float value);
        void SetUniform(GLint location, float value);

        void Link();
        void Use();

        GLuint GetProgramID();
    };
}



#endif // GLSLPROGRAM_H
