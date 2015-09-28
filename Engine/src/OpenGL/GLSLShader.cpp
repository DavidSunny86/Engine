#include "GLSLShader.h"
#include <iostream>

GLSLShader::GLSLShader(const GLSLShaderType type, const std::string & pathToFile)
{
    type_ = type;
    pathToFile_ = pathToFile;
    CompileShader();
}

GLSLShader::~GLSLShader()
{
    glDeleteShader(id_);
}

void GLSLShader::RecompileShader()
{
    glDeleteShader(id_);
    CompileShader();
}

const GLSLShaderType GLSLShader::Type() const
{
    return type_;
}

const std::string & GLSLShader::PathToFile() const
{
    return pathToFile_;
}

const GLuint GLSLShader::ID() const
{
    return id_;
}

void GLSLShader::Type(const GLSLShaderType type)
{
    type_ = type;
}

void GLSLShader::LoadFile()
{
    std::ifstream file(pathToFile_, std::ios::in | std::ios::binary);

    if (!file.is_open())
        throw new std::invalid_argument("Can't find file");

    file.seekg(0, file.end);
    unsigned int size = (unsigned int)file.tellg();
    file.seekg(0, file.beg);

    code_ = new GLchar[size + 1];
    int i = 0;
    while (!file.eof())
    {
        code_[i++] = file.get();
    }
    code_[i - 1] = 0;
}

void GLSLShader::CompileShader()
{
    LoadFile();
    id_ = glCreateShader(GetShaderDefine());
    glShaderSource(id_, 1, &code_, NULL);
    glCompileShader(id_);

    GLint compileStatus = GL_TRUE;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE)
    {
        GLint logSize = 0;
        char* log = NULL;
        glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &logSize);
        log = new char[logSize + 1];
        glGetShaderInfoLog(id_, logSize, &logSize, log);
        std::cerr << log << std::endl;
        delete[] log;
    }
}

GLuint GLSLShader::GetShaderDefine()
{
    switch (type_)
    {
        case Vertex:
            return GL_VERTEX_SHADER;
        case Tessalation_Control:
            return GL_TESS_CONTROL_SHADER;
        case Tessalation_Evaluation:
            return GL_TESS_EVALUATION_SHADER;
        case Geometry:
            return GL_GEOMETRY_SHADER;
        case Fragment:
            return GL_FRAGMENT_SHADER;
        case Compute:
            return GL_COMPUTE_SHADER;
        default:
            return 0;
    }
}
