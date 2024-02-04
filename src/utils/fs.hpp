#include <string>

std::string readFile(bool* hasError, const char* path);

void writeFile(bool* hasError, const char* path, std::string text);

std::string getSavePath(std::string appname);

std::string getExtension(std::string path);

char getPathSeparator();

bool createDirectory(std::string path);
