#include "file_tools.h"

#include <filesystem>

namespace fs = std::filesystem;

void check_for_directory(const char *directory)
{
    if(!fs::exists(directory))
    {
        if(fs::create_directory(directory))
        {
            std::cout << "Directory " << directory << " has been created" << std::endl;
        }
        else
        {
            std::cerr << "Failed to create directory " << directory << std::endl;
        }
    }
}

void check_for_file(const char *path)
{
    if(!fs::exists(path))
    {
        std::ofstream file(path);
        if(file)
        {
            std::cout << "File has been created in " << path << std::endl;
        }
        else
        {
            std::cerr << "Failed to create file in " << path << std::endl;
        }
    }
}
