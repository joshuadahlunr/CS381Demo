#include "AudioDevice.hpp"
#include "Camera3D.hpp"
#include "Keyboard.hpp"
#include "Matrix.hpp"
#include "Model.hpp"
#include "RadiansDegrees.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "raylib-cpp.hpp"
#include "raylib.h"
#include <concepts>
#include <iostream>
#include <limits>
#include <optional>
#include <memory>

#define SKYBOX_IMPLEMENTATION
#include "skybox.hpp"

void DrawBoundedModel(raylib::Model& model, auto transformer) {
    raylib::Transform backupTransform = model.transform;
    model.transform = transformer(backupTransform);
    model.Draw({});
    // Homework: draw a bounding box
    model.transform = backupTransform;
}

raylib::Degree angle_normalize(raylib::Degree angle) {
    float decimal = float(angle) - int(angle);
    int whole = int(angle) % 360; // [-360-360]
    whole += (angle < 0) * 360; // [0-360]
    return decimal + whole;
}

int main() {
    raylib::Window window(800, 600, "As0");
    window.SetState(FLAG_WINDOW_RESIZABLE);
    raylib::AudioDevice audio;

    raylib::Model penguin("models/penguin.glb");
    penguin.transform = raylib::Transform(penguin.transform).Scale(30).RotateY(raylib::Degree(90));
    raylib::Camera camera({0, 120, 500}, {0, 0, 0});

    raylib::Model ground = raylib::Mesh::Plane(10000, 10000, 50, 50, 25).LoadModelFrom();
    raylib::Texture snow("textures/snow.jpg");
    ground.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = snow;

    cs381::SkyBox skybox("textures/skybox.png");

    raylib::Vector3 position = {0, 0, 0};
    raylib::Vector3 velocity {0, 0, 0};

    // float timer = std::numeric_limits<float>::infinity();
    raylib::Degree heading = 0;
    float speed = 0, target_speed = 0;
    const float acceleration = 10;
       
    while(!window.ShouldClose()) {
        window.BeginDrawing(); {
            window.ClearBackground(raylib::Color::RayWhite());
            float dt = window.GetFrameTime();

            if(raylib::Keyboard::IsKeyPressed(KEY_W))
                target_speed += 50;
            if(raylib::Keyboard::IsKeyPressed(KEY_S))
                target_speed -= 50;
            if(raylib::Keyboard::IsKeyPressed(KEY_SPACE))
                target_speed = 0;

            if(speed < target_speed) speed += acceleration * dt;
            else if(speed > target_speed) speed -= acceleration * dt;

            velocity = raylib::Vector3{cos(heading.RadianValue()), 0, sin(heading.RadianValue())} * speed;
            position += velocity * dt;
            // timer -= dt;

            // bool w_down = raylib::Keyboard::IsKeyDown(KEY_W);
            // if(w_down && timer < 0) {
            //     velocity += {10, 0, 0};
            //     timer = 3;
            // }
        
            camera.BeginMode(); {
                skybox.Draw();
                ground.Draw({});

                DrawBoundedModel(penguin, [&position](raylib::Transform& transform){
                    return transform.Translate(position);
                });

            } camera.EndMode();
            
            window.DrawFPS();
        } window.EndDrawing();
    }
}