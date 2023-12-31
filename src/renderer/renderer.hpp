#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H 
#include "utils/store.hpp"
#include "shader.hpp"
#include "texture.hpp"

class Renderer{
    public:
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
    void enable(){enabled = true;}
    void disable(){enabled = false;}
    Texture* getTexture(int id){
        return textureStore.get(id);
    }
    bool isEnabled(){
        return enabled || target != nullptr;
    }
    bool isUsingRenderTarget(){
        return target != nullptr;
    }
    private:
    Store<Texture> textureStore;
    Texture* target = nullptr;
    bool enabled = false;
    Shader imageShader;
    // Shader textShader;
    bool initalized = false;
    unsigned int VBO, VAO;
    unsigned int fbo, fbt;
    glm::mat4 cameraTransform;
    // FT_Library ft;
    // FT_Face face;
};