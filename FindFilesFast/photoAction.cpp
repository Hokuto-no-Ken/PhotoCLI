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

void printMetadata(const fs::path& file_path) {
    try {

        if (!fs::exists(file_path)) {
            throw std::runtime_error("File does not exist: " + file_path.string());
        }


        auto image = Exiv2::ImageFactory::open(file_path.string());
        if (!image) {
            throw std::runtime_error("Could not open image file: " + file_path.string());
        }

        image->readMetadata();

        std::cout << "\nMetadata for: " << file_path.filename() << "\n";
        std::cout << "----------------------------------------\n";

        
        auto& exifData = image->exifData();
        if (exifData.empty()) {
            std::cout << "No EXIF metadata found.\n";
        }
        else {
            auto dateTime = exifData.findKey(Exiv2::ExifKey("Exif.Photo.DateTimeOriginal"));
            if (dateTime != exifData.end()) {
                std::cout << "Creation Date/Time: " << dateTime->toString() << "\n";
                std::cout << "----------------------------------------\n";
            }

            std::cout << "[EXIF DATA]\n";
            for (auto it = exifData.begin(); it != exifData.end(); ++it) {
                if (it->key() == "Exif.Photo.DateTimeOriginal") continue;

                std::cout << "  " << std::setw(35) << std::left
                    << it->key() << " = " << it->value() << "\n";
            }
        }
        std::cout << "----------------------------------------\n";
    }
    catch (const Exiv2::Error& e) {
        std::cerr << "Exiv2 error: " << e.what() << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}
void iptcprint(const fs::path& file_path) {
    auto image = Exiv2::ImageFactory::open(file_path.string());
    assert(image.get() != 0);
    image->readMetadata();
    Exiv2::IptcData& iptcData = image->iptcData();
    if (iptcData.empty()) {
        std::string error(file_path.string());
        error += ": No IPTC data found in the file";

    }
    std::cout << "\nMetadata for: " << file_path.filename() << "\n";
    std::cout << "----------------------------------------\n";
    Exiv2::IptcData::iterator end = iptcData.end();
    for (Exiv2::IptcData::iterator md = iptcData.begin(); md != end; ++md) {
       
        std::cout << std::setw(44) << std::setfill(' ') << std::left
            << md->key() << " "
            << "0x" << std::setw(4) << std::setfill('0') << std::right
            << std::hex << md->tag() << " "
            << std::setw(9) << std::setfill(' ') << std::left
            << md->typeName() << " "
            << std::dec << std::setw(3)
            << std::setfill(' ') << std::right
            << md->count() << "  "
            << std::dec << md->value()
            << std::endl;
    }
}

void iptcWrite(const fs::path& file_path) {
    Exiv2::IptcData iptcData;


    std::string input;
    std::string headline, keywords, dateCreated, timeSent, rasterizedCaption, unknownTag;
    uint16_t urgency, modelVersion;

    // Prompt for Headline
    std::cout << "Headline (A brief title or summary of the content): ";
    std::getline(std::cin, input);
    headline = input.empty() ? "None" : input;
    iptcData["Iptc.Application2.Headline"] = headline;

    // Prompt for Keywords
    std::cout << "Keywords (Descriptive words or phrases, comma-separated): ";
    std::getline(std::cin, input);
    keywords = input.empty() ? "None" : input;
    iptcData["Iptc.Application2.Keywords"] = keywords;

    // Prompt for Date Created
    std::cout << "Date Created (Format YYYY-MM-DD0 : ";
    std::getline(std::cin, input);
    dateCreated = input.empty() ? "0000-0-0" : input;
    iptcData["Iptc.Application2.DateCreated"] = dateCreated;

    // Prompt for Urgency
    std::cout << "Urgency (A number from 1 to 8, where 1 is most urgent) [default: 1]: ";
    std::getline(std::cin, input);
    try {
        urgency = input.empty() ? 1 : std::stoi(input);
        if (urgency < 1 || urgency > 8) {
            std::cerr << "Warning: Urgency must be between 1 and 8. Using default value 1." << std::endl;
            urgency = 1;
        }
    }
    catch (...) {
        std::cerr << "Warning: Invalid urgency input. Using default value 1." << std::endl;
        urgency = 1;
    }
    iptcData["Iptc.Application2.Urgency"] = urgency;

    // Prompt for Model Version
    std::cout << "Model Version (Envelope model version number) [default: 42]: ";
    std::getline(std::cin, input);
    try {
        modelVersion = input.empty() ? 42 : std::stoi(input);
    }
    catch (...) {
        std::cerr << "Warning: Invalid model version input. Using default value 42." << std::endl;
        modelVersion = 42;
    }
    iptcData["Iptc.Envelope.ModelVersion"] = modelVersion;

    // Prompt for Time Sent
    std::cout << "Time Sent (Format HH:MM:SS±HH:MM): ";
    std::getline(std::cin, input);
    timeSent = input.empty() ? "00:00:0-00:00" : input;
    iptcData["Iptc.Envelope.TimeSent"] = timeSent;

    // Prompt for Rasterized Caption
    std::cout << "Rasterized Caption (Byte sequence): ";
    std::getline(std::cin, input);
    rasterizedCaption = input.empty() ? "230 42 34 2 90 84 23 146" : input;
    iptcData["Iptc.Application2.RasterizedCaption"] = rasterizedCaption;

    // Prompt for Unknown Tag (Iptc.0x0009.0x0001)
    std::cout << "Custom text field: ";
    std::getline(std::cin, input);
    unknownTag = input.empty() ? "None" : input;
    iptcData["Iptc.0x0009.0x0001"] = unknownTag;

    Exiv2::StringValue value;
    value.read("very!");
    iptcData["Iptc.Application2.Urgency"] = value;
    std::cout << "Time sent: " << iptcData["Iptc.Envelope.TimeSent"] << "\n";
    // Open image file
    auto image = Exiv2::ImageFactory::open(file_path.string());
    assert(image.get() != 0);
    // Set IPTC data and write it to the file
    image->setIptcData(iptcData);
    image->writeMetadata();
}




void encryptPhotoMetaDate(const fs::path& filePath) {
    try {

        if (!fs::exists(filePath)) {
            throw std::runtime_error("File does not exist: " + filePath.string());
        }

        std::string key;
        std::cout << "Enter encryption key (any non-empty string): ";
        std::cin.ignore();
        std::getline(std::cin, key);
        if (key.empty()) {
            throw std::runtime_error("Encryption key cannot be empty");
        }

        auto image = Exiv2::ImageFactory::open(filePath.string());
        if (!image) {
            throw std::runtime_error("Failed to open image: " + filePath.string());
        }

        image->readMetadata();
        Exiv2::ExifData& exifData = image->exifData();
        Exiv2::IptcData& iptcData = image->iptcData();


        if (!exifData.empty()) {
            for (auto it = exifData.begin(); it != exifData.end(); ++it) {
                if (it->typeId() == Exiv2::asciiString || it->typeId() == Exiv2::string) {
                    std::string value = it->toString();
                    if (value.empty()) continue;

                    size_t keyIndex = 0;
                    for (char& c : value) {
                        c ^= key[keyIndex % key.length()];
                        keyIndex++;
                    }

                    exifData[it->key()] = value;
                }
            }
        }

        if (!iptcData.empty()) {
            for (auto it = iptcData.begin(); it != iptcData.end(); ++it) {
                if (it->typeId() == Exiv2::string || it->typeId() == Exiv2::asciiString) {
                    std::string value = it->toString();
                    if (value.empty()) continue;

                    size_t keyIndex = 0;
                    for (char& c : value) {
                        c ^= key[keyIndex % key.length()];
                        keyIndex++;
                    }
                    iptcData[it->key()] = value;
                }
            }
        }
        image->writeMetadata();
        std::cout << "Metadata successfully encrypted for: " << filePath.filename() << std::endl;

    }
    catch (const Exiv2::Error& e) {
        std::cerr << "Exiv2 error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void decryptPhotoMetaDate(const fs::path& filePath) {
    try {
        // Check if file exists
        if (!fs::exists(filePath)) {
            throw std::runtime_error("File does not exist: " + filePath.string());
        }

        std::string key;
        std::cout << "Enter decryption key: ";
        std::cin.ignore(); // Clear input buffer
        std::getline(std::cin, key);
        if (key.empty()) {
            throw std::runtime_error("Decryption key cannot be empty");
        }


        auto image = Exiv2::ImageFactory::open(filePath.string());
        if (!image) {
            throw std::runtime_error("Failed to open image: " + filePath.string());
        }

        image->readMetadata();

        Exiv2::ExifData& exifData = image->exifData();
        Exiv2::IptcData& iptcData = image->iptcData();


        if (!exifData.empty()) {
            for (auto it = exifData.begin(); it != exifData.end(); ++it) {
                if (it->typeId() == Exiv2::asciiString || it->typeId() == Exiv2::string) {
                    std::string value = it->toString();
                    if (value.empty()) continue;

                    size_t keyIndex = 0;
                    for (char& c : value) {
                        c ^= key[keyIndex % key.length()];
                        keyIndex++;
                    }
                    exifData[it->key()] = value;
                }
            }
        }

        // Decrypt IPTC metadata (string-based tags only)
        if (!iptcData.empty()) {
            for (auto it = iptcData.begin(); it != iptcData.end(); ++it) {
                if (it->typeId() == Exiv2::string || it->typeId() == Exiv2::asciiString) {
                    std::string value = it->toString();
                    if (value.empty()) continue;

                    size_t keyIndex = 0;
                    for (char& c : value) {
                        c ^= key[keyIndex % key.length()];
                        keyIndex++;
                    }
                    iptcData[it->key()] = value;
                }
            }
        }
        image->writeMetadata();
        std::cout << "Metadata successfully decrypted for: " << filePath.filename() << std::endl;

    }
    catch (const Exiv2::Error& e) {
        std::cerr << "Exiv2 error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void moveToAlbum(std::vector<fs::path>& found_files, int file_index) {
	const auto& selected_file = found_files[file_index - 1];
	std::string album_name;
	std::cout << "Enter album name: ";
	std::cin.ignore();
	std::getline(std::cin, album_name);
	fs::path album_path = fs::current_path() / "Albums" / album_name;
	if (!fs::exists(album_path)) {
		fs::create_directories(album_path);
	}
	fs::path destination = album_path / selected_file.filename();
	try {
		fs::copy(selected_file, destination);
		std::cout << "File moved to album: " << destination << '\n';
		deletePhoto(found_files, file_index);
	}
	catch (const fs::filesystem_error& e) {
		std::cerr << "Error moving file: " << e.what() << '\n';
	}
}

void copyToAlbum(std::vector<fs::path>& found_files, int file_index) {
    const auto& selected_file = found_files[file_index - 1];
    std::string album_name;
    std::cout << "Enter album name: ";
    std::cin.ignore();
    std::getline(std::cin, album_name);
    fs::path album_path = fs::current_path() / "Albums" / album_name;
    if (!fs::exists(album_path)) {
        fs::create_directories(album_path);
    }
    fs::path destination = album_path / selected_file.filename();
    try {
        fs::copy(selected_file, destination);
        std::cout << "File copied to album: " << destination << '\n';
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "Error copied file: " << e.what() << '\n';
    }
}

void organizePhotosByDate( std::vector<fs::path>& files) {
    std::cout << "\nОrganisation photoes by date\n";
    std::string album_name;
    std::getline(std::cin, album_name);
    fs::path album_path = fs::current_path() / "Albums" / album_name;
    std::cout << std::setw(50) << std::setfill('-') << "" << "\n";

    for ( auto& file_path : files) {
        try {
            // Open image file
            auto image = Exiv2::ImageFactory::open(file_path.string());
            assert(image.get() != 0);
            image->readMetadata();

            // Get date from Exif or file modification time
            std::string dateStr;
            Exiv2::ExifData& exifData = image->exifData();
            auto dateTime = exifData.findKey(Exiv2::ExifKey("Exif.Photo.DateTimeOriginal"));
            if (dateTime != exifData.end() && !dateTime->toString().empty()) {
                dateStr = dateTime->toString(); // e.g., "2025:06:16 14:30:00"
            }
            else {
                // Fallback to file modification time
                auto ftime = fs::last_write_time(file_path);
                auto time = std::chrono::system_clock::to_time_t(ftime);
                char buffer[20];
                std::strftime(buffer, sizeof(buffer), "%Y:%m:%d", std::localtime(&time));
                dateStr = buffer;
            }

            std::string year = dateStr.substr(0, 4);
            std::string month = dateStr.substr(5, 2);
            if (year.empty() || month.empty()) {
                continue;
            }
            fs::path targetDir = album_path / year / month;
            fs::create_directories(targetDir);

            fs::path targetPath = targetDir / file_path.filename();
            fs::rename(file_path, targetPath);
        }
        catch (const Exiv2::Error& e) {
            std::cerr << file_path.filename() << ": Exiv2 error: " << e.what() << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << file_path.filename() << ": Error idk: " << e.what() << "\n";
        }
    }
    std::cout << std::setw(50) << std::setfill('-') << "" << "\n";
    std::cout << "Organisation End !\n";
}


















