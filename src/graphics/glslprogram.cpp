#include "glslprogram.h"

#include "gtc/type_ptr.hpp"
#include "util/shader.h"

#include "iostream"

namespace graphics {
    GLSLProgram::GLSLProgram()
    {
        initializeOpenGLFunctions();

        this->m_programID = glCreateProgram();

        this->m_shaders = new std::vector<GLint>();
        this->m_uniforms = new std::map<std::string, GLint>();
    }

    GLSLProgram::~GLSLProgram() {
        glDeleteProgram(this->m_programID);

        delete m_shaders;
        delete m_uniforms;
    }

    void GLSLProgram::Link() {
        glLinkProgram(this->m_programID);
        glUseProgram(this->m_programID);

        for(GLint shader: *m_shaders) {
            glDetachShader(this->m_programID, shader);
            glDeleteShader(shader);
        }
    }

    void GLSLProgram::Use() {
        glUseProgram(this->m_programID);
    }

    GLint GLSLProgram::AddShader(GLenum type, std::string source) {
        const GLchar* source_ptr = source.c_str();
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source_ptr, NULL);
        glCompileShader(shader);
        glAttachShader(this->m_programID, shader);

        this->m_shaders->push_back(shader);
        return shader;
    }

    void GLSLProgram::BindFragDataLocation(GLint color, const std::string& name) {
        glBindFragDataLocation(this->m_programID, color, name.c_str());
    }

    GLint GLSLProgram::SetAttribute(const std::string& name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) {
        GLint attrib = glGetAttribLocation(this->m_programID, name.c_str());

        //std::cout << m_programID << std::endl;
        glEnableVertexAttribArray(attrib);
        glVertexAttribPointer(attrib, size, type, normalized, stride, pointer);

        return attrib;
    }

    GLint GLSLProgram::AddUniform(const std::string& name) {
        GLint uniform = glGetUniformLocation(this->m_programID, name.c_str());

        (*this->m_uniforms)[name] = uniform;
        return uniform;
    }

    GLint GLSLProgram::GetUniform(const std::string &name) {
        auto iter = this->m_uniforms->find(name);
        if(iter != this->m_uniforms->end()) {
            return iter->second;
        }

        return -1;
    }

    void GLSLProgram::SetUniform(const std::string name, glm::mat4x4& value) {
        SetUniform((*this->m_uniforms)[name], value);
    }

    void GLSLProgram::SetUniform(GLint location, glm::mat4x4& value) {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void GLSLProgram::SetUniform(const std::string name, glm::vec3& value) {
        SetUniform((*this->m_uniforms)[name], value);
    }

    void GLSLProgram::SetUniform(GLint location, glm::vec3& value) {
        glUniform3fv(location, 1, glm::value_ptr(value));
    }

    void GLSLProgram::SetUniform(const std::string name, float value) {
        SetUniform((*this->m_uniforms)[name], value);
    }

    void GLSLProgram::SetUniform(GLint location, float value) {
        glUniform1f(location, value);
    }

    GLuint GLSLProgram::GetProgramID() {
        return this->m_programID;
    }
}


