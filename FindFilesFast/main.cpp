#include "header.h"
#include <typeinfo>
#include <iomanip>
#ifdef _WIN32
#include <windows.h>
#endif

// Function to set up console for UTF-8 and colors (Windows-specific)
void setupConsole() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

// Function to set console text color (Windows-specific)
void setConsoleColor(int color) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
#else
    (void)color; // Suppress unused parameter warning for non-Windows
#endif
}

// Function to reset console color to default
void resetConsoleColor() {
#ifdef _WIN32
    setConsoleColor(7); // Default white on black
#endif
}

// Function to display a formatted menu
void displayMenu() {
    setupConsole();

    // Print header with green color
    setConsoleColor(10); // Light green
    std::cout << "\n" << std::setw(50) << std::setfill('=') << "" << std::endl;
    std::cout << std::setw(10) << "" << "  Photo CLI  " << std::setw(10) << "" << std::endl;
    std::cout << std::setw(50) << std::setfill('=') << "" << std::endl;
    resetConsoleColor();

    std::cout << std::setfill(' ');
    std::cout << "  ["; setConsoleColor(9); std::cout << "1"; resetConsoleColor(); std::cout << "] Open Photo\n";
    std::cout << "  ["; setConsoleColor(9); std::cout << "2"; resetConsoleColor(); std::cout << "] Delete Photo\n";
    std::cout << "  ["; setConsoleColor(9); std::cout << "3"; resetConsoleColor(); std::cout << "] Exchangeable Image File Format Read\n";
    std::cout << "  ["; setConsoleColor(9); std::cout << "4"; resetConsoleColor(); std::cout << "] International Press Telecommunications Council Read\n";
    std::cout << "  ["; setConsoleColor(9); std::cout << "5"; resetConsoleColor(); std::cout << "] International Press Telecommunications Council Write\n";
    std::cout << "  ["; setConsoleColor(9); std::cout << "6"; resetConsoleColor(); std::cout << "] Decrypt Photo MetaDate\n";
    std::cout << "  ["; setConsoleColor(9); std::cout << "7"; resetConsoleColor(); std::cout << "] Encrypt Photo MetaDate\n";
    std::cout << "  ["; setConsoleColor(9); std::cout << "8"; resetConsoleColor(); std::cout << "] Enter Album action: \n";
    std::cout << "  ["; setConsoleColor(9); std::cout << "9"; resetConsoleColor(); std::cout << "] Exit\n";
	std::cout << std::setw(50) << std::setfill('-') << "" << std::endl;
    setConsoleColor(22);
    std::cout << "\nEnter Action: ";
    resetConsoleColor();
}

int main() {
    std::string constructed_path_str_dbg = "C:\\Users\\Vitalii\\Downloads";
    std::string ext(".jpg");

    std::vector<fs::path> found_files = findFilesByExt(constructed_path_str_dbg, ext);

    // metadata  disk system       enc dec photo      --- organization to folder ---archive  about os    attributes read...

    while (true) {
        displayMenu(); // Use the formatted menu

        int choice,albumChoice;
        std::cin >> choice;
        if (choice == 9) {
            break;
        }

        if (choice >= 1 && choice <= 8) {
            std::cout << "Enter file number (1-" << found_files.size() << "): ";
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
                    decryptPhotoMetaDate(found_files[file_index - 1]);
                }
                else if (choice == 7) {
                    encryptPhotoMetaDate(found_files[file_index - 1]);
                }
                else if (choice == 8) {
                    std::cout << std::setfill('-');
                    std::cout << "  ["; setConsoleColor(9); std::cout << "1"; resetConsoleColor(); std::cout << "] Copy Photo to album\n";
                    std::cout << "  ["; setConsoleColor(9); std::cout << "2"; resetConsoleColor(); std::cout << "] Move Photo to album\n";
                    std::cout << "  ["; setConsoleColor(9); std::cout << "3"; resetConsoleColor(); std::cout << "] Auto album creation by date\n";
                    std::cin >> albumChoice;
                    if (albumChoice == 1) {
                        copyToAlbum(found_files, file_index);
                    }
                    else if(albumChoice == 2){
                        moveToAlbum(found_files, file_index);
                    }
					else if (albumChoice == 3) {
						organizePhotosByDate(found_files);
					}
					else {
						std::cout << "Invalid choice. Try again.\n";
					}
                   
                    
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