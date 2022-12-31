#pragma once

#include "../ecs/component.hpp"
#include "../shader/shader.hpp"

#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>

namespace our {

    // To define the light type
    enum class LightType {
        DIRECTIONAL,
        POINT,
        SPOT
    };

    // This component denotes that any renderer should draw the scene relative to this light.
    // We do not define the eye, center or up here since they can be extracted from the entity local to world matrix
    //the light component class which inherits from the component class
    class LightComponent : public Component {
    public:
        //Data that will be read from the json file, except position and direction as they are calculated from the entity component in the forward renderer
        LightType lightType;
        glm::vec3 diffuse,specular,attenuation; 
        glm::vec3 color;
        glm::vec2 cone_angles; 
        glm::vec3 direction; // Used for Directional and Spot Lights only


        //In json file, type of light would be specified as string
        std::string lightTypeStr;
        
        //overriding the deserializer function to read the light data from the json file
        void deserialize(const nlohmann::json& data) override;
        
        //ID: "light", read from the json file and specifies that this is a light component
        static std::string getID() { return "light"; } };

}

