#include "header.h"

std::vector<fs::path> findFilesByExt(std::string constructed_path_str_dbg, std::string ext) {
    std::vector<fs::path> found_files;
    for (auto& p : fs::recursive_directory_iterator(constructed_path_str_dbg)) {
        if (p.path().extension() == ext) {
            found_files.push_back(p.path());
            std::cout << found_files.size() << ": " << p.path() << '\n';
        }
    }


    if (found_files.empty()) {
        std::cout << "FIle " << ext << " error.\n";
    }
    return found_files;
}