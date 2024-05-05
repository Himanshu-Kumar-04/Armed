#include"ArmPCH.h"
#include "shader.h"
#include <glad.h>
#include <glm/gtc/type_ptr.hpp>

////////////////////////////////////////////////////////////////
// readShader();
// createShader(); both vertex and fragment shader
// compileShader();
// 
// setuniform();
// uses function getUniformLocation();
//////////////////////////////////////////////////////////////// 
namespace Arm {

    Shader::Shader()
        :m_RendererID(0)
    {
        //std::string sourceVS = readShader("res/shader/vertexShader.shader");
        std::string sourceVS = R"(
    #version 460 core

    layout(location = 0) in vec3 position;
    
    uniform mat4 u_ViewProjection;
    uniform mat4 u_Transform;

    out vec3 v_Position;
    
    void main()
    {
        v_Position = position;
        gl_Position = u_ViewProjection * u_Transform * vec4(position, 1.0);
    };
)";
        //std::string sourceFS = readShader("res/shader/fragmentShader.shader");
        std::string sourceFS = R"(
    #version 460 core
    
    layout(location = 0) out vec4 color;
    in vec3 v_Position;
    
    void main(){
        color = vec4(v_Position*0.5+0.5,1.0);
    };
)";
        m_RendererID = createShader(sourceVS, sourceFS);
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_RendererID);
    }

    Shader* Shader::Create()
    {
        return new Shader();
    }

    void Shader::bind()
    {
        glUseProgram(m_RendererID);
    }

    void Shader::unbind()
    {
        glUseProgram(0);
    }

    ////////////////////////////////////////////////////////////////
    // Set up Uniform for each type
    //////////////////////////////////////////////////////////////// 

    void Shader::setUniform1i(const std::string& name, int32_t v1)
    {
        glUniform1i(getUniformLocation(name), v1);
    }

    void Shader::setUniform1f(const std::string& name, float v1)
    {
        glUniform1f(getUniformLocation(name), v1);
    }

    void Shader::setUniform4f(const std::string& name, float v1, float v2, float v3, float v4)
    {
        glUniform4f(getUniformLocation(name), v1, v2, v3, v4);
    }

    void Shader::setUniformMat4(const std::string name,const  glm::mat4& matrix)
    {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    uint32_t Shader::compileShader(uint32_t type, const std::string& source) {
        uint32_t ID = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(ID, 1, &src, nullptr);
        glCompileShader(ID);

        int result;
        glGetShaderiv(ID, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            int length;
            glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)_malloca(length * sizeof(char));
            glGetShaderInfoLog(ID, length, &length, message);
            std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader\n" << message << "\n";
            glDeleteShader(ID);
            return 0;
        }
        return ID;
    }

    std::string Shader::readShader(const std::string& filePath)
    {
        std::ifstream search;
        search.open(filePath);
        std::string line,shaderString;
        while (getline(search, line)) {
            shaderString += line;
            shaderString += '\n';
        }
        return shaderString;
    }

    uint32_t Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader) {
        uint32_t program = glCreateProgram();
        uint32_t vs = compileShader(GL_VERTEX_SHADER, vertexShader);
        uint32_t fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

        glAttachShader(program, vs);
        glAttachShader(program, fs);

        glLinkProgram(program);
        glValidateProgram(program);

        glDetachShader(program, vs);
        glDetachShader(program, fs);

        return program;
    }

    int32_t Shader::getUniformLocation(const std::string& name)
    {
        if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
            return m_UniformLocationCache[name];
        int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location == -1)
            std::cout << "Warning: Uniform " << name << " doesn't exist\n";
        m_UniformLocationCache[name] = location;
        return location;
    }
}