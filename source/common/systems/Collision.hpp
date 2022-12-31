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
            vector<IsCollide*> allColl;


            Entity *cam=nullptr;
            for(auto entity : world->getEntities()){
                //cout<<"ana ana ana"<<endl;
                auto collider =entity->getComponent<IsCollide>();
                if(collider)
                {
                    //cout<<"ana ana ana"<<endl;
                    allColl.push_back(collider);
                }
                if(entity->name=="player") //to save cam entity
                {
                    cam=entity;
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
            for (auto First : allColl)
			{
				for (auto Second : allColl)
				{
					if (First != Second)
					{
                        //cout<<"ana ana ana"<<endl;
						auto Name1 = First->getOwner()->name;
						auto Name2 = Second->getOwner()->name;
						auto center1 = First->getOwner()->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1);
						auto center2 = Second->getOwner()->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1);
						//Calculating distance to get Collision
						auto dist = glm::distance(center1, center2);
                        if(dist<0){
                            dist=dist*-1;
                        }
						if (First->Raduis+Second->Raduis >= dist) 
						{
                            if(Name1=="sword" && Name2=="zombie")
                            {
                                // cout<<"Coll ooooooooooooooooooooh"<<endl;
                                world->markForRemoval(Second->getOwner());
                                world->deleteMarkedEntities();
                            }
                            else if(Name1=="zombie" && Name2=="sword")
                            {
                                // cout<<"Coll ooooooooooooooooooooh"<<endl;
                                world->markForRemoval(First->getOwner());
                                world->deleteMarkedEntities();
                            }

                            if(Name1=="player" && Name2=="zombie")
                            {
                                if(!health)
                                {
                                    world->markForRemoval(Second->getOwner());
                                    world->deleteMarkedEntities();
                                    //app->changestate("Damaged")
                                    cout<<"Got hurted"<<endl;
                                    app->changeState("hurt");
                                }
                                if(health)
                                {
                                    app->changeState("lose");
                                }

                            }
                            else if(Name1=="zombie" && Name2=="player")
                            {
                                if(!health)
                                {
                                    world->markForRemoval(First->getOwner());
                                    world->deleteMarkedEntities();
                                    cout<<"Got hurted"<<endl;
                                    app->changeState("hurt");
                                }
                                if(health)
                                {
                                    app->changeState("lose");
                                }
                            }

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
