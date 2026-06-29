#include "readfile.hpp"
#include <fstream>
#include <iostream>

auto read_file(const std::string& filename) -> std::vector<u8> {
    std::ifstream stream(filename, std::ios::binary | std::ios::ate);
    if (!stream.good()) {
        std::cerr << "Erreur de lecture, impossible de lire " << filename << std::endl;
        return {};
    }

    const auto file_size = static_cast<size_t>(stream.tellg());

    std::vector<u8> data(file_size);
    stream.seekg(0, std::ios::beg);
    stream.read(reinterpret_cast<char*>(data.data()),
                static_cast<std::streamsize>(file_size));

    return data;
}
