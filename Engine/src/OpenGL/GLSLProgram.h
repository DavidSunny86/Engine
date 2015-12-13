#pragma once
#include <vector>
#include <map>
#include "GL\glew.h"

class GLSLShader;

class GLSLProgram
{
public:
    GLSLProgram(const std::vector<GLSLShader*>& shaders);
    GLSLProgram(const std::string& pathToVertexShader, const std::string& pathToFragmentShader);
    ~GLSLProgram();
    
    void Activate();
    void Deactivate();
    GLint GetUniformLocation(std::string name);
    void RecompileProgram();
    GLuint ID();
protected:
    void CompileProgram();
    GLuint id_;
    std::vector<GLSLShader*> shaders_;
};

