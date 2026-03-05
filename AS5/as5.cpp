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
#include <functional>
#include <iostream>
#include <limits>
#include <optional>
#include <memory>
#include <vector>

#include <BufferedRaylib.hpp>

#define SKYBOX_IMPLEMENTATION
#include "skybox.hpp"


// struct StateMachine {
//     enum State {
//         Idle,
//         Move,
//         Jump,
//         Fall,
//     } state = Idle;

//     void update() {
//         switch(state) {
//         break; case Idle:
//             input();
//         break; case Move:
//             // if(left_pressed) move left
//             // if(right_pressed) move right
//             input();
//         break; case Jump:
//             input();
//         break; case Fall:
//             // if(left_pressed) move left
//             // if(right_pressed) move right
//             input();
//         }
//     }
//     void input() {
//         switch (state) {
//         break; case Idle:
//             animation_player.playing != "land";
//             if(left_pressed) state = Move;
//             if(right_pressed) state = Move;
//             if(space_pressed) state = Jump;
//         break; case Move:
//             if(!left_pressed && !right_pressed) state = Idle;
//             if(space_pressed) state = Jump;
//         break; case Jump:
//             if(jump_animation_finished) state = Fall;
//         break; case Fall:
//             if(on_ground) {
//                 state = Idle;
//                 animation_player.play("land");
//             }
//         }
//     }
// };

class Component {
public:
    struct Entity* attached;

    virtual void Setup() = 0;
    virtual void Update(float dt) = 0;
    virtual void Cleanup() = 0;
};

struct Entity {
    std::vector<std::shared_ptr<Component>> components;

    template<std::derived_from<Component> T>
    T& AddComponent() {
        auto out = components.emplace_back(std::make_shared<T>());
        out->attached = this;
        return (T&)*out;
    }

    template<std::derived_from<Component> T>
    std::optional<std::reference_wrapper<T>> GetComponent() {
        for(auto& component: components) {
            T* cast = dynamic_cast<T*>(component.get());
            if(cast) return *cast;
        }

        return {};
        // throw std::runtime_error("Component Not Attached");
    }
};

struct TransformComponent : public Component {
    raylib::Vector3 position = {0, 0, 0};
    raylib::Quaternion rotation = raylib::Quaternion::Identity();

    void Setup() override {}
    void Update(float dt) override {}
    void Cleanup() override {}
};

struct DrawModelComponent : public Component {
    raylib::Model* model;

    void Setup() override {}
    void Update(float dt) override {
        if(auto t = attached->GetComponent<TransformComponent>(); t) {
            auto& transform = t->get();
            raylib::Transform backupTransform = model->transform;
            model->transform = raylib::Transform(model->transform)
                .Translate(transform.position)
                .Rotate(transform.rotation);
            model->Draw({});
            // Homework: draw a bounding box
            model->transform = backupTransform;
        }
    }
    void Cleanup() override {}
};

template<typename T>
struct Delegate {};

template<typename Treturn, typename... Targs>
struct Delegate<Treturn(Targs...)> {
    std::vector<std::function<Treturn(Targs...)>> functions;

    void connect(const std::function<Treturn(Targs...)>& f) {
        functions.push_back(f);
    }

    Treturn operator() (Targs... args) {
        static_assert(std::is_same_v<Treturn, void>, "EC Homework: Only void return allowed");
        for(auto& f: functions) {
            f(std::forward<Targs>(args)...);
        }
    }
};

int main() {
    Delegate<void(int)> d;
    d.connect([](int) {

    });
    d(5);

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

    raylib::BufferedInput input;
    input["jump"] = raylib::Action::key(KEY_SPACE).AddPressedCallback([]() {
        std::cout << "Pressed" << std::endl;
    }).move();

    input["jump"] = raylib::Action::key(KEY_P);

    input["move"] = raylib::Action::button_axis(
        {raylib::Button::key(KEY_W), raylib::Button::key(KEY_UP)},
        {raylib::Button::key(KEY_S), raylib::Button::key(KEY_DOWN)}
    ).AddCallback([](float value, float change) {
        std::cout << "Move: " << value << " - " << change << std::endl;
    }).move();

    // Add things to entity.
    std::vector<Entity> entities;
    auto& e = entities.emplace_back();
    auto& transform = e.AddComponent<TransformComponent>();
    transform.position = raylib::Vector3{-200, 0, 0};
    auto& model = e.AddComponent<DrawModelComponent>();
    model.model = &penguin;

    // setup
    for(Entity& e: entities) {
        for(std::shared_ptr<Component>& c: e.components) {
            c->Setup();
        }
    }
       
    while(!window.ShouldClose()) {
        input.PollEvents();

        // auto state = input["move"].data.vector.last_state;
        // std::cout << state.x << ", " << state.y << std::endl;

        window.BeginDrawing(); {
            window.ClearBackground(raylib::Color::RayWhite());
            float dt = window.GetFrameTime();
        
            camera.BeginMode(); {
                skybox.Draw();
                ground.Draw({});

                // Update
                for(Entity& e: entities) {
                    for(std::shared_ptr<Component>& c: e.components) {
                        c->Update(dt);
                    }
                }

            } camera.EndMode();
            
            window.DrawFPS();
        } window.EndDrawing();
    }

    for(Entity& e: entities) {
        for(std::shared_ptr<Component>& c: e.components) {
            c->Cleanup();
        }
    }
}