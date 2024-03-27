#pragma once

#include <string>
#include <iostream>
#include <fstream>

inline bool DecodeRLE(const std::string& src_name, const std::string& dst_name) {
    std::ifstream in(src_name, std::ios::binary);
    if (!in) return false;

    std::ofstream out(dst_name, std::ios::binary);

    while (!in.eof()) {
        unsigned char header;
        in.read(reinterpret_cast<char*>(&header), 1);
        if (in.eof()) break; // Break if we've reached the end of the file

        bool isCompressed = header & 1; // Check the least significant bit to determine if the block is compressed
        int size = (header >> 1) + 1; // Extract the size of the block

        if (isCompressed) {
            char data;
            in.read(&data, 1); // Read the character to be repeated
            for (int i = 0; i < size; ++i) {
                out.put(data); // Repeat the character
            }
        }
        else {
            for (int i = 0; i < size; ++i) {
                char data;
                in.read(&data, 1); // Read and write the character directly
                out.put(data);
            }
        }
    }

    return true; // Return true if the decompression was successful
}