#include "movement.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

#include <vector>
#include<iostream>
#include "../ecs/world.hpp"
#include "../components/movement.hpp"
#include "../ecs/component.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

#include "../components/IsCollide.hpp"
#include "../application.hpp"
namespace our {
    // Reads linearVelocity & angularVelocity from the given json object
    void MovementComponent::deserialize(const nlohmann::json& data){


        if(!data.is_object()) return;
        linearVelocity = data.value("linearVelocity", linearVelocity);
        angularVelocity = glm::radians(data.value("angularVelocity", angularVelocity));


        
    }
}