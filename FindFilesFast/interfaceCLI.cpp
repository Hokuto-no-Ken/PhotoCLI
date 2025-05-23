#include "header.h"

void printFindedFiles(std::vector<fs::path> found_files) {
    for (int i = 0; i <= found_files.size() - 1;i++) {
        std::cout << i + 1 << ": " << found_files[i] << ": " << '\n';
    }
}