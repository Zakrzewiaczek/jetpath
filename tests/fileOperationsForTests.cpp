#include "fileOperationsForTests.h"

void addDirectory(const std::string& dir_path, const std::string& folder_name) {
    std::filesystem::path full_path = std::filesystem::path(dir_path) / folder_name;
    if (!std::filesystem::exists(full_path)) {
        std::filesystem::create_directories(full_path);
    }
}

void addFile(const std::string& file_path, std::string name) {
    std::filesystem::path full_path = std::filesystem::path(file_path) / name;
    std::filesystem::create_directories(full_path.parent_path());
    std::ofstream file(full_path);
    file << "test content";
    file.close();
}

void deleteFile(const std::string& file_path) {
    try {
        if (std::filesystem::exists(file_path)) {
            if (std::filesystem::is_directory(file_path)) {
                std::filesystem::remove_all(file_path);  
            } else {
                std::filesystem::remove(file_path);  
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error deleting: " << e.what() << std::endl;
    }
}
