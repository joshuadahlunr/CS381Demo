#include "raylib-cpp.hpp"

std::array<int, 2> x() { 
    return {1, 2};
}

int main() {
    raylib::Window window(800, 600, "As0");
    window.SetState(FLAG_WINDOW_RESIZABLE);

    std::array<int, 2> array2;
    {
        auto array = x();
        array2 = std::move(array);
    }

    raylib::Text text;
    text.SetText("Hello World");
    text.SetFontSize(20);
    text.SetSpacing(2);
    text.SetColor(raylib::Color::LightGray());

    while(!window.ShouldClose()) {
        window.BeginDrawing(); {
            window.ClearBackground(raylib::Color::RayWhite());
            text.Draw({400, 300});
        } window.EndDrawing();
    }
}