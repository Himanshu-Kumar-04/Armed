#include"ArmPCH.h"
#include "openGLShader.h"
#include <glm/gtc/type_ptr.hpp>
#include <glad.h>


////////////////////////////////////////////////////////////////
// readShader();
// createShader(); both vertex and fragment shader
// compileShader();
// 
// uploadUniform();
// uses function getUniformLocation();
//////////////////////////////////////////////////////////////// 
namespace Arm {
    static GLenum ShaderTypeFromString(const std::string& type) {
        if (type == "vertex_Shader")
            return GL_VERTEX_SHADER;
        else if (type == "fragment_Shader" || type == "pixel_Shader")
            return GL_FRAGMENT_SHADER;

        ARM_ASSERT(false, "unknown shader type");
        return 0;
    }

    OpenGLShader::OpenGLShader(const std::string& filepath)
        :m_RendererID(0),m_FilePath(filepath)
    {
        std::string source = readFile(filepath);
        auto shaderSource = preProcess(source);
        compileShader(shaderSource);

        //assets/shader/flat_color.glsl
        auto lastSlash = filepath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = filepath.rfind('.');
        auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
        m_Name = filepath.substr(lastSlash, count);
    }

    OpenGLShader::OpenGLShader(const std::string name, const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
        :m_Name(name)
    {
        std::unordered_map<GLenum, std::string> shaderSource;
        shaderSource[GL_VERTEX_SHADER] = vertexShaderSource;
        shaderSource[GL_FRAGMENT_SHADER] = fragmentShaderSource;
        compileShader(shaderSource);
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_RendererID);
    }

    void OpenGLShader::bind() const
    {
        glUseProgram(m_RendererID);
    }

    void OpenGLShader::unbind() const
    {
        glUseProgram(0);
    }

    void OpenGLShader::setInt(const std::string& name, int value)
    {
        uploadUniformInt1(name, value);
    }

    void OpenGLShader::setIntArray(const std::string& name, int* values, uint32_t count)
    {
        uploadUniformIntArray(name, values, count);
    }

    void OpenGLShader::setFloat(const std::string& name, float value)
    {
        uploadUniformFloat1(name, value);
    }

    void OpenGLShader::setFloat2(const std::string& name, const glm::vec2& value)
    {
        uploadUniformFloat2(name, value);
    }

    void OpenGLShader::setFloat3(const std::string& name, const glm::vec3& value)
    {
        uploadUniformFloat3(name, value);
    }

    void OpenGLShader::setFloat4(const std::string& name, const glm::vec4& value)
    {
        uploadUniformFloat4(name, value);
    }

    void OpenGLShader::setMat4(const std::string& name, const glm::mat4& value)
    {
        uploadUniformMat4(name, value);
    }

    ////////////////////////////////////////////////////////////////
    // Set up Uniform for each type
    //////////////////////////////////////////////////////////////// 

    void OpenGLShader::uploadUniformInt1(const std::string& name, int32_t v1)
    {
        glUniform1i(getUniformLocation(name), v1);
    }

    void OpenGLShader::uploadUniformIntArray(const std::string& name, int* values, uint32_t count)
    {
        glUniform1iv(getUniformLocation(name), count, values);
    }

    void OpenGLShader::uploadUniformFloat1(const std::string& name, float vec)
    {
        glUniform1f(getUniformLocation(name), vec);
    }

    void OpenGLShader::uploadUniformFloat2(const std::string& name, const glm::vec2& vec)
    {
        glUniform2f(getUniformLocation(name), vec.x, vec.y);
    }

    void OpenGLShader::uploadUniformFloat3(const std::string& name, const glm::vec3& vec)
    {
        glUniform3f(getUniformLocation(name), vec.x, vec.y, vec.z);
    }

    void OpenGLShader::uploadUniformFloat4(const std::string& name, const glm::vec4& vec)
    {
        glUniform4f(getUniformLocation(name), vec.x, vec.y, vec.z, vec.w);
    }

    void OpenGLShader::uploadUniformMat3(const std::string name, const glm::mat3& matrix)
    {
        glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::uploadUniformMat4(const std::string name, const  glm::mat4& matrix)
    {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    std::string OpenGLShader::readFile(const std::string& filepath)
    {
        std::string line, shaderString;
        std::ifstream search(filepath, std::ios::in, std::ios::binary);
        if (search) {
            search.seekg(0, std::ios::end);
            shaderString.resize(search.tellg());
            search.seekg(0, std::ios::beg);
            search.read(&shaderString[0], shaderString.size());
            search.close();
        }
        else {
            ARM_ERROR("could not open file: %s\n", filepath);
        }

        return shaderString;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::preProcess(const std::string& source)
    {
        std::unordered_map<GLenum, std::string> shaderSource;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0);
        while (pos != std::string::npos) {
            size_t eol = source.find_first_of("\r\n", pos);
            ARM_ASSERT(eol != std::string::npos, "syntax error");
            size_t begin = pos + typeTokenLength + 1;
            std::string type = source.substr(begin, eol - begin);
            ARM_ASSERT(ShaderTypeFromString(type), "invalid shader specified specified");

            size_t nextLinePos = source.find_first_not_of("\r\n", eol);
            pos = source.find(typeToken, nextLinePos);
            shaderSource[ShaderTypeFromString(type)] =
                source.substr(nextLinePos,
                pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
        }

        return shaderSource;
    }

    void  OpenGLShader::compileShader(const std::unordered_map<GLenum, std::string>& shaderSource)
    {
        GLenum program = glCreateProgram();

        std::vector<GLenum> shaderIDs(shaderSource.size());
        for (auto& kv : shaderSource) {
            GLenum type = kv.first;
            const std::string& source = kv.second;

            GLuint shader = glCreateShader(type);
            const char* src = source.c_str();
            glShaderSource(shader, 1, &src, nullptr);
            glCompileShader(shader);

            int result;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
            if (result == GL_FALSE) {
                int length = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
                char* message = (char*)_malloca(length * sizeof(char));
                glGetShaderInfoLog(shader, length, &length, message);
                glDeleteShader(shader);

                ARM_ERROR("%s",message);
                ARM_ASSERT(false, "shader compilation failure");
                return;
            }
            glAttachShader(program, shader);
            shaderIDs.push_back(shader);
        }

        glLinkProgram(program);

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
            //TO CHANGE

            glDeleteProgram(program);

            for (auto id : shaderIDs)
                glDeleteShader(id);
        }

        for (GLenum id : shaderIDs) {
            glDetachShader(program, id);
        }

        m_RendererID = program;
    }

    int32_t OpenGLShader::getUniformLocation(const std::string& name)
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