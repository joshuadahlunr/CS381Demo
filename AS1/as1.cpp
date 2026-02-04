#include "AudioDevice.hpp"
#include "raylib-cpp.hpp"
#include <concepts>
#include <iostream>
#include <optional>

// Callback
auto PingButton() -> void {
    std::cout << "Ping" << std::endl;
}

#define GUI_VOLUMECONTROL_IMPLEMENTATION
#include "VolumeControl.h"

template<typename T>
concept calls_with_no_arguments = requires(T t) {
    { t() };
};

template<calls_with_no_arguments Tfunction>
void call_lambda(Tfunction func) {
    func();
}

int main() {
    raylib::Window window(300, 350, "As0");
    raylib::AudioDevice audio;
    
    auto gui_state = InitGuiVolumeControl();

    raylib::Sound ping("audio/ping.wav");
    raylib::Music music("audio/price-of-freedom.mp3");
    music.Play();

    std::optional<int> x = 5;
    if(x) {
        std::cout << x.value_or(0) << std::endl;
    }

    // size_t counter = 0;
    // call_lambda([&counter](int) {
    //     std::cout << counter << std::endl;
    // });

    while(!window.ShouldClose()) {
        window.BeginDrawing(); {
            window.ClearBackground(raylib::Color::RayWhite());
        
            GuiVolumeControl(&gui_state);
        } window.EndDrawing();

        music.Update();
        // counter++;
        // count();
    }
}