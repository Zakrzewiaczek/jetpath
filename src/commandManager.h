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
    std::string group;
    std::string permissions;
    std::string extension;
    std::string mime_type;

    std::time_t modification_date;
    uintmax_t size;

    bool is_directory;
    bool is_symlink;
    bool is_hidden;
    bool is_executable;
    bool is_device;

    std::string symlink_target;

    uintmax_t inode;
    uintmax_t hard_links;
    uintmax_t device_id;
    uint32_t raw_mode;
};



class CommandManager {
}; 

#endif