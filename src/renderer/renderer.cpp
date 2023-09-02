#include "renderer.hpp"

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
    for (size_t i = 0; i < textures.size(); i++)
    {
        glDeleteTextures(1, &textures[i].idx);
    }
    
}
void Renderer::init(bool* hasError){
    if(*hasError) return;
    glClearColor( 0.4f, 0.3f, 0.4f, 0.0f );
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
unsigned int Renderer::newTexture(unsigned int width, unsigned int height){
    TextureData data = {
        .idx = 0,
        .width = width,
        .height = height
    };
    glGenTextures(1, &data.idx);
    textures.emplace_back(data);
    return textures.size() - 1;
}