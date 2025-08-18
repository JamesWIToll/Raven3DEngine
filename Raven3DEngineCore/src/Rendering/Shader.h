#ifndef SHADER_H
#define SHADER_H

namespace Raven3DEngineCore::Rendering {
    class IShader {
    public:
        virtual ~IShader() = default;

        unsigned int ID;
        virtual void Initialize(const std::string &vertexPath, const std::string &fragmentPath) = 0;
        virtual void use() const = 0;
        virtual void setBool(const std::string& name, bool value) const = 0;
        virtual void setInt(const std::string& name, int value) const = 0;
        virtual void setFloat(const std::string& name, float value) const = 0;
        virtual void setMat3(const std::string &name, const glm::mat3 &matrix) const = 0;
        virtual void setMat4(const std::string &name, const glm::mat4 &matrix) const = 0;
        virtual void setVec4(const std::string &name, glm::vec4 vector) const = 0;
        virtual void setVec3(const std::string &name, glm::vec3 vector) const = 0;
        virtual void setVec2(const std::string &name, glm::vec2 vector) const = 0;
    };

    class GLShader final : public IShader {
    public:
        GLShader() = default;
        ~GLShader() override {
            glDeleteProgram(ID);
        }

        void Initialize(const std::string &vertexPath, const std::string &fragmentPath) override;
        void use() const override;
        void setBool(const std::string &name, bool value) const override;
        void setInt(const std::string &name, int value) const override;
        void setFloat(const std::string &name, float value) const override;
        void setMat3(const std::string &name, const glm::mat3 &matrix) const override;
        void setMat4(const std::string &name, const glm::mat4 &matrix) const override;
        void setVec4(const std::string &name, glm::vec4 vector) const override;
        void setVec3(const std::string &name, glm::vec3 vector) const override;
        void setVec2(const std::string &name, glm::vec2 vector) const override;
    };
}


#endif //SHADER_H