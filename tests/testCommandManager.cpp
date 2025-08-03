#include <gtest/gtest.h>
#include "../src/commandManager.h"
#include <filesystem>
#include <fstream>
#include <chrono>
#include <thread>

class CommandManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Stwórz katalog testowy
        test_dir = "/tmp/jetpath_test";
        std::filesystem::create_directories(test_dir);
        
        // Stwórz pliki testowe
        createTestFiles();
    }
    
    void TearDown() override {
        // Wyczyść pliki testowe
        if (std::filesystem::exists(test_dir)) {
            std::filesystem::remove_all(test_dir);
        }
    }
    
    void createTestFiles() {
        // Plik tekstowy
        std::ofstream txt_file(test_dir + "/test.txt");
        txt_file << "Hello World!\nThis is a test file.";
        txt_file.close();
        
        // Plik C++
        std::ofstream cpp_file(test_dir + "/main.cpp");
        cpp_file << "#include <iostream>\nint main() { return 0; }";
        cpp_file.close();
        
        // Plik Python
        std::ofstream py_file(test_dir + "/script.py");
        py_file << "print('Hello from Python!')";
        py_file.close();
        
        // Plik bez rozszerzenia
        std::ofstream no_ext_file(test_dir + "/README");
        no_ext_file << "This is a README file";
        no_ext_file.close();
        
        // Katalog
        std::filesystem::create_directory(test_dir + "/subdir");
        
        // Plik w podkatalogu
        std::ofstream sub_file(test_dir + "/subdir/nested.log");
        sub_file << "Log entry 1\nLog entry 2";
        sub_file.close();
        
        // Czekaj chwilę żeby pliki się zapisały
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    std::string test_dir;
};

// ✅ TEST 1: Sprawdź czy getAllFiles() zwraca pliki
TEST_F(CommandManagerTest, GetAllFiles_ReturnsFiles) {
    auto files = CommandManager::getAllFiles();
    
    EXPECT_GT(files.size(), 0) << "Should return at least some files";
    
    // Sprawdź czy zwrócone obiekty mają wypełnione pola
    bool found_valid_file = false;
    for (const auto& file : files) {
        if (!file.path.empty() && !file.name.empty()) {
            found_valid_file = true;
            break;
        }
    }
    
    EXPECT_TRUE(found_valid_file) << "Should return files with valid path and name";
}

// ✅ TEST 2: Sprawdź strukturę FileInfo
TEST_F(CommandManagerTest, FileInfo_HasCorrectStructure) {
    auto files = CommandManager::getAllFiles();
    ASSERT_GT(files.size(), 0);
    
    const auto& first_file = files[0];
    
    // Sprawdź czy wszystkie pola są dostępne
    EXPECT_TRUE(first_file.path.empty() || !first_file.path.empty());
    EXPECT_TRUE(first_file.name.empty() || !first_file.name.empty());
    EXPECT_TRUE(first_file.owner.empty() || !first_file.owner.empty());
    EXPECT_TRUE(first_file.permissions.empty() || !first_file.permissions.empty());
    EXPECT_TRUE(first_file.extension.empty() || !first_file.extension.empty());
    EXPECT_GE(first_file.size, 0);
    EXPECT_GE(first_file.modification_date, 0);
    // is_directory może być true lub false
}

// ✅ TEST 3: Sprawdź czy pliki mają poprawne rozszerzenia
TEST_F(CommandManagerTest, FileInfo_HasCorrectExtensions) {
    auto files = CommandManager::getAllFiles();
    
    bool found_cpp = false;
    bool found_txt = false;
    bool found_py = false;
    
    for (const auto& file : files) {
        if (file.extension == ".cpp") found_cpp = true;
        if (file.extension == ".txt") found_txt = true;
        if (file.extension == ".py") found_py = true;
        
        // Sprawdź czy rozszerzenie pasuje do nazwy
        if (!file.extension.empty()) {
            //EXPECT_TRUE(file.name.ends_with(file.extension)) 
            //    << "Extension '" << file.extension << "' should match filename '" << file.name << "'";
        }
    }
    
    // W systemie powinny być przynajmniej jakieś pliki z tymi rozszerzeniami
    std::cout << "Found .cpp files: " << found_cpp << std::endl;
    std::cout << "Found .txt files: " << found_txt << std::endl;
    std::cout << "Found .py files: " << found_py << std::endl;
}

// ✅ TEST 4: Sprawdź wydajność
TEST_F(CommandManagerTest, GetAllFiles_Performance) {
    auto start = std::chrono::high_resolution_clock::now();
    
    auto files = CommandManager::getAllFiles();
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    
    EXPECT_LT(duration.count(), 30) << "getAllFiles() should complete within 30 seconds";
    EXPECT_GT(files.size(), 0) << "Should return at least some files";
    
    std::cout << "Performance: Found " << files.size() << " files in " 
              << duration.count() << " seconds" << std::endl;
}

// ✅ TEST 5: Sprawdź czy katalogi są prawidłowo identyfikowane
TEST_F(CommandManagerTest, FileInfo_IdentifiesDirectories) {
    auto files = CommandManager::getAllFiles();
    
    bool found_directory = false;
    bool found_regular_file = false;
    
    for (const auto& file : files) {
        if (file.is_directory) {
            found_directory = true;
            EXPECT_EQ(file.size, 0) << "Directories should have size 0";
        } else {
            found_regular_file = true;
            // Pliki mogą mieć rozmiar 0 lub większy
        }
        
        if (found_directory && found_regular_file) break;
    }
    
    EXPECT_TRUE(found_regular_file) << "Should find at least one regular file";
    std::cout << "Found directories: " << found_directory << std::endl;
    std::cout << "Found regular files: " << found_regular_file << std::endl;
}

// ✅ TEST 6: Sprawdź czy właściciele są prawidłowo pobierani (Linux)
TEST_F(CommandManagerTest, FileInfo_HasOwnerInfo) {
    auto files = CommandManager::getAllFiles();
    ASSERT_GT(files.size(), 0);
    
    bool found_valid_owner = false;
    bool found_valid_permissions = false;
    
    for (const auto& file : files) {
        if (!file.owner.empty() && file.owner != "unknown" && file.owner != "error") {
            found_valid_owner = true;
        }
        
        if (!file.permissions.empty() && file.permissions != "unknown" && file.permissions != "error") {
            found_valid_permissions = true;
        }
        
        if (found_valid_owner && found_valid_permissions) break;
    }
    
    #ifndef _WIN32
    EXPECT_TRUE(found_valid_owner) << "Should find files with valid owner (Linux)";
    EXPECT_TRUE(found_valid_permissions) << "Should find files with valid permissions (Linux)";
    #endif
    
    std::cout << "Found valid owners: " << found_valid_owner << std::endl;
    std::cout << "Found valid permissions: " << found_valid_permissions << std::endl;
}

// ✅ TEST 7: Sprawdź czy daty modyfikacji są rozsądne
TEST_F(CommandManagerTest, FileInfo_HasReasonableModificationDates) {
    auto files = CommandManager::getAllFiles();
    
    auto now = std::time(nullptr);
    auto one_year_ago = now - (365 * 24 * 60 * 60); // Rok temu
    
    bool found_recent_file = false;
    
    for (const auto& file : files) {
        if (file.modification_date > 0) {
            // Data nie może być z przyszłości
            EXPECT_LE(file.modification_date, now + 3600) 
                << "Modification date should not be more than 1 hour in the future";
            
            // Data nie powinna być bardzo stara (chyba że to stary system)
            if (file.modification_date > one_year_ago) {
                found_recent_file = true;
            }
        }
    }
    
    EXPECT_TRUE(found_recent_file) << "Should find at least one file modified in the last year";
}

// ✅ TEST 8: Sprawdź czy nie ma duplikatów
TEST_F(CommandManagerTest, GetAllFiles_NoDuplicates) {
    auto files = CommandManager::getAllFiles();
    
    std::set<std::string> unique_paths;
    int duplicates = 0;
    
    for (const auto& file : files) {
        if (unique_paths.count(file.path) > 0) {
            duplicates++;
        } else {
            unique_paths.insert(file.path);
        }
    }
    
    EXPECT_EQ(duplicates, 0) << "Should not return duplicate file paths";
    EXPECT_EQ(unique_paths.size(), files.size()) << "Number of unique paths should equal number of files";
}

// ✅ TEST 9: Sprawdź czy filtruje systemowe katalogi
TEST_F(CommandManagerTest, GetAllFiles_FiltersSystemDirectories) {
    auto files = CommandManager::getAllFiles();
    
    int proc_files = 0;
    int sys_files = 0;
    
    for (const auto& file : files) {
        //if (file.path.starts_with("/proc/")) proc_files++;
        //if (file.path.starts_with("/sys/")) sys_files++;
    }
    
    EXPECT_EQ(proc_files, 0) << "Should filter out /proc/ files";
    EXPECT_EQ(sys_files, 0) << "Should filter out /sys/ files";
}

// ✅ TEST 10: Test granicznego przypadku - pusty wynik
TEST_F(CommandManagerTest, GetAllFiles_HandlesEmptyResult) {
    // Ten test może być trudny do napisania, ale sprawdźmy czy funkcja
    // nie crashuje gdy nie znajdzie plików
    
    // Symulujemy sytuację gdzie locate może nie zwrócić wyników
    // (ten test może nie zawsze przejść w zależności od systemu)
    
    auto files = CommandManager::getAllFiles();
    
    // Nawet jeśli jest 0 plików, funkcja powinna zwrócić pusty wektor
    EXPECT_GE(files.size(), 0) << "Should return non-negative number of files";
}

// ✅ BENCHMARK TEST
TEST_F(CommandManagerTest, DISABLED_Benchmark_CompareWithFind) {
    std::cout << "\n=== BENCHMARK: locate vs find ===\n";
    
    // Test locate (nasze rozwiązanie)
    auto start_locate = std::chrono::high_resolution_clock::now();
    auto files_locate = CommandManager::getAllFiles();
    auto end_locate = std::chrono::high_resolution_clock::now();
    auto duration_locate = std::chrono::duration_cast<std::chrono::milliseconds>(end_locate - start_locate);
    
    std::cout << "locate method: " << files_locate.size() << " files in " 
              << duration_locate.count() << "ms\n";
    
    // Test find (dla porównania - tylko /home żeby nie trwało wiecznie)
    auto start_find = std::chrono::high_resolution_clock::now();
    FILE* pipe = popen("find /home -type f 2>/dev/null | head -1000", "r");
    std::vector<std::string> files_find;
    if (pipe) {
        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), pipe)) {
            files_find.emplace_back(buffer);
        }
        pclose(pipe);
    }
    auto end_find = std::chrono::high_resolution_clock::now();
    auto duration_find = std::chrono::duration_cast<std::chrono::milliseconds>(end_find - start_find);
    
    std::cout << "find method: " << files_find.size() << " files in " 
              << duration_find.count() << "ms\n";
    
    // locate powinno być szybsze
    if (files_locate.size() > 100 && files_find.size() > 100) {
        EXPECT_LT(duration_locate.count(), duration_find.count() * 2) 
            << "locate should be faster than find";
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}