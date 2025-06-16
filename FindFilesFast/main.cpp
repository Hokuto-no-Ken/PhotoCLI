#include "header.h"

int main() {
    std::string constructed_path_str_dbg = "C:\\Users\\Vitalii\\Downloads";
    std::string ext(".jpg");

    std::vector<fs::path> found_files = findFilesByExt(constructed_path_str_dbg, ext);

	// metadata  disk system       enc dec photo      --- organization to folder ---archive  about os    attributes read...

    while (true) {

        std::cout << "\nChoose action:\n";
        std::cout << "1. Open File\n";
        std::cout << "2. Delete File\n";
        std::cout << "3. Exchangeable Image File Format Read \n";
        std::cout << "4  International Press Telecommunications Council Read \n";
        std::cout << "5  International Press Telecommunications Council Write \n";
        std::cout << "6. Dec \n";
        std::cout << "7. Enc \n";
        std::cout << "8. Exit\n";
        std::cout << "Please Enter: ";

        int choice;
        std::cin >> choice;
        if (choice == 8) {
            break;
        }

        if (choice >= 1 && choice <= 7) {
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
                    printMetadata(found_files[file_index - 1]);
                }
                else if (choice == 4) {
                    iptcprint(found_files[file_index - 1]);
				}
				else if (choice == 5) {

                    iptcWrite(found_files[file_index - 1]);
				}
                else if (choice == 6) {
                    encryptPhotoMetaDate(found_files[file_index - 1]);
                }
                else if (choice == 7) {
                    decryptPhotoMetaDate(found_files[file_index - 1]);
                }


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