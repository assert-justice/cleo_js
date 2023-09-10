#include "texture.hpp"
#include <glad/glad.h>

Texture::Texture(int width, int height, unsigned char* data){
    this->width = width;
    this->height = height;
    glGenTextures(1, &id);
    use();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
}

Texture::~Texture(){
    glDeleteTextures(1, &id);
}

void Texture::use(){
    glBindTexture(GL_TEXTURE_2D, id);
}