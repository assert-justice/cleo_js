#include "renderer.hpp"
#include <iostream>
#include <string>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

const float quad[] = {
    1.0f, 1.0f, 0.0f,     1.0f, 1.0f, // top right
    1.0f, 0.0f, 0.0f,     1.0f, 0.0f, // bottom right
    0.0f, 1.0f, 0.0f,     0.0f, 1.0f, // top left

    1.0f, 0.0f, 0.0f,     1.0f, 0.0f, // bottom right
    0.0f, 1.0f, 0.0f,     0.0f, 1.0f, // top left
    0.0f, 0.0f, 0.0f,     0.0f, 0.0f, // bottom left
};

std::string vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
std::string fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

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
    imageShader.initialize(hasError, vertexShaderSource, fragmentShaderSource);
    if(*hasError) return;
    glClearColor( 0.4f, 0.3f, 0.4f, 0.0f );
    // // build and compile our shader program
    // // ------------------------------------
    // // vertex shader
    // unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // glCompileShader(vertexShader);
    // // check for shader compile errors
    // int success;
    // char infoLog[512];
    // glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    // if (!success)
    // {
    //     glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    //     std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    // }
    // // fragment shader
    // unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // glCompileShader(fragmentShader);
    // // check for shader compile errors
    // glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    // if (!success)
    // {
    //     glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    //     std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    // }
    // // link shaders
    // shaderProgram = glCreateProgram();
    // glAttachShader(shaderProgram, vertexShader);
    // glAttachShader(shaderProgram, fragmentShader);
    // glLinkProgram(shaderProgram);
    // // check for linking errors
    // glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    // if (!success) {
    //     glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    //     std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    // }
    // else{
    //     std::cout << "Shader compiled!" << std::endl;
    // }
    // glDeleteShader(vertexShader);
    // glDeleteShader(fragmentShader);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    initalized = true;
}
void Renderer::setClearColor(float r, float g, float b){
    glClearColor(r, g, b, 0.0f);
}
void Renderer::update(){
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);
    // draw our first triangle
    glUseProgram(imageShader.program);
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