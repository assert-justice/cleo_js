#pragma once

class Texture{
    public:
    unsigned int id;
    int width;
    int height;
    Texture(int width, int height, unsigned char* data);
    ~Texture();
    void use();
};