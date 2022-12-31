#pragma once
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


bool health=false;

using namespace std;
namespace our
{

    class Collision {
        Application* app;
    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application* app){
            this->app = app;
        }

        // This should be called every frame to update all entities containing a MovementComponent. 
        void update(World* world, float deltaTime) {
            vector<IsCollide*> allColl; //vector to save all collides entities


            Entity *cam=nullptr;
            for(auto entity : world->getEntities()){
                auto collider =entity->getComponent<IsCollide>();
                if(collider) //if it can collide push it
                {
                    allColl.push_back(collider);
                }
                if(entity->name=="player") //to save camera entity to use in making zombie moving to player
                {
                    cam=entity; //save player entity "camera"

                    //this conditions to make sure that camera can't go out walls
                    if(cam->localTransform.position.x <-10 || cam->localTransform.position.x >10)
                    {
                        app->changeState("hurt");
                    }
                    if(cam->localTransform.position.y <-10 || cam->localTransform.position.y >10)
                    {
                        app->changeState("hurt");
                    }
                }
            }
            for (auto First : allColl) //nested loop to get two parameters on the collision
			{
				for (auto Second : allColl)
				{
					if (First != Second)
					{
						auto Name1 = First->getOwner()->name; //saving first one name
						auto Name2 = Second->getOwner()->name;//saving second one name

                        //getting the center of entity by getLocalToWorldMatrix * vec4 to get the point
						auto center1 = First->getOwner()->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1);
						auto center2 = Second->getOwner()->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1);
						//Calculating distance to get Collision
						auto dist = glm::distance(center1, center2);
                        if(dist<0){ //if distance -ve value get absolute
                            dist=dist*-1;
                        }
						if (First->Raduis+Second->Raduis >= dist) // at this condition , collision happened
						{
                            if(Name1=="sword" && Name2=="zombie") //if sword with zombie, destroy the zombie
                            {
                                world->markForRemoval(Second->getOwner());
                                world->deleteMarkedEntities();
                            }
                            else if(Name1=="zombie" && Name2=="sword") //if sword with zombie, destroy the zombie
                            {
                                world->markForRemoval(First->getOwner());
                                world->deleteMarkedEntities();
                            }
                            //if player with zombie, check lose state or it's hurt "damaged" state
                            if(Name1=="player" && Name2=="zombie") 
                            {
                                //boolean health if it's true ,It means I got hurted before
                                if(!health)
                                {
                                    //destroy zombie who hurted or killed me
                                    world->markForRemoval(Second->getOwner());
                                    world->deleteMarkedEntities();

                                    //damaged state
                                    app->changeState("hurt");
                                }
                                if(health) //If I got hurted before --> Lose State "GAME OVER"
                                {
                                    app->changeState("lose");
                                }

                            }//if player with zombie, check lose state or it's hurt "damaged" state
                            else if(Name1=="zombie" && Name2=="player")
                            {
                                //boolean health if it's true ,It means I got hurted before
                                if(!health)
                                {
                                    //destroy zombie who hurted or killed me
                                    world->markForRemoval(First->getOwner());
                                    world->deleteMarkedEntities();

                                    //damaged state
                                    app->changeState("hurt");
                                }
                                if(health) //If I got hurted before --> Lose State "GAME OVER"
                                {
                                    app->changeState("lose");
                                }
                            }

                            // Here If player collidied wall then player got hurted
                            // wall can't kill the player , only damage him
                            if(Name1=="player" && Name2=="wall")
                            {
                                app->changeState("hurt");
                            }
                            else if(Name1=="wall" && Name2=="player")
                            {
                                app->changeState("hurt");
                            }
						}
					}
				}
			}
        }

    };

}
