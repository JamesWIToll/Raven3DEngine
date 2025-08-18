#include <Raven3DEngineCore.h>

using namespace Raven3DEngineCore::Rendering;


void GLShader::Initialize(const std::string &vertex, const std::string &fragment, const bool &useAsPaths) {
    std::string vertexCode;
    std::string fragmentCode;


    if (useAsPaths) {
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            vShaderFile.open(vertex);
            fShaderFile.open(fragment);
            std::stringstream vShaderStream, fShaderStream;

            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            vShaderFile.close();
            fShaderFile.close();

            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch ([[maybe_unused]] std::ifstream::failure &e) {
            RAVEN_LOG_ERROR("ERROR could not find shader files: {}, {}", vertex, fragment);
            return;
        }
    } else {
        vertexCode = vertex;
        fragmentCode = fragment;
    }


    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();


    unsigned int vertexID, fragmentID {};
    int success {};
    char infoLog[512] {};

    vertexID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexID, 1, &vShaderCode, nullptr);
    glCompileShader(vertexID);
    glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexID, 512, nullptr, infoLog);
        RAVEN_LOG_ERROR("ERROR failed to compile vertex shader {}: {}", vertex,  infoLog);
        return;
    }

    fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentID, 1, &fShaderCode, nullptr);
    glCompileShader(fragmentID);
    glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentID, 512, nullptr, infoLog);
        RAVEN_LOG_ERROR("ERROR failed to compile frag shader {}: {}", fragment,  infoLog);
        return;
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertexID);
    glAttachShader(ID, fragmentID);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        RAVEN_LOG_ERROR("ERROR failed to link shader program: {}", infoLog);
        return;
    }

    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);
}


void GLShader::use() const {
    glUseProgram(ID);
}

void GLShader::setBool(const std::string &name, const bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void GLShader::setInt(const std::string &name, const int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void GLShader::setFloat(const std::string &name, const float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void GLShader::setMat3(const std::string &name, const glm::mat3 &matrix) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void GLShader::setMat4(const std::string &name, const glm::mat4 &matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void GLShader::setVec4(const std::string &name, glm::vec4 vector) const {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vector));
}

void GLShader::setVec3(const std::string &name, glm::vec3 vector) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vector));
}

void GLShader::setVec2(const std::string &name, glm::vec2 vector) const {
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vector));
}
