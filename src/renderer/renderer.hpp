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
    int loadImage(const char* path);
    void drawImage(
        int textureId, 
        float x, float y, float width, float height,
        float sx, float sy, float sw, float sh);
    Store<Texture> textureStore;
    private:
    Shader imageShader;
    bool initalized = false;
    unsigned int VBO, VAO;
    glm::mat4 cameraTransform;
};