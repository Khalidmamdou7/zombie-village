#pragma once

#include "../ecs/component.hpp"
#include "../shader/shader.hpp"

#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>

namespace our {

    // An enum that defines the type of the light (DIRECTIONAL, POINT or SPOT)
    enum class LightType {
        DIRECTIONAL,
        POINT,
        SPOT
    };

    // This component denotes that any renderer should draw the scene relative to this light.
    // We do not define the eye, center or up here since they can be extracted from the entity local to world matrix
    class LightComponent : public Component {
    public:
        LightType lightType; // The type of the light
        glm::vec3 color = glm::vec3(0, 0, 0); // The color of the light

        glm::vec3 attenuation = glm::vec3(0.0f, 0.0f, 0.0f); // The attenuation of the light for intensity
        glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);  //direction of light for Directional and Spot
        glm::vec2 cone_angles = glm::vec2(0.0f, 0.0f);  //cone angles for spot light

        static std::string getID() { return "Light"; }

        // Reads light parameters from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}

