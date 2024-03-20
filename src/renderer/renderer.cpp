#include "renderer.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "utils/fs.hpp"
#include "stb_image.h"
#include "stb_image_write.h"
#include "engine/engine.hpp"

const float quad[] = {
    1.0f, 1.0f, 0.0f,     1.0f, 1.0f, // top right
    1.0f, 0.0f, 0.0f,     1.0f, 0.0f, // bottom right
    0.0f, 1.0f, 0.0f,     0.0f, 1.0f, // top left

    1.0f, 0.0f, 0.0f,     1.0f, 0.0f, // bottom right
    0.0f, 1.0f, 0.0f,     0.0f, 1.0f, // top left
    0.0f, 0.0f, 0.0f,     0.0f, 0.0f, // bottom left
};

static const char* vertSrc = "#version 330 core\n"
    "in vec3 aPos;"
    "in vec2 aTexCoord;"
    "out vec2 TexCoord;"
    "uniform mat4 camera;"
    "uniform mat4 sprite;"
    "uniform mat4 coord;"
    "void main()"
    "{"
    "    gl_Position = camera * sprite * vec4(aPos, 1.0);"
    "    TexCoord = (coord * vec4(aTexCoord, 1.0, 1.0)).xy;"
    "}";

static const char* fragSrc = "#version 330 core\n"
    "out vec4 FragColor;"
    "in vec2 TexCoord;"
    "uniform sampler2D ourTexture;"
    "void main()"
    "{"
    "    FragColor = texture(ourTexture, TexCoord);"
    "}";

Renderer::~Renderer(){
    if(!initalized) return;
    initalized = false;
    // FT_Done_Face(face);
    // FT_Done_FreeType(ft);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteFramebuffers(1, &fbo); 
    glDeleteTextures(1, &fbt);
}
void Renderer::init(bool* hasError){
    if(*hasError) return;
    std::string vertexShaderSource = vertSrc; //readFile(hasError, "./vert.glsl");
    std::string fragmentShaderSource = fragSrc; //readFile(hasError, "./frag.glsl");
    if(*hasError) return;
    imageShader.initialize(hasError, vertexShaderSource, fragmentShaderSource);
    if(*hasError) return;
    setClearColor(0.0f, 0.0f, 0.0f);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    unsigned int positionLoc, textureLoc;
    // positionLoc = glGetAttribLocation(imageShader.id, "aPos");
    positionLoc = imageShader.getAttribLocation("aPos");
    // textureLoc = glGetAttribLocation(imageShader.id, "aTexCoord");
    textureLoc = imageShader.getAttribLocation("aTexCoord");
    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(positionLoc);
    glVertexAttribPointer(textureLoc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(textureLoc);
    setDimensions(engine.window.width, engine.window.height);

    // glGenFramebuffers(1, &fbo);
    // glBindFramebuffer(GL_FRAMEBUFFER, fbo);   
    // glGenTextures(1, &fbt);
    // glBindTexture(GL_TEXTURE_2D, fbt);
    
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbt, 0); 
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // textShader.initialize(hasError, 
    //     readFile(hasError, "./font_vert.glsl"), 
    //     readFile(hasError,  "./font_frag.glsl"));
    // FT_Library ft;
    // if (FT_Init_FreeType(&ft))
    // {
    //     std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    //     *hasError=true;
    //     return;
    // }

    // FT_Face face;
    // if (FT_New_Face(ft, "NormalFont.ttf", 0, &face))
    // {
    //     std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  
    //     *hasError=true;
    //     return;
    // }
    // FT_Done_Face(face);
    // FT_Done_FreeType(ft);
    clear();
    glfwSwapBuffers(engine.window.window);
    initalized = true;
}
void Renderer::setClearColor(float r, float g, float b){
    glClearColor(r, g, b, 1.0f);
}
void Renderer::clear(){
    getTarget()->useTarget();
    glClear(GL_COLOR_BUFFER_BIT);
}
void Renderer::startRender(){
    enabled = true;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    clear();
}
void Renderer::endRender(){
    enabled = false;
    // Draw root render target
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    Texture* tex = renderTargetStack[0].second;
    auto spriteTransform = glm::mat4(1.0f);
    spriteTransform = glm::scale(spriteTransform, glm::vec3(tex->width ,tex->height, 0.0f));
    drawImageInternal(
        tex,
        rootTransform,
        spriteTransform,
        glm::mat4(1.0f)  
    );
}
bool Renderer::isInitialized(){
    return initalized;
}
int Renderer::newTexture(int width, int height, unsigned char* data){
    auto ptr = new Texture(width, height, data);
    return textureStore.add(ptr);
}

int Renderer::loadImage(const char* path){
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 4); 
    if(!data) return -1;
    int id = newTexture(width, height, data);
    stbi_image_free(data);
    return id;
}

bool Renderer::saveTexture(const char* path, int id){
    Texture* tex = nullptr;
    if(id == -1) tex = renderTargetStack[0].second;
    else tex = textureStore.get(id);
    stbi_flip_vertically_on_write(id == -1);
    if(tex == nullptr) return false;
    auto data = (uint8_t*)calloc(tex->width * tex->height * 4, sizeof(uint8_t));
    glReadPixels(0, 0, tex->width, tex->height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_write_png(path, tex->width, tex->height, 4, data, 4 * tex->width);
    free(data);
    return true;
}

void Renderer::drawImage(int textureId, glm::mat4 spriteTransform, glm::mat4 coordTransform){
    getTarget()->useTarget();
    auto tex = textureStore.get(textureId);
    if(tex == nullptr){
        // TODO: actually handle this error
        return;
    }
    drawImageInternal(tex, 
        getTransform(),
        spriteTransform, 
        coordTransform);

}

void Renderer::freeTexture(int id){
    textureStore.del(id);
}

bool Renderer::pushRenderTarget(int id){
    // Returns false if the texture id is invalid, true otherwise.
    auto fb = getTexture(id);
    if(fb == nullptr) return false;
    renderTargetStack.push_back(std::make_pair(id, fb));
    // std::cout << "push\n";
    return true;
}

int Renderer::popRenderTarget(){
    // Returns -1 if stack is empty (actually contains one element, at the base framebuffer), a valid id otherwise.
    auto res = -1;
    auto size = renderTargetStack.size();
    // Stack must always contain at least one element, the root that is drawn to the window.
    if(size > 1){
        auto top = renderTargetStack[renderTargetStack.size()-1];
        res = top.first;
        renderTargetStack.pop_back();
    }
    return res;
}

int Renderer::newShader(const char* vertexSource, const char* fragmentSource){
    auto shader = new Shader();
    bool hasError;
    shader->initialize(&hasError, vertexSource, fragmentSource);
    if(hasError) return -1;
    return shaderStore.add(shader);
}
void Renderer::freeShader(int id){
    shaderStore.del(id);
}

Shader* Renderer::getShader(int id){
    return shaderStore.get(id);
}

int Renderer::newMesh(Shader* shader, const float* data, int vertexCount, int vertexSize, std::vector<std::pair<int,const char*>> attributes){
    Mesh* mesh = new Mesh(shader, data, vertexCount, vertexSize, attributes);
    return meshStore.add(mesh);
}
void Renderer::freeMesh(int id){
    meshStore.del(id);
}
Mesh* Renderer::getMesh(int id){
    return meshStore.get(id);
}

void Renderer::setDimensions(int width, int height){
    // If the root render target hasn't been set yet, create it
    // Otherwise resize and clear the root render target.
    if(renderTargetStack.size() == 0){
        int id = newTexture(width, height, NULL);
        pushRenderTarget(id);
        auto transform = glm::ortho(0.0f, (float)width, 0.0f, (float)height, -0.1f, 100.0f);
        transformStack.push_back(transform);
    }
    else{
        auto root = renderTargetStack[0];
        freeTexture(root.first);
        root.first = newTexture(width, height, NULL);
        root.second = getTexture(root.first);
        auto transform = glm::ortho(0.0f, (float)width, 0.0f, (float)height, -0.1f, 100.0f);
        transformStack[0] = transform;
    }
    // Reset the root transform.
    rootTransform = glm::ortho(0.0f, (float)width, float(height), 0.0f, -0.1f, 100.0f);
}

void Renderer::drawImageInternal(Texture* tex, 
    glm::mat4 baseTransform,
    glm::mat4 spriteTransform, 
    glm::mat4 coordTransform){
    tex->use();
    imageShader.use();
    glBindVertexArray(VAO);
    unsigned int cameraLoc, spriteLoc, coordLoc;
    // unsigned int cameraLoc = glGetUniformLocation(imageShader.id, "camera");
    cameraLoc = imageShader.getUniformLocation("camera");
    glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(baseTransform));
    // unsigned int spriteLoc = glGetUniformLocation(imageShader.id, "sprite");
    spriteLoc = imageShader.getUniformLocation("sprite");
    glUniformMatrix4fv(spriteLoc, 1, GL_FALSE, glm::value_ptr(spriteTransform));
    // unsigned int coordLoc = glGetUniformLocation(imageShader.id, "coord");
    coordLoc = imageShader.getUniformLocation("coord");
    glUniformMatrix4fv(coordLoc, 1, GL_FALSE, glm::value_ptr(coordTransform));
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
