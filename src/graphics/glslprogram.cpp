#include "glslprogram.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "gtc/type_ptr.hpp"

namespace graphics {
    GLSLProgram::GLSLProgram() {
        initializeOpenGLFunctions();
    }

    GLSLProgram::GLSLProgram(const char* vertexPath, const char* fragmentPath) : GLSLProgram() {
        using namespace std;

        ifstream vertexStream(vertexPath);
        if(!vertexStream.is_open()) {
            cout << "Error opening vertex shader file: " << vertexPath << endl;
        } else {
            ostringstream buffer;
            buffer << vertexStream.rdbuf();
            this->vertexSourceCode = buffer.str();
            cout << this->vertexSourceCode << endl;
        }

        ifstream fragmentStream(fragmentPath);
        if(!fragmentStream.is_open()) {
            cout << "Error opening vertex shader file: " << fragmentPath << endl;
        } else {
            ostringstream buffer;
            buffer << fragmentStream.rdbuf();
            this->fragmentSourceCode = buffer.str();
            cout << this->fragmentSourceCode << endl;
        }
    }

    void GLSLProgram::SetSources(const char *vertexSource, const char *fragmentSource) {
        this->vertexSourceCode = vertexSource;
        this->fragmentSourceCode = fragmentSource;
    }

    //TODO generalize outColor etc., fix strings
    void GLSLProgram::Prepare() {
        const GLchar* source = this->vertexSourceCode.c_str();
        this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(this->vertexShader, 1, &source, NULL);
        glCompileShader(this->vertexShader);

        source = this->fragmentSourceCode.c_str();
        this->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(this->fragmentShader, 1, &source, NULL);
        glCompileShader(this->fragmentShader);

        this->program = glCreateProgram();
        glAttachShader(this->program, this->vertexShader);
        glAttachShader(this->program, this->fragmentShader);
        glBindFragDataLocation(this->program, 0, "outColor");
        glLinkProgram(this->program);
        glUseProgram(this->program);

        //cleanup
        glDetachShader(this->program, this->vertexShader);
        glDetachShader(this->program, this->fragmentShader);
    }

    GLuint GLSLProgram::GetUniform(const GLchar* name) {
        return glGetUniformLocation(this->program, name);
    }

    GLuint GLSLProgram::GetAttribute(const GLchar *name) {
        return glGetAttribLocation(this->program, name);
    }

    void GLSLProgram::LoadUniformMat4x4(GLuint uniform, const glm::mat4x4 &data) {
        glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(data));
    }
}
