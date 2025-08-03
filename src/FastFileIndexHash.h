#ifndef FASTFILEINDEXHASH_H
#define FASTFILEINDEXHASH_H

#include <unordered_map>
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <algorithm>    // dla std::find
#include <filesystem>   // dla std::filesystem
#include <cstdint> 
#include <ctime>

#include "sizeCatergory.h"  

namespace{
    // ====== INDEXES BY =====

    // file name  , path with name
    std::unordered_map<std::string, std::vector<std::string>> filename_index; 

    // file extension
    std::unordered_map<std::string, std::vector<std::string>> extension_index;

    // file size with category
    std::unordered_map<FileSize, std::vector<std::string>> sizeByCategory_index;

    // file size
    std::unordered_map<uintmax_t , std::vector<std::string>> size_index;
    
    // modification date
    std::unordered_map<std::time_t , std::vector<std::string>> date_index;
    
    // key words
    std::unordered_map<std::string, std::vector<std::string>> keywords_index;
    
    // owner file
    std::unordered_map<std::string, std::vector<std::string>> owner_index;
    
    // permissions
    std::unordered_map<std::string, std::vector<std::string>> permissions_index;


    //-------------------------------------------------------
    //=======================================================
    //-------------------------------------------------------
    
    
    void addFileToIndex(const std::string name, const std::string file_path) {
        if (name.empty() || file_path.empty()) {
            std::cerr << "Error: File name or path is empty." << std::endl;
            return;
        }
        
        auto& file_list = filename_index[name];
        if (std::find(file_list.begin(), file_list.end(), file_path) != file_list.end()) {
            file_list.push_back(file_path);
        }
    }
    void addExtensionToIndex(const std::string name, const std::string file_path) {
        if (name.empty() || file_path.empty()) {
            //assert here 
        }
        
        auto& file_list = extension_index[name];
        if (std::find(file_list.begin(), file_list.end(), file_path) != file_list.end()) {
            file_list.push_back(file_path);
        }
    }
    void addSizeCategoryToIndex(uintmax_t file_size,const std::string file_path) {
        if (file_path.empty() ) {
            //assert here 
        }

        FileSize size_category = getSizeCategory(file_size);
        auto& file_list = sizeByCategory_index[size_category];
        if (std::find(file_list.begin(), file_list.end(), file_path) != file_list.end()) {
            file_list.push_back(file_path);
        }
    }
    void addSizeToIndex(uintmax_t file_size, const std::string file_path) {
        if (file_path.empty() ) {
            //assert here 
        }

        auto& file_list = sizeByCategory_index[file_size];
        if (std::find(file_list.begin(), file_list.end(), file_path) != file_list.end()) {
            file_list.push_back(file_path);
        }
    }
    
    
    
}




#endif