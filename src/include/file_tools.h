#ifndef _FILE_TOOLS_
#define _FILE_TOOLS_

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

void check_for_directory(const char *directory);

void check_for_file(const char *path);

template <typename T>
void write_vector_file(std::string filename, std::vector<T>& data);

#endif // !_FILE_TOOLS_

template <typename T>
inline void write_vector_file(std::string filename, std::vector<T> &data)
{
    std::ofstream out;
    out.open(filename, std::ios::out);

    if(!out.is_open()) {
        std::cout << "Error open file: " << filename << std::endl;
        return;
    }

    out << data.size() << std::endl << std::endl;

    for (auto& var : data) {
        out << var << std::endl;
    }

    out.close();
    std::cout << "File has been written: " << filename << std::endl;
    return;
}