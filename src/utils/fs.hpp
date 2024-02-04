#include <string>

std::string readFile(bool* hasError, const char* path);
void writeFile(bool* hasError, const char* path, std::string text);

std::string getExtension(const char* path);

std::string getSavePath(std::string appname);