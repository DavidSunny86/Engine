#pragma once
#include <glm/glm.hpp>
#include <GL\glew.h>
#include <assimp\mesh.h>

class GLSLProgram;
class Texture;
class Material;

enum VBOType {
    Position = 0,
    TextureCoord,
    Normal,
    Index
};

class Mesh
{
public:
    Mesh(const aiMesh* mesh);
    ~Mesh();

    void Render(GLSLProgram* program);
    void RenderNoMaterial();
    void SetMaterial(Material* material);
protected:
    void LoadMesh(const aiMesh* mesh);
    Material* material_;
    GLuint vao_;
    GLuint vbo_[4];
    unsigned int numberOfElement_;
};

