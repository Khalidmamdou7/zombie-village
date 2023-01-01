#include "light.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <asset-loader.hpp>
#include "../ecs/component.hpp"
#include "../deserialize-utils.hpp"

#include <iostream>

namespace our
{

    // reading the light data from json file
    void LightComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())//if it is not an object asln
            return;

        //default value to everything read
        lightTypeStr = data.value("typeOfLight", "DIRECTIONAL");
        std::cout << "extracted light type: " << lightTypeStr << std::endl; 

        //set light type (integer) according to string read from the json file
        if (lightTypeStr == "DIRECTIONAL")
            lightType = LightType::DIRECTIONAL;

        else if (lightTypeStr == "POINT")
            lightType = LightType::POINT;

        else if (lightTypeStr == "SPOT")
            lightType = LightType::SPOT;

        // reading the color attribute of the light
        color = glm::vec3(data.value("color", glm::vec3(1, 1, 1)));

        // reading the direction vector
        direction = glm::vec3(data.value("direction", glm::vec3(0, -1, 1)));

        //reading the diffuse vector
        diffuse = glm::vec3(data.value("diffuse", glm::vec3(1, 1, 1)));

        //reading the specular vector
        specular = glm::vec3(data.value("specular", glm::vec3(1, 1, 1)));

        //Attenuation vector is only considered with POINT and SPOT light types 
        if (lightType != LightType::DIRECTIONAL)
        {
        attenuation = glm::vec3(data.value("attenuation", glm::vec3(1, 0, 0)));
        }

        //If SPOT light type then read the inner and outer cone angles
        //In json degrees, therefore here convert them to radians
        if (lightType == LightType::SPOT)
        {  
        cone_angles.x = glm::radians((float)data.value("cone_angles.in",10));
        cone_angles.y = glm::radians((float)data.value("cone_angles.out",80));
        }
    }

}
