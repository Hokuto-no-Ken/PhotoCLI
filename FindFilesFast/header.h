#pragma once
#include <vector>
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <string>
#include <experimental/filesystem>
#include <iostream>
#include "CImg.h"
namespace fs = std::experimental::filesystem;

std::vector<fs::path> findFilesByExt(std::string constructed_path_str_dbg, std::string ext);

void printFindedFiles(std::vector<fs::path> found_files);

void openPhoto(std::vector<fs::path> &found_files, int file_index);

void deletePhoto(std::vector<fs::path> &found_files, int file_index);

void displayImageCImg(const std::vector<fs::path>& found_files, int file_index);

void resizeImage(const std::vector<fs::path>& found_files, int file_index, int new_width, int new_height);

void convertToGrayscale(const std::vector<fs::path>& found_files, int file_index);

void applyBlur(const std::vector<fs::path>& found_files, int file_index, float sigma);