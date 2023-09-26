#pragma once

class Texture{
    public:
    unsigned int textureId;
    unsigned int frameBufferId;
    int width;
    int height;
    float camX, camY;
    Texture(int width, int height, unsigned char* data = nullptr, int channels = 4);
    ~Texture();
    void use();
    void useTarget();
};