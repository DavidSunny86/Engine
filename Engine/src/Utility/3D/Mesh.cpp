#include "Mesh.h"
#include "OpenGL\GLSLProgram.h"
#include "Texture.h"
#include "Material.h"

Mesh::Mesh(const aiMesh* mesh)
{
    vbo_[Position] = NULL;
    vbo_[TextureCoord] = NULL;
    vbo_[Normal] = NULL;
    vbo_[Index] = NULL;
    LoadMesh(mesh);
}


Mesh::~Mesh()
{
    if (vbo_[Position]) glDeleteBuffers(1, &vbo_[Position]);
    if (vbo_[TextureCoord]) glDeleteBuffers(1, &vbo_[TextureCoord]);
    if (vbo_[Normal]) glDeleteBuffers(1, &vbo_[Normal]);
    if (vbo_[Index]) glDeleteBuffers(1, &vbo_[Index]);

    glDeleteVertexArrays(1, &vao_);
}

void Mesh::Render(GLSLProgram* program)
{
    material_->ApplyMaterial(program);
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, numberOfElement_, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::RenderNoMaterial()
{
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, numberOfElement_, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::SetMaterial(Material* material)
{
    material_ = material;
}

void Mesh::LoadMesh(const aiMesh* mesh)
{
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    numberOfElement_ = mesh->mNumFaces * 3;

    if (mesh->HasPositions())
    {
        float* vertices = new float[mesh->mNumVertices * 3];
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
        {
            vertices[i * 3 + 0] = mesh->mVertices[i].x;
            vertices[i * 3 + 1] = mesh->mVertices[i].y;
            vertices[i * 3 + 2] = mesh->mVertices[i].z;
        }
        glGenBuffers(1, &vbo_[Position]);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_[Position]);
        glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);
        delete[] vertices;
    }
    if (mesh->HasNormals())
    {
        float* normals = new float[mesh->mNumVertices * 3];
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
        {
            normals[3 * i + 0] = mesh->mNormals[i].x;
            normals[3 * i + 1] = mesh->mNormals[i].y;
            normals[3 * i + 2] = mesh->mNormals[i].z;
        }
        glGenBuffers(1, &vbo_[Normal]);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_[Normal]);
        glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), normals, GL_STATIC_DRAW);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);
        delete[] normals;
    }
    unsigned int j = 0;
    while(mesh->HasTextureCoords(j))
    {
        float* coord = new float[mesh->mNumVertices * 2];
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
        {
            coord[i * 2 + 0] = mesh->mTextureCoords[j][i].x;
            coord[i * 2 + 1] = mesh->mTextureCoords[j][i].y;
        }

        glGenBuffers(1, &vbo_[TextureCoord]);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_[TextureCoord]);
        glBufferData(GL_ARRAY_BUFFER, 2 * mesh->mNumVertices * sizeof(GLfloat), coord, GL_STATIC_DRAW);

        glVertexAttribPointer(j+2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(j+2);
        delete[] coord;
        ++j;
    }
    if (mesh->HasFaces())
    {
        GLuint* indices = new GLuint[mesh->mNumFaces * 3];
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
        {
            indices[3 * i + 0] = mesh->mFaces[i].mIndices[0];
            indices[3 * i + 1] = mesh->mFaces[i].mIndices[1];
            indices[3 * i + 2] = mesh->mFaces[i].mIndices[2];
        }
        glGenBuffers(1, &vbo_[Index]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_[Index]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfElement_ * sizeof(GLuint), indices, GL_STATIC_DRAW);
        delete[] indices;
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
