#pragma once

class Texture{
    public:
    unsigned int textureId;
    unsigned int frameBufferId;
    int width;
    int height;
    Texture(int width, int height, unsigned char* data);
    ~Texture();
    void use();
    void useTarget();
};