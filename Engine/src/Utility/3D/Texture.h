#pragma once
#include <string>
#include <GL\glew.h>

enum TextureType
{
    None = 0,
    Diffuse,
    Specular,
    Ambient,
    Emissive,
    Height,
    Normals,
    Shininess,
    Opacity,
    Displacement,
    LightMap,
    Reflection,
    Unkown
};

class Texture
{
public:
    Texture(const std::string& pathToImage, TextureType type);
    ~Texture();
    void ReloadTexture();

    unsigned int Width();
    unsigned int Height();
    unsigned int BytePerPixel();
    GLuint ID();
    TextureType Type();
protected:
    void LoadTexture();

    TextureType type_;
    unsigned int width_;
    unsigned int height_;
    unsigned int bytePerPixel_;
    GLuint id_;
    std::string pathToImage_;
};

