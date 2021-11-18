#include "ImageSteganography.h"

int main(int argc, char *argv[]) {
    std::cout << "num of arguments: " << argc << std::endl;
    if (argc != 3) {
        std::cerr << "this program requires 2 arguments!\n";
        return 1;
    }

    std::string image_path = argv[1];
    std::string file_path = argv[2];
    std::cout << "image_path: " << image_path << std::endl;
    std::cout << "file_path: " << file_path << std::endl;

     is::encode(image_path, file_path);
     is::decode();

    return 0;
}
