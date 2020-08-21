#include "shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

namespace util {
    std::string load_shader_source(const char* path) {
        using namespace std;

        ifstream vertexStream(path);
        if(!vertexStream.is_open()) {
            cout << "Error opening shader file: " << path << endl;
            return "";
        } else {
            ostringstream buffer;
            buffer << vertexStream.rdbuf();
            return buffer.str();
        }
    }
}

