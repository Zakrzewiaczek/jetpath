#ifndef FASTFILEINDEXHASH_H
#define FASTFILEINDEXHASH_H

#include <unordered_map>
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <algorithm> 
#include <filesystem>   
#include <cstdint> 
#include <ctime>

#include "sizeCatergory.h"  
#include "commandManager.h"

enum class indexBy {
        by_filename,
        by_extension,
        by_size_category,
        by_size,
        by_date,
        by_keywords,
        by_owner,
        by_permissions
    };


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
    
}



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

    auto& file_list = size_index[file_size];
    if (std::find(file_list.begin(), file_list.end(), file_path) != file_list.end()) {
        file_list.push_back(file_path);
    }
}
void addDateToIndex(std::time_t modification_date, const std::string file_path) {
    if (file_path.empty() ) {
        //assert here 
    }

    auto& file_list = date_index[modification_date];
    if (std::find(file_list.begin(), file_list.end(), file_path) != file_list.end()) {
        file_list.push_back(file_path);
    }
}
//void addKeywordsToIndex(const std::string keyword, const std::string file_path) soon

void addOwnerToIndex(const std::string owner, const std::string file_path) {
    if (owner.empty() || file_path.empty()) {
        //assert here 
    }

    auto& file_list = owner_index[owner];
    if (std::find(file_list.begin(), file_list.end(), file_path) != file_list.end()) {
        file_list.push_back(file_path);
    }
}

void addPermissionsToIndex(const std::string permissions, const std::string file_path){
    if (permissions.empty() || file_path.empty()) {
        //assert here 
    }

    auto& file_list = permissions_index[permissions];
    if (std::find(file_list.begin(), file_list.end(), file_path) != file_list.end()) {
        file_list.push_back(file_path);
    }
}


//====== getters ======

std::vector<std::string> getPathByFileName(const std::string name) {
    return filename_index[name];

}

std::vector<std::string> getPathByExtension(const std::string extension) {
    return extension_index[extension];
}

std::vector<std::string> getPathBySizeCategory(FileSize size_category) {
    return sizeByCategory_index[size_category];
}

std::vector<std::string> getPathBySize(uintmax_t size) {
    return size_index[size];
}

std::vector<std::string> getPathByDate(std::time_t date) {
    return date_index[date];
}

std::vector<std::string> getPathByOwner(const std::string owner) {
    return owner_index[owner];
}

std::vector<std::string> getPathByPermissions(const std::string permissions) {
    return permissions_index[permissions];
}
/*
template <typename T>
void addAllIndexes(indexBy indexType) {
    if(indexType==by_filename){
        std::vector<FileInfo> files = getAllFiles();
        for (const auto& file : files) {
            addFileToIndex(file.name, file.path);
        }

    }
    else if(indexType==by_extension){
        std::vector<FileInfo> files = getAllFiles();
        for (const auto& file : files) {
            addExtensionToIndex(file.extension, file.path);
        }
    }
    else if(indexType==by_size_category){
        std::vector<FileInfo> files = getAllFiles();
        for (const auto& file : files) {
            addSizeCategoryToIndex(file.size, file.path);
        }
    }
    else if(indexType==by_size){
        std::vector<FileInfo> files = getAllFiles();
        for (const auto& file : files) {
            addSizeToIndex(file.size, file.path);
        }
    }
    else if(indexType==by_date){
        std::vector<FileInfo> files = getAllFiles();
        for (const auto& file : files) {
            addDateToIndex(file.modification_date, file.path);
        }
    }
    else if(indexType==by_owner){
        std::vector<FileInfo> files = getAllFiles();
        for (const auto& file : files) {
            addOwnerToIndex(file.owner, file.path);
        }
    }
    else if(indexType==by_permissions){
        std::vector<FileInfo> files = getAllFiles();
        for (const auto& file : files) {
            addPermissionsToIndex(file.permissions, file.path);
        }
    }
}
*/

#endif