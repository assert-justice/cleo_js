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
#include "mesh.hpp"

class Renderer{
    public:
    // Engine utilities
    ~Renderer();
    void init(bool* hasError);
    void startRender();
    void endRender();
    bool isInitialized();
    bool isEnabled(){
        return enabled || !isAtRoot();
    }
    void setClearColor(float r, float g, float b);
    void setDimensions(int width, int height);
    // Direct rendering
    void clear();
    void drawImage(
        int textureId,
        glm::mat4 spriteTransform,
        glm::mat4 coordTransform
    );
    // Textures
    int newTexture(int width, int height, unsigned char* data);
    void freeTexture(int id);
    int loadImage(const char* path);
    bool saveTexture(const char* path, int id);
    Texture* getTexture(int id){
        return textureStore.get(id);
    }
    bool pushRenderTarget(int id);
    int popRenderTarget();
    // Shaders
    int newShader(const char* vertexSource, const char* fragmentSource);
    void freeShader(int id);
    Shader* getShader(int id);
    // Meshes
    int newMesh(Shader* shader, const float* data, int vertexCount, int vertexSize, std::vector<std::pair<int,const char*>> attributes);
    void freeMesh(int id);
    Mesh* getMesh(int id);
    // Transforms
    void setTransform(glm::mat4 transform){
        setCurrentTransform(transform);
    }
    glm::mat4 getTransform(){
        return transformStack[transformStack.size()-1];
    }
    void pushTransform(){
        // duplicates the top of the transform stack
        transformStack.push_back(getTransform());
    }
    void popTransform(){
        if(transformStack.size() > 0) transformStack.pop_back();
    }
    void translate(glm::vec3 vector){
        setCurrentTransform(glm::translate(getTransform(), vector));
    }
    void scale(glm::vec3 vector){
        setCurrentTransform(glm::scale(getTransform(), vector));
    }
    void rotate(float angle, glm::vec3 axis){
        setCurrentTransform(glm::rotate(getTransform(), angle, axis));
    }
    void setOrthoProjection(float left, float right, float top, float bottom, float near, float far){
        setCurrentTransform(glm::ortho(left, right, bottom, top, near, far));
    }
    void setPerspectiveProjection(float fov, float aspect, float near, float far){
        setCurrentTransform(glm::perspective(fov, aspect, near, far));
    }
    void setIdentity(){
        setCurrentTransform(glm::mat4(1.0f));
    }
    private:
    Store<Texture> textureStore;
    Store<Shader> shaderStore;
    Store<Mesh> meshStore;
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