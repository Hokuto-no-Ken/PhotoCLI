#pragma once
#include <vector>
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <string>
#include <experimental/filesystem>
#include <iostream>
#include <exiv2/exiv2.hpp>
#include <iomanip>
#include <memory>
#include <fstream>

namespace fs = std::experimental::filesystem;

std::vector<fs::path> findFilesByExt(std::string constructed_path_str_dbg, std::string ext);

void printFindedFiles(std::vector<fs::path> found_files);

void openPhoto(std::vector<fs::path> &found_files, int file_index);

void deletePhoto(std::vector<fs::path> &found_files, int file_index);


void printMetadata(const fs::path& file_path);

void iptcprint(const fs::path& file_path);


void iptcWrite(const fs::path& file_path);

void encryptPhotoMetaDate(const fs::path& filePath);
void decryptPhotoMetaDate(const fs::path& filePath);

void moveToAlbum(std::vector<fs::path>& found_files, int file_index);
void copyToAlbum(std::vector<fs::path>& found_files, int file_index);
void organizePhotosByDate (std::vector<fs::path>& files);