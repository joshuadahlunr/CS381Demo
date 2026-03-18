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
#include <vector>

#define SKYBOX_IMPLEMENTATION
#include "skybox.hpp"

extern size_t globalComponentCounter;
template<typename T>
size_t GetComponentID(/* T reference = {} */) {
    static size_t id = globalComponentCounter++;
    return id;
}

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

using entity = size_t;

struct ComponentStorageBase {
    virtual ~ComponentStorageBase() {}
    virtual size_t element_size() { return 0; }

    virtual void* Get(size_t index) = 0;
    virtual void* GetOrAllocate(size_t index) = 0;
};

template<typename Tcomponent>
struct ComponentStorage: public ComponentStorageBase, std::vector<Tcomponent> {
    using std::vector<Tcomponent>::vector;

    size_t element_size() override { return sizeof(Tcomponent); }

    void* Get(size_t index) override { 
        return &this->at(index);
    }

    void* GetOrAllocate(size_t index) override {
        if(this->size() <= index)
            this->resize(index + 1);
        return Get(index);
    }
};

struct Context {
    std::vector<std::vector<bool>> entityMasks;
    std::vector<std::shared_ptr<ComponentStorageBase>> storages = {nullptr};

    template<typename Tcomponent>
    ComponentStorageBase& GetStorage() {
        size_t id = GetComponentID<Tcomponent>();
        if(storages.size() <= id)
            storages.insert(
                storages.end(), 
                std::max<int64_t>(id - storages.size(), 1), 
                nullptr
            );
        if(!storages[id] || storages[id]->element_size() == 0)
            storages[id] = std::make_shared<ComponentStorage<Tcomponent>>();
        return *storages[id];
    }

    entity CreateEntity() {
        entity e = entityMasks.size();
        entityMasks.emplace_back(std::vector<bool>{false});
        return e;
    }

    // EC Homework: how do we remove entities?

    template<typename Tcomponent>
    Tcomponent& AddComponent(entity e) {
        size_t id = GetComponentID<Tcomponent>();
        auto& mask = entityMasks[e];
        if(mask.size() <= id)
            mask.resize(id + 1, false);
        mask[id] = true;
        return *(Tcomponent*)GetStorage<Tcomponent>().GetOrAllocate(e);
    }

    template<typename Tcomponent>
    Tcomponent& GetComponent(entity e) {
        size_t id = GetComponentID<Tcomponent>();
        assert(HasComponent<Tcomponent>(e));
        return *(Tcomponent*)GetStorage<Tcomponent>().Get(e);
    }

    template<typename Tcomponent>
    bool HasComponent(entity e) {
        size_t id = GetComponentID<Tcomponent>();
        return entityMasks.size() > e && entityMasks[e].size() > id && entityMasks[e][id];
    }
};

void Draw(Context& ctx) {
    for(entity e = 0; e < ctx.entityMasks.size(); ++e) {
        if(!ctx.HasComponent<DrawInfo>(e)) continue;

        // Draw or something
    }
}

// struct Entities {
//     enum Type {
//         Penguin = 0,
//         Eagle = 1,
//         Frog = 2,
//         Camera = 3,
//     } type;

//     std::vector<raylib::Vector3> position = {0, 0, 0};
//     std::vector<raylib::Vector3> velocity {0, 0, 0};

//     std::vector<raylib::Camera> camera;

//     // float timer = std::numeric_limits<float>::infinity();
//     std::vector<raylib::Degree> heading;
//     std::vector<float> speed, target_speed;
// };

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

    // Add things to entity.
    // std::vector<Entity> entities;
    // auto& e = entities.emplace_back();


    const float acceleration = 10;
    
       
    while(!window.ShouldClose()) {
        window.BeginDrawing(); {
            window.ClearBackground(raylib::Color::RayWhite());
            float dt = window.GetFrameTime();

            // for(auto& e: entities) {

            //     if(raylib::Keyboard::IsKeyPressed(KEY_W))
            //         e.target_speed += 50;
            //     if(raylib::Keyboard::IsKeyPressed(KEY_S))
            //         e.target_speed -= 50;
            //     if(raylib::Keyboard::IsKeyPressed(KEY_SPACE))
            //         e.target_speed = 0;

            //     if(e.speed < e.target_speed) e.speed += acceleration * dt;
            //     else if(e.speed > e.target_speed) e.speed -= acceleration * dt;

            //     e.velocity = raylib::Vector3{cos(e.heading.RadianValue()), 0, sin(e.heading.RadianValue())} * e.speed;
            //     e.position += e.velocity * dt;

            // }
            // timer -= dt;

            // bool w_down = raylib::Keyboard::IsKeyDown(KEY_W);
            // if(w_down && timer < 0) {
            //     velocity += {10, 0, 0};
            //     timer = 3;
            // }
        
            camera.BeginMode(); {
                skybox.Draw();
                ground.Draw({});

                // for(auto& e: entities) {
                //     // -> continue
                //     if(e.type != Entity::Camera)
                //         continue;

                //     DrawBoundedModel(penguin, [&e](raylib::Transform& transform){
                //         return transform.Translate(e.position);
                //     });
                    
                // }

                // -> break;

            } camera.EndMode();
            
            window.DrawFPS();
        } window.EndDrawing();
    }
}