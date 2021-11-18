//
// Created by dawid on 18.11.2021.
//

#include "ImageSteganography.h"

void is::encode(const std::string &image_path, const std::string &file_path, const std::string &out_path) {
    cimg_library::CImg<unsigned char> image(image_path.c_str());
    std::vector<unsigned char> binary;

    load_file(file_path, binary);

    //  marks binary so that 'decode' will know where the encoded binary ends
    mark_end(binary);

    //  checks if binary fits inside image before proceeding
    if (!fits(file_path, image_path)) {
        std::cerr << "file does not fit in the image!\n";
        return;
    }

    //  goes through all the pixels encoding one bit inside each pixel's color least significant bit
    long byte = 0;
    for (int color = 0; color < 3;)
        for (int y = 0; y < image.height();)
            for (int x = 0; x < image.width();) {
                if (byte >= binary.size()) {
                    image.save(out_path.c_str());
                    return;
                }
                encode_byte(binary.at(byte++), image, x, y, color);
            }
}

/// @brief decodes a file inside given image_path
void is::decode(const std::string &image_path, const std::string &out_path) {
    cimg_library::CImg<unsigned char> image(image_path.c_str());
    std::vector<unsigned char> binary;

    //  goes through all the pixels and encoding one bit inside each pixel's color
    for (int color = 0; color < 3;) {
        for (int y = 0; y < image.height();) {
            for (int x = 0; x < image.width();) {
                if (decoded(binary)) {
                    save_file(out_path, binary);
                    return;
                }

                unsigned char data_byte = decode_byte(image, x, y, color);
                binary.emplace_back(data_byte);
            }
        }
    }
}

/// @returns image capacity in bytes
long is::capacity(const std::string &image_path) {
    cimg_library::CImg<unsigned char> image(image_path.c_str());

    //  width * height to get all the image pixels, * 3 to account for 3 colors in each pixel, /8 to convert to bytes
    return (image.width() * image.height() * 3) / 8;
}

/// @returns given file size in bytes
long is::size(const std::string &file_path) {
    std::ifstream bin(file_path, std::ios::in | std::ios::binary);
    bin.seekg(0, std::ios::end);
    long size = bin.tellg();
    bin.close();
    return size;
}

/// @returns whether the given file will fit inside the given image
bool is::fits(const std::string &file_path, const std::string &image_path) {
    cimg_library::CImg<unsigned char> image(image_path.c_str());
    //  the "+ 3" is for the marking at the end of the binary see is::mark_end()
    return capacity(image_path) >= (size(file_path) + 3);
}

/// @brief encodes one byte inside the given image
void is::encode_byte(unsigned char byte, cimg_library::CImg<unsigned char> &image, int &x, int &y, int &color) {
    //  should go through all bits in byte and encode each inside one pixel's color
    for (int bit = 7; bit >= 0; --bit) {
        //  "(input >> n) & 1" binary expression that gets the bit @n positions from byte @input
        bool data_bit = (byte >> bit) & 1;
        set_bit(image(x, y, color), 0, data_bit);

        //  jump to the next pixel
        if (++x >= image.width()) {
            x = 0;
            if (++y >= image.height()) {
                y = 0;
                color++;
            }
        }
    }
}

/// @brief decodes one byte inside the given image
unsigned char is::decode_byte(cimg_library::CImg<unsigned char> &image, int &x, int &y, int &color) {
    //  should go through all bits in byte and encode each inside one pixel's color
    unsigned char byte = '\0';
    for (int bit = 7; bit >= 0; --bit) {
        //  "(input >> n) & 1" binary expression that gets the bit @n positions from byte @input
        unsigned char last_bit = (image(x, y, color) >> 0) & 1;
        set_bit(byte, bit, last_bit);

        //  jump to the next pixel
        if (++x >= image.width()) {
            if (++y >= image.height()) {
                color++;
            }
        }
    }
    return byte;
}

/// @brief save binary file to the hard drive
void is::save_file(const std::string &file_path, std::vector<unsigned char> &binary) {
    std::ofstream bin(file_path, std::ios::out | std::ios::binary);
    for (auto &x: binary)
        bin.put((char) x);
    bin.close();
}

/// @brief load binary file into memory
void is::load_file(const std::string &file_path, std::vector<unsigned char> &binary) {
    std::ifstream bin(file_path, std::ios::in | std::ios::binary);
    binary.clear();

    char c;
    while (bin.get(c))
        binary.emplace_back(c);
    bin.close();
}

/// @brief marks the encrypted binary with a specific sequence of bits at the end
void is::mark_end(std::vector<unsigned char> &binary) {
    binary.emplace_back(85);
    binary.emplace_back(95);
    binary.emplace_back(255);
}

/**
 * @brief checks if the whole binary is already decoded
 * @return true if file is decoded
 */
bool is::decoded(std::vector<unsigned char> &binary) {
    //  mark_end is 3 bytes long so binary can't be shorter than that
    if (binary.size() < 3)
        return false;

    unsigned char first = binary.at(binary.size() - 1);
    unsigned char second = binary.at(binary.size() - 2);
    unsigned char third = binary.at(binary.size() - 3);
    if (first == 255 && second == 95 && third == 85) {
        //  remove mark_end from decoded binary
        binary.pop_back();
        binary.pop_back();
        binary.pop_back();
        return true;
    }
    return false;
}

/// @brief set bit @n from byte @c to @b
void is::set_bit(unsigned char &c, int n, bool b) {
    /**
     * c |= 1 << n sets @n bit to 1
     * c &= ~(1 << n) sets @n bit to 1
     */
    b ? c |= 1 << n : c &= ~(1 << n);
}

/// @brief writes byte to the terminal  @format "0000 0000"
void is::write_byte(const unsigned char &byte) {
    for (int bit = 7; bit >= 0; --bit) {
        //  divide into 2 columns of 4
        if (bit == 3)
            std::cout << ' ';
        //  "(input >> n) & 1" binary expression that gets the bit @n positions from byte @input
        std::cout << ((byte >> bit) & 1);
    }
}