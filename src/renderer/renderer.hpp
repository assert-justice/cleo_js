#pragma once
#include <unordered_map>
#include <vector>
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
    void startRender();
    void endRender();
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
    // void setCameraPosition(float x, float y);
    Texture* getTexture(int id){
        return textureStore.get(id);
    }
    bool isEnabled(){
        return enabled || isAtRoot();
    }
    // bool isUsingRenderTarget(){
    //     return target != nullptr;
    // }
    bool pushRenderTarget(int id);
    int popRenderTarget();
    void pushTransform(glm::mat4 transform){
        transformStack.push_back(transform);
    }
    void saveTransform(){
        // duplicates the top of the transform stack
        transformStack.push_back(getCurrentTransform());
    }
    void popTransform(){
        if(transformStack.size() > 0) transformStack.pop_back();
    }
    glm::mat4 getCurrentTransform(){
        return transformStack[transformStack.size()-1];
    }
    void translate(glm::vec3 vector){
        setCurrentTransform(glm::translate(getCurrentTransform(), vector));
    }
    void scale(glm::vec3 vector){
        setCurrentTransform(glm::scale(getCurrentTransform(), vector));
    }
    void rotate(float angle, glm::vec3 axis){
        setCurrentTransform(glm::rotate(getCurrentTransform(), angle, axis));
    }
    void setOrthoProjection(float left, float right, float top, float bottom, float near, float far){
        setCurrentTransform(glm::ortho(left, right, bottom, top, near, far));
    }
    void setPerspectiveProjection(float fov, float aspect, float near, float far){
        setCurrentTransform(glm::perspective(fov, aspect, near, far));
    }
    
    void setDimensions(int width, int height);
    private:
    Store<Texture> textureStore;
    // Texture* target = nullptr;
    std::vector<std::pair<int, Texture*>> renderTargetStack;
    bool enabled = true;
    Shader imageShader;
    // Shader textShader;
    bool initalized = false;
    unsigned int VBO, VAO;
    unsigned int fbo, fbt;
    // The transform used when rendering the last render target to the view
    glm::mat4 rootTransform; 
    std::vector<glm::mat4> transformStack;
    // glm::mat4 cameraTransform;
    // FT_Library ft;
    // FT_Face face;
    // void setTarget(Texture* target);
    void drawImageInternal(
        Texture* tex,
        glm::mat4 cameraTransform,
        glm::mat4 spriteTransform,
        glm::mat4 coordTransform
    );
    Texture* getTarget(){
        return renderTargetStack[renderTargetStack.size() - 1].second;
    }
    bool isAtRoot(){
        return renderTargetStack.size() == 1;
    }
    void setCurrentTransform(glm::mat4 transform){
        transformStack[transformStack.size()-1] = transform;
    }
};