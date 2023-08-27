#include "renderer.hpp"

Renderer::Renderer(){}
Renderer::~Renderer(){
    if(!initalized) return;
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