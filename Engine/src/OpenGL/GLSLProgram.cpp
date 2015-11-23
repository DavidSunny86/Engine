#include "GLSLProgram.h"
#include <iostream>

GLSLProgram::GLSLProgram(const std::vector<GLSLShader*>& shaders)
{
    shaders_ = shaders;
    CompileProgram();
}

GLSLProgram::GLSLProgram(const std::string& pathToVertexShader, const std::string& pathToFragmentShader)
{
    shaders_.push_back(new GLSLShader(GLSLShaderType::Vertex, pathToVertexShader));
    shaders_.push_back(new GLSLShader(GLSLShaderType::Fragment, pathToFragmentShader));
    CompileProgram();
}

GLSLProgram::~GLSLProgram()
{
    for (auto shader : shaders_)
        delete shader;
}

GLint GLSLProgram::GetUniformLocation(std::string name)
{
    return glGetUniformLocation(id_, name.c_str());
}

void GLSLProgram::RecompileProgram()
{
    glDeleteProgram(id_);
    for (auto shader : shaders_)
        shader->RecompileShader();
    CompileProgram();
}

GLuint GLSLProgram::ID()
{
    return id_;
}

void GLSLProgram::CompileProgram()
{
    id_ = glCreateProgram();
    for (auto shader : shaders_)
        glAttachShader(id_, shader->ID());
    
    glLinkProgram(id_);
    GLint results;
    glGetProgramiv(id_, GL_LINK_STATUS, &results);
    if (results != GL_TRUE)
    {
        GLint logsize;
        GLchar* log = NULL;
        glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &logsize);
        log = new GLchar[logsize + 1];
        glGetProgramInfoLog(id_, logsize, &logsize, log);
        std::cerr << log << std::endl;

        glDeleteProgram(id_);
        delete[] log;
    }

}


