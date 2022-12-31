#pragma once

#include "../ecs/world.hpp"
#include "../components/movement.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic. 
    // For more information, see "common/components/movement.hpp"
    class MovementSystem {
    public:

        // This should be called every frame to update all entities containing a MovementComponent. 
        void update(World* world, float deltaTime) {

            //(zombies position - camera position).normalized *speed
            // direction= (zombies position - camera position)
            // deltatime ----> dirction


            // For each entity in the world

            
            for(auto entity : world->getEntities()){
                // Get the movement component if it exists
                MovementComponent* movement = entity->getComponent<MovementComponent>();
                
                //saving player entity on cam to get position
                glm::vec3 cam={0, 0.2, 10};
                
                for(auto entity : world->getEntities()){
                    if(entity->name=="player")
                    {
                        cam=entity->localTransform.position;
                    }
                    
            }
                //editing movement to make zombies move to the player 


                // If the movement component exists
                if(movement){
                    // Change the position and rotation based on the linear & angular velocity and delta time.
                    if(entity->name=="zombie")
                    {
                        //getting direction (player position - zombie position)
                        auto dirct=(cam-entity->localTransform.position);
                        // then normalize it 
                        dirct=normalize(dirct);
                        //entity->localTransform.position += deltaTime * movement->linearVelocity;
                        //making entity position which is zombie equals delta time * normalized direction
                        entity->localTransform.position += deltaTime * dirct;

                        entity->localTransform.rotation += deltaTime * movement->angularVelocity;
                    }
                }
                
            }
        }

    };

}
