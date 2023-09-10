#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "utils/store.hpp"
#include "shader.hpp"
#include "texture.hpp"

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
    int newTexture(int width, int height, unsigned char* data);
    int newImage(const char* path);
    // void drawTexture(
    //     unsigned int texture, 
    //     float x, float y, float width, float height,
    //     float sx, float sy, float sWidth, float sHeight,
    //     unsigned int targetTexture);
    private:
    Shader imageShader;
    bool initalized = false;
    unsigned int VBO, VAO;
    Store<Texture> textureStore;
    glm::mat4 cameraTransform;
    glm::mat4 spriteTransform;
};