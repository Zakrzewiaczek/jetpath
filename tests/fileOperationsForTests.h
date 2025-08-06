#ifndef FILE_OPERATIONS_FOR_TESTS_H
#define FILE_OPERATIONS_FOR_TESTS_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>


void addDirectory(const std::string& dir_path, const std::string& folder_name) ;


void addFile(const std::string& file_path, std::string name) ;


void deleteFile(const std::string& file_path) ;




#endif