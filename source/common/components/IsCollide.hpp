#pragma once

#include "../shader/shader.hpp"

#include <glm/mat4x4.hpp>

#include "../ecs/component.hpp"

#include <glm/glm.hpp>
namespace our {


    // This component denotes that any renderer should draw the scene relative to this light.
    // We do not define the eye, center or up here since they can be extracted from the entity local to world matrix
    class IsCollide : public Component {
    public:
        //For component raduis to calc
        float Raduis;
        //Id for Component is Collide or No
        static std::string getID() { return "IsCollide"; }

        // Reads Colliding parameters from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}

