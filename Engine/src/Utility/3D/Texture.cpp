#include <iostream>
#include "Texture.h"
#include "IL\il.h"
#include "IL\ilut.h"

Texture::Texture(const std::string& pathToImage, TextureType type)
{

    type_ = type;
    pathToImage_ = pathToImage;
    LoadTexture();
}


Texture::~Texture()
{
}

void Texture::ReloadTexture()
{
    glDeleteTextures(1, &id_);
    LoadTexture();
}

unsigned int Texture::Width()
{
    return width_;
}

unsigned int Texture::Height()
{
    return height_;
}

unsigned int Texture::BytePerPixel()
{
    return bytePerPixel_;
}

GLuint Texture::ID()
{
    return id_;
}

TextureType Texture::Type()
{
    return type_;
}

void Texture::LoadTexture()
{
    GLenum error = glGetError();
    glGenTextures(1, &id_);
    ILuint imageId = ilGenImage();

    ilBindImage(imageId);
    std::wstring temp = std::wstring(pathToImage_.begin(), pathToImage_.end());
    ILboolean success = ilLoadImage(temp.c_str());
    if (!success)
        std::cerr << "Error while loading the texture file: " << pathToImage_ << std::endl;
   
    width_ = ilGetInteger(IL_IMAGE_WIDTH);
    height_ = ilGetInteger(IL_IMAGE_HEIGHT);
    bytePerPixel_ = ilGetInteger(IL_IMAGE_BPP);
    GLuint format;
    switch (bytePerPixel_)
    {
    case 1:
        format = GL_R;
        break;
    case 2:
        format = GL_RG;
        break;
    case 3:
        format = GL_RGB;
        break;
    case 4:
        format = GL_RGBA;
        break;
    default:
        format = GL_RGBA;
        break;
    }
    glBindTexture(GL_TEXTURE_2D, id_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width_, height_, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
    ilDeleteImage(imageId);
}
