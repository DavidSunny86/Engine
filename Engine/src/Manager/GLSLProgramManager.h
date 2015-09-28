#pragma once
#include <map>
#include <string>

#include "OpenGL/GLSLProgram.h"
class GLSLProgramManager
{
public:
    ~GLSLProgramManager();

    static GLSLProgramManager* Instance();
    GLSLProgram* GetProgram(std::string name);
protected:
    void LoadPrograms(std::string& path);
    GLSLProgramManager(std::string& path);
    static GLSLProgramManager* instance_;
    std::map<std::string, GLSLProgram*> programs_;
};

