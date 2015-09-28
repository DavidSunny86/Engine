#pragma once
#include <vector>
#include <map>
#include "GL\glew.h"

#include "GLSLShader.h"
class GLSLProgram
{
public:
    GLSLProgram(const std::vector<GLSLShader*>& shaders);
    GLSLProgram(const std::string& pathToVertexShader, const std::string& pathToFragmentShader);
    ~GLSLProgram();
    
    GLint GetUniformLocation(std::string name);
    void RecompileProgram();
    GLuint ID();
protected:
    void CompileProgram();
    GLuint id_;
    std::vector<GLSLShader*> shaders_;
    std::map<std::string, GLint> uniforms_;
};

