#include "renderer.hpp"
#include <iostream>
#include <string>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "utils/fs.hpp"
#include "stb_image.h"

const float quad[] = {
    1.0f, 1.0f, 0.0f,     1.0f, 1.0f, // top right
    1.0f, 0.0f, 0.0f,     1.0f, 0.0f, // bottom right
    0.0f, 1.0f, 0.0f,     0.0f, 1.0f, // top left

    1.0f, 0.0f, 0.0f,     1.0f, 0.0f, // bottom right
    0.0f, 1.0f, 0.0f,     0.0f, 1.0f, // top left
    0.0f, 0.0f, 0.0f,     0.0f, 0.0f, // bottom left
};

Renderer::Renderer(){}
Renderer::~Renderer(){
    if(!initalized) return;
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}
void Renderer::init(bool* hasError){
    if(*hasError) return;
    std::string vertexShaderSource = readFile(hasError, "./vert.glsl");
    std::string fragmentShaderSource = readFile(hasError, "./frag.glsl");
    if(*hasError) return;
    imageShader.initialize(hasError, vertexShaderSource, fragmentShaderSource);
    if(*hasError) return;
    glClearColor( 0.4f, 0.3f, 0.4f, 0.0f );

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
    // intentionally flipped
    cameraTransform = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -0.1f, 100.0f);
    initalized = true;
}
void Renderer::setClearColor(float r, float g, float b){
    glClearColor(r, g, b, 0.0f);
}
void Renderer::update(){
    /* Render here */
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

void Renderer::drawImage(
    int textureId, 
    float x, float y, float width, float height,
    float sx, float sy, float sw, float sh)
{
    glUseProgram(imageShader.id);
    auto tex = textureStore.get(textureId);
    tex->use();
    glBindVertexArray(VAO);
    unsigned int cameraLoc = glGetUniformLocation(imageShader.id, "camera");
    glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, glm::value_ptr(cameraTransform));
    unsigned int spriteLoc = glGetUniformLocation(imageShader.id, "sprite");
    glm::mat4 spriteTransform = glm::mat4(1.0f);
    spriteTransform = glm::translate(spriteTransform, glm::vec3(x, y, 0.0f));
    spriteTransform = glm::scale(spriteTransform, glm::vec3(width, height, 0.0f));
    
    glUniformMatrix4fv(spriteLoc, 1, GL_FALSE, glm::value_ptr(spriteTransform));
    unsigned int dimensionsLoc = glGetUniformLocation(imageShader.id, "dimensions");
    glUniform4fv(dimensionsLoc, 1, glm::value_ptr(
        glm::vec4(sx/tex->width, sy/tex->height, sw/tex->width, sh/tex->height))
    );
    glDrawArrays(GL_TRIANGLES, 0, 6);
}