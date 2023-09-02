#pragma once
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <vector>

typedef struct TextureData{
    unsigned int idx;
    unsigned int width;
    unsigned int height;
}TextureData;

class Renderer{
    public:
    Renderer();
    ~Renderer();
    void init(bool* hasError);
    void update();
    bool isInitialized();
    void setClearColor(float r, float g, float b);
    unsigned int newTexture(unsigned int width, unsigned int height);
    unsigned int newImage(const char* path);
    void drawTexture(
        unsigned int texture, 
        float x, float y, float width, float height,
        float sx, float sy, float sWidth, float sHeight,
        unsigned int targetTexture);
    private:
    bool initalized = false;
    std::vector<TextureData> textures;
};