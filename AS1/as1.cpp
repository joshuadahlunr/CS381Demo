#include "AudioDevice.hpp"
#include "Matrix.hpp"
#include "RadiansDegrees.hpp"
#include "Vector4.hpp"
#include "raylib-cpp.hpp"
#include <concepts>
#include <iostream>
#include <optional>
#include <memory>

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

// Heap access "anywhere"
// Stack is access bottom

int main() {
    raylib::Window window(300, 350, "As0");
    raylib::AudioDevice audio;
    
    auto gui_state = InitGuiVolumeControl();

    raylib::Sound ping("audio/ping.wav");
    raylib::Music music("audio/price-of-freedom.mp3");
    music.Play();

    auto A = raylib::Transform::CreateScale(3, 3, 3);
    auto B = raylib::Transform::CreateRotateY(raylib::Degree(270));

    auto transform = B * A;
    raylib::Vector4 up = {0, 1, 0};
    // transform * up; // HOW DO?

    std::optional<int> x = 5;
    if(x) {
        std::cout << x.value_or(0) << std::endl;
    }

    int* p = new int;
    delete p;

    // std::unique_ptr<int> p2 = std::make_unique<int>(0);
    // std::unique_ptr<int> p3 = std::move(p2);
    std::shared_ptr<int> p2 = std::make_shared<int>(0);
    std::shared_ptr<int> p3 = p2;

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