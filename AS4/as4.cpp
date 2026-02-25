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

void DrawBoundedModel(raylib::Model& model, bool drawBB, auto transformer) {
    raylib::Transform backupTransform = model.transform;
    model.transform = transformer(backupTransform);
    model.Draw({});
    if(drawBB) model.GetTransformedBoundingBox().Draw();
    model.transform = backupTransform;
}

raylib::Degree angle_normalize(raylib::Degree angle) {
    float decimal = float(angle) - int(angle);
    int whole = int(angle) % 360; // [-360-360]
    whole += (angle < 0) * 360; // [0-360]
    return decimal + whole;
}

struct Entity {
    raylib::Vector3 position;
    float speed = 0;
    raylib::Degree heading = 0;

    raylib::Model* model;
    // bool is_selected;
};

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

    const float acceleration = 10;
    std::vector<Entity> entities; entities.reserve(3);

    entities.emplace_back(raylib::Vector3{0, 0, 0}, 0, 0, &penguin);
    auto& e = entities.emplace_back();
    e.position = raylib::Vector3{-100, 0, 0};
    e.model = &penguin;

    size_t selected = 0;
       
    while(!window.ShouldClose()) {
        window.BeginDrawing(); {
            window.ClearBackground(raylib::Color::RayWhite());
            float dt = window.GetFrameTime();

            if(raylib::Keyboard::IsKeyPressed(KEY_TAB)){
                // entities[selected].is_selected = false;
                selected = (selected + 1) % entities.size();
                // entities[selected].is_selected = true;
            }
                
            if(raylib::Keyboard::IsKeyPressed(KEY_W))
                entities[selected].speed += 50;
            if(raylib::Keyboard::IsKeyPressed(KEY_S))
                entities[selected].speed -= 50;
            if(raylib::Keyboard::IsKeyPressed(KEY_SPACE))
                entities[selected].speed = 0;

            camera.BeginMode(); {
                skybox.Draw();
                ground.Draw({});

                for(size_t i = 0; i < entities.size(); ++i) {
                    auto& e = entities[i];
                    auto velocity = raylib::Vector3{cos(e.heading.RadianValue()), 0, sin(e.heading.RadianValue())} * e.speed;
                    e.position += velocity * dt;

                    DrawBoundedModel(penguin, i == selected, [&e](raylib::Transform& transform){
                        return transform.Translate(e.position);
                    });
                }                

            } camera.EndMode();
            
            window.DrawFPS();
        } window.EndDrawing();
    }
}