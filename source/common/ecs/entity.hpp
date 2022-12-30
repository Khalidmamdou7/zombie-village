#pragma once

#include "component.hpp"
#include "transform.hpp"
#include <list>
#include <iterator>
#include <string>
#include <glm/glm.hpp>

#include<iostream>
namespace our {

    class World; // A forward declaration of the World Class

    class Entity{
        World *world; // This defines what world own this entity
        std::list<Component*> components; // A list of components that are owned by this entity

        friend World; // The world is a friend since it is the only class that is allowed to instantiate an entity
        Entity() = default; // The entity constructor is private since only the world is allowed to instantiate an entity
    public:
        std::string name; // The name of the entity. It could be useful to refer to an entity by its name
        Entity* parent;   // The parent of the entity. The transform of the entity is relative to its parent.
                          // If parent is null, the entity is a root entity (has no parent).
        Transform localTransform; // The transform of this entity relative to its parent.

        World* getWorld() const { return world; } // Returns the world to which this entity belongs

        glm::mat4 getLocalToWorldMatrix() const; // Computes and returns the transformation from the entities local space to the world space
        void deserialize(const nlohmann::json&); // Deserializes the entity data and components from a json object
        
        // This template method create a component of type T,
        // adds it to the components map and returns a pointer to it 
        template<typename T>
        T* addComponent(){
            static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
            //DONE: (Req 8) Create an component of type T, set its "owner" to be this entity, then push it into the component's list
            T* comp=new T();
            comp->owner=this;
            components.push_back(comp);
            // Don't forget to return a pointer to the new component
            return comp;
        }

        // This template method searhes for a component of type T and returns a pointer to it
        // If no component of type T was found, it returns a nullptr 
        template<typename T>
        T* getComponent(){
            //std::cout<<"ana gwa el get ";
            //DONE: (Req 8) Go through the components list and find the first component that can be dynamically cast to "T*".
            for(auto it=components.begin();it!=components.end();it++)
            {
                T* cast=dynamic_cast<T*>(*it);
                if(cast)
                    return cast;
            }
            // Return the component you found, or return null of nothing was found.
            return nullptr;
        }

        // This template method dynami and returns a pointer to it
        // If no component of type T was found, it returns a nullptr 
        template<typename T>
        T* getComponent(size_t index){
            auto it = components.begin();
            std::advance(it, index);
            if(it != components.end())
                return dynamic_cast<T*>(*it);
            return nullptr;
        }

        // This template method searhes for a component of type T and deletes it
        template<typename T>
        void deleteComponent(){
            //DONE: (Req 8) Go through the components list and find the first component that can be dynamically cast to "T*".
            // If found, delete the found component and remove it from the components list
            T* first_component = getComponent<T>(); // making use of the getComponent<T>() method we implemented above
             if(first_component) //if NOT NULL i.e, there was a component that could be dynamically cast to T*
             {
                delete *first_component;
                components.erase(first_component);
             }
        }

        // This template method searhes for a component of type T and deletes it
        void deleteComponent(size_t index){
            auto it = components.begin();
            std::advance(it, index);
            if(it != components.end()) {
                delete *it;
                components.erase(it);
            }
        }

        // This template method searhes for the given component and deletes it
        template<typename T>
        void deleteComponent(T const* component){
            //DONE: (Req 8) Go through the components list and find the given component "component".
            for(auto it=components.begin();it!=components.end();it++)
            {
                if(*it==component)
                {
                    delete *it;
                    components.erase(it);
                    break;
                }
            }
            // If found, delete the found component and remove it from the components list
        }

        // Since the entity owns its components, they should be deleted alongside the entity
        ~Entity(){
            //DONE: (Req 8) Delete all the components in "components".
            
            for(Component *comp : components)
            {
                delete comp;
            }
            components.clear();
            }

        // Entities should not be copyable
        Entity(const Entity&) = delete;
        Entity &operator=(Entity const &) = delete;
    };

}