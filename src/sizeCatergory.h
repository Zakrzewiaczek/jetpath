#ifndef SIZECATERGORY_H
#define SIZECATERGORY_H

#include <iostream>
#include <string>
#include <cstdint>
#include <algorithm>   
#include <filesystem>   
#include <cstdint> 


enum class FileSize {
EMPTY,       // 0 bytes
BYTE,        // 1 - 999 bytes
TINY,        // 1KB - 9KB
SMALL,       // 10KB - 99KB
MEDIUM_SMALL,// 100KB - 999KB
MEDIUM,      // 1MB - 9MB
MEDIUM_LARGE,// 10MB - 99MB
LARGE,       // 100MB - 999MB
HUGE,        // 1GB - 9GB
MASSIVE,     // 10GB - 99GB
GIGANTIC,    // 100GB - 999GB
TERABYTE     // >= 1TB
};



FileSize getSizeCategory(uintmax_t size_bytes) {
        if (size_bytes == 0) return FileSize::EMPTY;
        if (size_bytes < 1000) return FileSize::BYTE;                    // < 1KB
        if (size_bytes < 10 * 1024) return FileSize::TINY;              // 1-9KB
        if (size_bytes < 100 * 1024) return FileSize::SMALL;            // 10-99KB
        if (size_bytes < 1024 * 1024) return FileSize::MEDIUM_SMALL;    // 100KB-1MB
        if (size_bytes < 10 * 1024 * 1024) return FileSize::MEDIUM;     // 1-9MB
        if (size_bytes < 100 * 1024 * 1024) return FileSize::MEDIUM_LARGE; // 10-99MB
        if (size_bytes < 1024 * 1024 * 1024ULL) return FileSize::LARGE; // 100MB-1GB
        if (size_bytes < 10ULL * 1024 * 1024 * 1024) return FileSize::HUGE; // 1-9GB
        if (size_bytes < 100ULL * 1024 * 1024 * 1024) return FileSize::MASSIVE; // 10-99GB
        if (size_bytes < 1024ULL * 1024 * 1024 * 1024) return FileSize::GIGANTIC; // 100GB-1TB
        return FileSize::TERABYTE; // >= 1TB
    }



#endif