#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "utils/store.hpp"
#include "shader.hpp"
#include "texture.hpp"

class Renderer{
    public:
    Renderer();
    ~Renderer();
    void init(bool* hasError);
    void update();
    bool isInitialized();
    void setClearColor(float r, float g, float b, float a);
    void clear();
    int newTexture(int width, int height, unsigned char* data);
    void freeTexture(int id);
    int loadImage(const char* path);
    void drawImage(
        int textureId,
        glm::mat4 spriteTransform,
        glm::mat4 coordTransform
    );
    void setCameraPosition(float x, float y);
    void setTarget(Texture* target);
    Store<Texture> textureStore;
    float camWidth = 800;
    float camHeight = 600;
    Texture* target = nullptr;
    bool enabled = false;
    private:
    Shader imageShader;
    bool initalized = false;
    unsigned int VBO, VAO;
    unsigned int fbo, fbt;
    glm::mat4 cameraTransform;
};