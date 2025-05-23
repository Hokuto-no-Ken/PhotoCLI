#include "header.h"

void openPhoto(std::vector<fs::path>& found_files, int file_index) {
    const auto& selected_file = found_files[file_index - 1];
    std::string command = "start \"\" \"" + selected_file.string() + "\"";
    system(command.c_str());
    std::cout << "Open File: " << selected_file << '\n';
    printFindedFiles(found_files);
}

void deletePhoto(std::vector<fs::path>& found_files, int file_index) {
    const auto& selected_file = found_files[file_index - 1];
    try {
        if (fs::remove(selected_file)) {
            std::cout << "Deleted File: " << selected_file << '\n';
            found_files.erase(
                std::remove(found_files.begin(), found_files.end(), selected_file),
                found_files.end()
            );
            printFindedFiles(found_files);
        }
        else {
            std::cout << "Deleting File Error: " << selected_file << '\n';
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
}



void displayImageCImg(const std::vector<fs::path>& found_files, int file_index) {
    const auto& selected_file = found_files[file_index - 1];
    const char* vvv = selected_file.string().c_str();
    cimg_library::CImg<unsigned char> image(vvv);
    image.display("Image Viewer");
    std::cout << "Displayed File: " << selected_file << '\n';
}

void resizeImage(const std::vector<fs::path>& found_files, int file_index, int new_width, int new_height) {
    const auto& selected_file = found_files[file_index - 1];
    cimg_library::CImg<unsigned char> image(selected_file.string().c_str());
    image.resize(new_width, new_height);
    fs::path output_path = selected_file.parent_path() / (selected_file.stem().string() + "_resized" + selected_file.extension().string());
    image.save_png(output_path.string().c_str());
    std::cout << "Resized File saved as: " << output_path << '\n';
    printFindedFiles(found_files);
}

void convertToGrayscale(const std::vector<fs::path>& found_files, int file_index) {
    const auto& selected_file = found_files[file_index - 1];
    cimg_library::CImg<unsigned char> image(selected_file.string().c_str());
    std::cout << image;
    image.RGBtoYCbCr().channel(0); // Convert to YCbCr and keep luminance channel
    fs::path output_path = selected_file.parent_path() / (selected_file.stem().string() + "_grayscale" + selected_file.extension().string());
    image.save_png(output_path.string().c_str());
    std::cout << "Grayscale File saved as: " << output_path << '\n';
    printFindedFiles(found_files);
}

void applyBlur(const std::vector<fs::path>& found_files, int file_index, float sigma) {
    const auto& selected_file = found_files[file_index - 1];
    cimg_library::CImg<unsigned char> image(selected_file.string().c_str());
    image.blur(sigma);
    fs::path output_path = selected_file.parent_path() / (selected_file.stem().string() + "_blurred" + selected_file.extension().string());
    image.save_png(output_path.string().c_str());
    std::cout << "Blurred File saved as: " << output_path << '\n';
    printFindedFiles(found_files);
}