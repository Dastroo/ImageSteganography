//
// Created by dawid on 14.11.2021.
//

#pragma once

//  not using cimg to display anything
#define cimg_display 0

#include "CImg.h"
#include <fstream>
#include <iostream>

namespace is {
    /// encodes a binary file inside a image
    void encode(const std::string &image_path, const std::string &file_path,
                              const std::string &out_path = "assets/encoded_image.png");

    /// @brief decodes a file inside given image_path
    void decode(const std::string &image_path="assets/encoded_image.png", const std::string &out_path = "assets/decoded_file");

    /// @returns image capacity in bytes
    long capacity(const std::string &image_path);

    /// @returns given file size in bytes
    long size(const std::string &file_path);

    /// @returns whether the given file will fit inside the given image
    bool fits(const std::string &file_path, const std::string &image_path);

    /// @brief encodes one byte inside the given image
    void encode_byte(unsigned char byte, cimg_library::CImg<unsigned char> &image, int &x, int &y, int &color);

    /// @brief decodes one byte inside the given image
    unsigned char decode_byte(cimg_library::CImg<unsigned char> &image, int &x, int &y, int &color);

    /// @brief save binary file to the hard drive
    void save_file(const std::string &file_path, std::vector<unsigned char> &binary);

    /// @brief load binary file into memory
    void load_file(const std::string &file_path, std::vector<unsigned char> &binary);

    /// @brief marks the encrypted binary with a specific sequence of bits at the end
    void mark_end(std::vector<unsigned char> &binary);

    /**
     * @brief checks if the whole binary is already decoded
     * @return true if file is decoded
     */
    bool decoded(std::vector<unsigned char> &binary);

    /// @brief set bit @n from byte @c to @b
    void set_bit(unsigned char &c, int n, bool b);

    /// @brief writes byte to the terminal  @format "0000 0000"
    void write_byte(const unsigned char &byte);
}