#pragma once
#include <unordered_map>
#include "shader.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// typedef struct TextureData{
//     unsigned int idx;
//     unsigned int width;
//     unsigned int height;
// }TextureData;

class Renderer{
    public:
    Renderer();
    ~Renderer();
    void init(bool* hasError);
    void update();
    bool isInitialized();
    void setClearColor(float r, float g, float b);
    // unsigned int newTexture(unsigned int width, unsigned int height);
    // unsigned int newImage(const char* path);
    // void drawTexture(
    //     unsigned int texture, 
    //     float x, float y, float width, float height,
    //     float sx, float sy, float sWidth, float sHeight,
    //     unsigned int targetTexture);
    private:
    Shader imageShader;
    bool initalized = false;
    unsigned int VBO, VAO;
    unsigned int texture;
    int maxTextureId = 0;
    glm::mat4 cameraTransform;
    glm::mat4 spriteTransform;
    // std::unordered_map<int, TextureData> textureMap;
};