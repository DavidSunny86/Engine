#include <iostream>

#include "GLSLProgram.h"
#include "GLSLShader.h"

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

void GLSLProgram::Activate()
{
    glUseProgram(id_);
}

void GLSLProgram::Deactivate()
{
    glUseProgram(0);
}

GLint GLSLProgram::GetUniformLocation(std::string name)
{
    GLint location = glGetUniformLocation(id_, name.c_str());
    return location;
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
    {
        glAttachShader(id_, shader->ID());
        if (shader->Type() == Geometry)
        {
            if (shader->PathToFile() == ".\\Shaders\\Terrain/FirstPassGeometry.glsl")
            {
                const GLchar* feedbackOut[] = {"z8_y8_x8_case8"};
                glTransformFeedbackVaryings(id_, 1, feedbackOut, GL_INTERLEAVED_ATTRIBS);

            }
            else if (shader->PathToFile() == ".\\Shaders\\Terrain/SecondPassGeometry.glsl")
            {
                const GLchar* feedbackOut[] = { "z8_y8_x8_null4_edge4" };
                glTransformFeedbackVaryings(id_, 1, feedbackOut, GL_INTERLEAVED_ATTRIBS);
            }
            else if (shader->PathToFile() == ".\\Shaders\\Terrain/ThirdPassGeometry.glsl")
            {
                const GLchar* feedbackOut[] = { "outData.wsPosition", "outData.normal" };
                glTransformFeedbackVaryings(id_, 2, feedbackOut, GL_INTERLEAVED_ATTRIBS);
            }
            else if (shader->PathToFile() == ".\\Shaders\\Terrain/FifthPassGeometry.glsl")
            {
                const GLchar* feedbackOut[] = { "index" };
                glTransformFeedbackVaryings(id_, 1, feedbackOut, GL_INTERLEAVED_ATTRIBS);
            }
        }
    }

    
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


