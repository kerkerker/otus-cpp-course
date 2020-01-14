#include "lib.h"

#include <iostream>
#include <string_view>

int main(int argc, char **argv) {
    if (argc > 1 && std::string_view(argv[1]) == "--version") {
        std::cout << "Version: " << major_version() << '.' << minor_version() << '.'
                  << patch_version() << std::endl;
    } else {
        std::cout << "Hello, world!" << std::endl;
    }

    return 0;
}