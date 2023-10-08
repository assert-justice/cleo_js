#include "renderer.hpp"
#include <iostream>
#include <string>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "utils/fs.hpp"
#include "stb_image.h"
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
    // glClearColor( 0.4f, 0.3f, 0.4f, 0.0f );

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    unsigned int positionLoc, textureLoc;
    positionLoc = glGetAttribLocation(imageShader.id, "aPos");
    textureLoc = glGetAttribLocation(imageShader.id, "aTexCoord");
    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(positionLoc);
    glVertexAttribPointer(textureLoc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(textureLoc);
    setCameraPosition(0.0f, 0.0f);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);   
    glGenTextures(1, &fbt);
    glBindTexture(GL_TEXTURE_2D, fbt);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbt, 0); 
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    clear();
    glfwSwapBuffers(engine.window.window);
    initalized = true;
}
void Renderer::setClearColor(float r, float g, float b, float a){
    glClearColor(r, g, b, a);
}
void Renderer::clear(){
    glClear(GL_COLOR_BUFFER_BIT);
}
void Renderer::update(){
    glClear(GL_COLOR_BUFFER_BIT);
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
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0); 
    if(!data) return -1;
    int id = newTexture(width, height, data);
    stbi_image_free(data);
    return id;
}

void Renderer::drawImage(int textureId, glm::mat4 spriteTransform, glm::mat4 coordTransform){
    glUseProgram(imageShader.id);
    auto tex = textureStore.get(textureId);
    if(tex == nullptr){
        // TODO: actually handle this error
        return;
    }
    tex->use();
    glBindVertexArray(VAO);
    unsigned int cameraLoc = glGetUniformLocation(imageShader.id, "camera");
    glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(cameraTransform));
    unsigned int spriteLoc = glGetUniformLocation(imageShader.id, "sprite");
    glUniformMatrix4fv(spriteLoc, 1, GL_FALSE, glm::value_ptr(spriteTransform));
    unsigned int coordLoc = glGetUniformLocation(imageShader.id, "coord");
    glUniformMatrix4fv(coordLoc, 1, GL_FALSE, glm::value_ptr(coordTransform));
    // spriteTransform = glm::translate(spriteTransform, glm::vec3(x, y, 0.0f));
    // spriteTransform = glm::scale(spriteTransform, glm::vec3(width, height, 0.0f));
    
    // unsigned int dimensionsLoc = glGetUniformLocation(imageShader.id, "dimensions");
    // glUniform4fv(dimensionsLoc, 1, glm::value_ptr(
    //     glm::vec4(sx/tex->width, sy/tex->height, sw/tex->width, sh/tex->height))
    // );
    glDrawArrays(GL_TRIANGLES, 0, 6);

}

// void Renderer::drawImage(
//     int textureId, 
//     float x, float y, float width, float height,
//     float sx, float sy, float sw, float sh)
// {
//     glUseProgram(imageShader.id);
//     auto tex = textureStore.get(textureId);
//     tex->use();
//     glBindVertexArray(VAO);
//     unsigned int cameraLoc = glGetUniformLocation(imageShader.id, "camera");
//     glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(cameraTransform));
//     unsigned int spriteLoc = glGetUniformLocation(imageShader.id, "sprite");
//     glm::mat4 spriteTransform = glm::mat4(1.0f);
//     spriteTransform = glm::translate(spriteTransform, glm::vec3(x, y, 0.0f));
//     spriteTransform = glm::scale(spriteTransform, glm::vec3(width, height, 0.0f));
    
//     glUniformMatrix4fv(spriteLoc, 1, GL_FALSE, glm::value_ptr(spriteTransform));
//     unsigned int dimensionsLoc = glGetUniformLocation(imageShader.id, "dimensions");
//     glUniform4fv(dimensionsLoc, 1, glm::value_ptr(
//         glm::vec4(sx/tex->width, sy/tex->height, sw/tex->width, sh/tex->height))
//     );
//     glDrawArrays(GL_TRIANGLES, 0, 6);
// }

void Renderer::freeTexture(int id){
    textureStore.del(id);
}

void Renderer::setCameraPosition(float x, float y){
    // intentionally flipped
    float top = (float)engine.window.height;
    float bottom = 0.0f;
    if(target != nullptr){
        bottom = top;
        top = 0.0f;
    }
    // cameraTransform = glm::ortho(0.0f, camWidth, camHeight, 0.0f, -0.1f, 100.0f);
    cameraTransform = glm::ortho(0.0f, (float)engine.window.width, top, bottom, -0.1f, 100.0f);
    cameraTransform = glm::translate(cameraTransform, glm::vec3(-x, -y, 0.0f));
}

void Renderer::setTarget(Texture* target){
    this->target = target;
    if(target == nullptr) glBindFramebuffer(GL_FRAMEBUFFER, 0);
    else{
        target->useTarget();
    }
    setCameraPosition(0,0);
}