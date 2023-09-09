#include "renderer.hpp"
#include <iostream>
#include <string>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "fs/fs.hpp"
#include "stb_image.h"

const float quad[] = {
    1.0f, 1.0f, 0.0f,     1.0f, 1.0f, // top right
    1.0f, 0.0f, 0.0f,     1.0f, 0.0f, // bottom right
    0.0f, 1.0f, 0.0f,     0.0f, 1.0f, // top left

    1.0f, 0.0f, 0.0f,     1.0f, 0.0f, // bottom right
    0.0f, 1.0f, 0.0f,     0.0f, 1.0f, // top left
    0.0f, 0.0f, 0.0f,     0.0f, 0.0f, // bottom left
};

// std::string vertexShaderSource = "#version 330 core\n"
//     "layout (location = 0) in vec3 aPos;\n"
//     "void main()\n"
//     "{\n"
//     "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//     "}\0";
// std::string fragmentShaderSource = "#version 330 core\n"
//     "out vec4 FragColor;\n"
//     "void main()\n"
//     "{\n"
//     "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
//     "}\n\0";

Renderer::Renderer(){}
Renderer::~Renderer(){
    if(!initalized) return;
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // glDeleteProgram(shaderProgram);
    // for (size_t i = 0; i < textures.size(); i++)
    // {
    //     glDeleteTextures(1, &textures[i].idx);
    // }
    
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

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("character_0000.png", &width, &height, &nrChannels, 0); 
    
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    // glUniform1i(glGetUniformLocation(imageShader.id, "texture"), 0);
    
    initalized = true;
}
void Renderer::setClearColor(float r, float g, float b){
    glClearColor(r, g, b, 0.0f);
}
void Renderer::update(){
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);
    // draw our first triangle
    glUseProgram(imageShader.id);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
bool Renderer::isInitialized(){
    return initalized;
}
// unsigned int Renderer::newTexture(unsigned int width, unsigned int height){
//     TextureData data = {
//         .idx = 0,
//         .width = width,
//         .height = height
//     };
//     glGenTextures(1, &data.idx);
//     textures.emplace_back(data);
//     return textures.size() - 1;
// }