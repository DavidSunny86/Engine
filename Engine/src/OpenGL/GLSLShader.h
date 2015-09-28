#pragma once
#include <string>
#include <fstream>
#include "GL\glew.h"

enum GLSLShaderType
{
    Vertex = 0,
    Tessalation_Control,
    Tessalation_Evaluation,
    Geometry,
    Fragment,
    Compute
};

class GLSLShader
{
public:
    GLSLShader(const GLSLShaderType type, const std::string& path);
    ~GLSLShader();

    void RecompileShader();

    const GLSLShaderType Type() const;
    const std::string& PathToFile() const;
    const GLuint ID() const;
    
    void Type(const GLSLShaderType type);
protected:
    void LoadFile();
    void CompileShader();
    GLuint GetShaderDefine();

    GLSLShaderType type_;
    GLchar* code_;
    std::string pathToFile_;
    GLuint id_;
};

