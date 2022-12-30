#include "light.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <asset-loader.hpp>

namespace our
{

    // Reads light parameters from the given json object
    void LightComponent::deserialize(const nlohmann::json &data)
    {
        if(!data.is_object()) return;

        std::string lightTypestr = data.value("lightType", "Directional");

        if (lightTypestr == "Directional")
        {
            lightType = LightType::DIRECTIONAL;
        }
        else if (lightTypestr == "Point")
        {
            lightType = LightType::POINT;
        }
        else if (lightTypestr == "Spot")
        {
            lightType = LightType::SPOT;
        }
        color = glm::vec3(data["color"][0], data["color"][1], data["color"][2]);
        attenuation = glm::vec3(data["attenuation"][0], data["attenuation"][1], data["attenuation"][2]);
        cone_angles = glm::vec2(glm::radians((float)data["cone_angles"][0]), glm::radians((float)data["cone_angles"][1]));
        direction = glm::vec3(data["direction"][0], data["direction"][1], data["direction"][2]);
    }

}
