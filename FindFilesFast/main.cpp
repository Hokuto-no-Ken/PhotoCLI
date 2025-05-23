#include "header.h"
#define cimg_use_png
int main() {
    std::string constructed_path_str_dbg = "C:\\Users\\Vitalii\\Downloads";
    std::string ext(".png");
    int w = 300;
    int h = 300;
    cimg_library::CImg<unsigned char> bg(w,h,1,3,255);
    const unsigned char blue[] = { 0,170,255 };
    bg.draw_circle(150, 150, 100, blue);
    bg.display();
    std::vector<fs::path> found_files = findFilesByExt(constructed_path_str_dbg, ext);

    while (true) {
        std::cout << "\nChoose action:\n";
        std::cout << "1. Open File\n";
        std::cout << "2. Delete File\n";
        std::cout << "3. Display Image (CImg)\n";
        std::cout << "4. Resize Image\n";
        std::cout << "5. Convert to Grayscale\n";
        std::cout << "6. Apply Blur\n";
        std::cout << "7. Exit\n";
        std::cout << "Please Enter: ";

        int choice;
        std::cin >> choice;
        if (choice == 7) {
            break;
        }

        if (choice >= 1 && choice <= 6) {
            std::cout << "Please enter file number (1-" << found_files.size() << "): ";
            int file_index;
            std::cin >> file_index;

            if (file_index < 1 || file_index > found_files.size()) {
                std::cout << "\n";
                std::cout << "U Fool!\n";
                continue;
            }

            try {
                if (choice == 1) {
                    openPhoto(found_files, file_index);
                }
                else if (choice == 2) {
                    deletePhoto(found_files, file_index);
                }
                else if (choice == 3) {
                    displayImageCImg(found_files, file_index);
                }
                else if (choice == 4) {
                    int new_width, new_height;
                    std::cout << "Enter new width: ";
                    std::cin >> new_width;
                    std::cout << "Enter new height: ";
                    std::cin >> new_height;
                    resizeImage(found_files, file_index, new_width, new_height);
                }
                else if (choice == 5) {
                    convertToGrayscale(found_files, file_index);
                }

                else if (choice == 6) {
                    float sigma;
                    std::cout << "Enter blur sigma (e.g., 2.0): ";
                    std::cin >> sigma;
                    applyBlur(found_files, file_index, sigma);
                }
            }
            catch (const cimg_library::CImgException& e) {
                std::cerr << "CImg Error: " << e.what() << '\n';
            }
            catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << '\n';
            }
        }
        else {
            std::cout << "Try again!\n";
        }
    }

    return 0;
}