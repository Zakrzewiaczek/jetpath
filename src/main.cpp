#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <iostream>
#include <string>

using namespace ftxui;

int main() {
    // WYCZYŚĆ EKRAN NA POCZĄTKU
    system("clear");
    
    auto screen = ScreenInteractive::TerminalOutput();
    
    // Stan aplikacji
    std::string input_text = "";
    int counter = 0;
    bool quit = false;
    
    // Komponenty
    auto input = Input(&input_text, "Type something...");
    auto button_increment = Button("+", [&] { counter++; });
    auto button_decrement = Button("-", [&] { counter--; });
    auto button_quit = Button("Quit", [&] { 
        quit = true; 
        screen.ExitLoopClosure()(); 
    });
    
    // Layout komponentów
    auto buttons = Container::Horizontal({
        button_increment,
        button_decrement,
        button_quit,
    });
    
    auto main_container = Container::Vertical({
        input,
        buttons,
    });
    
    // USUŃ LOGIKĘ QUIT Z RENDERERA!
    auto renderer = Renderer(main_container, [&] {
        return vbox({
            // Nagłówek
            text("JetPath v1.0") | bold | center | color(Color::Cyan),
            separator(),
            
            // Informacje
            hbox({
                text("Counter: "),
                text(std::to_string(counter)) | bold | color(Color::Green),
            }),
            
            // Input
            hbox({
                text("Input: "),
                input->Render() | flex,
            }),
            
            // Wyświetl wprowadzony tekst
            hbox({
                text("You typed: "),
                text(input_text) | color(Color::Yellow),
            }),
            
            separator(),
            
            // Przyciski
            hbox({
                button_increment->Render(),
                separator(),
                button_decrement->Render(),
                separator(),
                button_quit->Render(),
            }) | center,
            
            separator(),
            
            // Instrukcje
            vbox({
                text("Instructions:") | bold,
                text("• Use +/- buttons to change counter"),
                text("• Type in the input field"),
                text("• Press 'Quit' to exit"),
                text("• Use Tab to navigate, Enter to activate"),
                text("• Press Ctrl+C to force exit"),
            }) | border,
            
        }) | border | center;
    });
    
    // Główna pętla aplikacji
    screen.Loop(renderer);
    
    // WYCZYŚĆ EKRAN PO WYJŚCIU
    system("clear");
    std::cout << "\nThanks for using JetPath!\n";
    std::cout << "Final counter value: " << counter << std::endl;
    std::cout << "Final input: " << input_text << std::endl;
    
    return 0;
}