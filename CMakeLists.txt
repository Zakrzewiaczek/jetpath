cmake_minimum_required(VERSION 3.14)
project(jetpath)

set(CMAKE_CXX_STANDARD 17)

# Pobierz FTXUI
include(FetchContent)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v4.1.1
)
FetchContent_MakeAvailable(ftxui)

# Stwórz aplikację jetpath
add_executable(jetpath src/main.cpp)

# Linkuj z FTXUI
target_link_libraries(jetpath PRIVATE
    ftxui::component
    ftxui::dom
    ftxui::screen
)