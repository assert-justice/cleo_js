#include <iostream>
#include "engine/engine.hpp"
#include "utils/cli.hpp"

#define MAJOR 0
#define MINOR 0
#define PATCH 0

int main(int argc, const char** argv){
    Cli cli(argc, argv);
    cli.next();
    if(cli.match("-h") || cli.match("--help")){
        std::cout << "Help text" << std::endl;
        return 0;
    }
    if(cli.match("-v") || cli.match("--version")){
        std::cout << "Cleo version " << MAJOR << "." << MINOR << "." << PATCH << std::endl;
        return 0;
    }
    const char* path = "./main.js";
    if(cli.hasArgs()) path = cli.next().c_str();
    engine.init(path);
    return 0;
}

// void println(qjs::rest<std::string> args) {
//     for (auto const & arg : args) std::cout << arg << " ";
//     std::cout << "\n";
// }

// void setInit(std::function<void()> fn){
//     fn();
// }

// int main(){
//     qjs::Runtime runtime;
//     qjs::Context context(runtime);
//     GLFWwindow* window;
//     if(!glfwInit()){
//         std::cout << "Failed to init glfw" << std::endl;
//         return -1;
//     }
//     window = glfwCreateWindow(800, 600, "hi glfw", NULL, NULL);
//     if (window == NULL){
//         std::cout << "Failed to create GLFW window" << std::endl;
//         glfwTerminate();
//         return -1;
//     }
//     glfwMakeContextCurrent(window);
//     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//     {
//         std::cout << "Failed to initialize GLAD" << std::endl;
//         return -1;
//     }
//     else{
//     }
//     glClearColor( 0.4f, 0.3f, 0.4f, 0.0f );
//     try{
//         auto& module = context.addModule("MyModule");
//         module.function<&println>("println");
//         module.function<&setInit>("setInit");
//         context.evalFile("./main.js", JS_EVAL_TYPE_MODULE);
//     }
//     catch(qjs::exception){
//         auto exc = context.getException();
//         std::cerr << (std::string) exc << std::endl;
//         if((bool) exc["stack"])
//             std::cerr << (std::string) exc["stack"] << std::endl;
//         return 1;
//     }
//     while (!glfwWindowShouldClose(window))
//     {
//         /* Render here */
//         glClear(GL_COLOR_BUFFER_BIT);

//         /* Swap front and back buffers */
//         glfwSwapBuffers(window);

//         /* Poll for and process events */
//         glfwPollEvents();
//     }

//     glfwTerminate();
//     return 0;
// }