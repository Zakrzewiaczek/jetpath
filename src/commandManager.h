#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H 

#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <ctime>
#include <chrono>       
#include <algorithm>     
#include <sys/stat.h>    
#include <pwd.h>         
#include <unistd.h> 


struct FileInfo {
    std::string name;
    std::string path;
    std::string owner;
    std::string permissions;
    std::string extension;

    std::time_t modification_date;
    uintmax_t size;
    bool is_directory;

};


class CommandManager {
private:
    static std::vector<FileInfo> executeCommand(const std::string& command) {
        std::vector<FileInfo> result;
        
        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) return result;
        
        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), pipe)) {
            std::string line(buffer);
            if (!line.empty() && line.back() == '\n') {
                line.pop_back();
            }
            
            if (!line.empty()) {
                FileInfo info;
                info.path = line;
                
                try {
                    if (std::filesystem::exists(line)) {
                        std::filesystem::path p(line);
                        
                        info.name = p.filename().string();
                        info.extension = p.extension().string();
                        info.is_directory = std::filesystem::is_directory(line);
                        
                        if (!info.is_directory) {
                            info.size = std::filesystem::file_size(line);
                            
                            auto ftime = std::filesystem::last_write_time(line);
                            auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                                ftime - std::filesystem::file_time_type::clock::now() + 
                                std::chrono::system_clock::now()
                            );
                            info.modification_date = std::chrono::system_clock::to_time_t(sctp);
                        } else {
                            info.size = 0;
                            info.modification_date = 0;
                        }
                        
                        #ifndef _WIN32
                        struct stat file_stat;
                        if (stat(line.c_str(), &file_stat) == 0) {
                            struct passwd* pw = getpwuid(file_stat.st_uid);
                            info.owner = pw ? pw->pw_name : std::to_string(file_stat.st_uid);

                            char perms[10];
                            sprintf(perms, "%o", file_stat.st_mode & 0777);
                            info.permissions = perms;
                        }
                        #else
                        info.owner = "unknown";
                        info.permissions = "unknown";
                        #endif
                        
                    } else {
                        // Plik nie istnieje - tylko ścieżka
                        std::filesystem::path p(line);
                        info.name = p.filename().string();
                        info.extension = p.extension().string();
                        info.owner = "unknown";
                        info.permissions = "unknown";
                        info.size = 0;
                        info.modification_date = 0;
                        info.is_directory = false;
                    }
                    
                } catch (const std::exception& e) {
                    // W przypadku błędu - tylko podstawowe dane
                    std::filesystem::path p(line);
                    info.name = p.filename().string();
                    info.extension = p.extension().string();
                    info.owner = "error";
                    info.permissions = "error";
                    info.size = 0;
                    info.modification_date = 0;
                    info.is_directory = false;
                }
                
                result.push_back(info);
            }
        }
        
        pclose(pipe);
        return result;
    }
    
    static void executeCommandVoid(const std::string& command) {
        std::system(command.c_str());
    }

public:
    static std::vector<FileInfo> getAllFiles() {

        //std::cout << "🥇 Using FASTEST method: locate\n";
        executeCommandVoid("updatedb"); 
        return executeCommand("locate '*' 2>/dev/null | grep -v '/proc/' | grep -v '/sys/'");
    }
}; 

#endif