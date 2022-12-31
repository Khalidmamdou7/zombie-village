#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

#include <iostream>

namespace our {

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const {
        //DONE: (Req 7) Write this function
        pipelineState.setup();
        // Set the shader to be used
        shader->use();
    }

    // This function read the material data from a json object
    void Material::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;

        if(data.contains("pipelineState")){
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint 
    void TintedMaterial::setup() const {
        //DONE: (Req 7) Write this function
        Material::setup();
        shader->set("tint",tint);
    }

    // This function read the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json& data){
        Material::deserialize(data);
        if(!data.is_object()) return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex" 
    void TexturedMaterial::setup() const {
        //DONE: (Req 7) Write this function
        //call the setup of its parent
        TintedMaterial::setup();

        // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
        shader->set("alphaThreshold", alphaThreshold);

        // bind the texture
        if (texture)
        {
            glActiveTexture(GL_TEXTURE0); // active texture unit
            texture->bind();
        }

        //bind the sampler
        if (sampler)
            sampler->bind(0); // We bind the sampler to texture unit 0 (the first texture unit)

        //send the uint number to the uniform variable "tex"
        shader->set("tex", 0);
    }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json& data){
        TintedMaterial::deserialize(data);
        if(!data.is_object()) return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

 //add here the lit material setup and deserializer

    void LitMaterial::setup() const 
    {
        //calling on the textured material setup function (since lit material inherits from textured material)
        TexturedMaterial::setup(); 
        /*
        1. Go through the texture maps defined in the .hpp
        2. Bind the texture and sampler
        3. Set the values in the shader file
        */
        if (albedo)
        {
            glActiveTexture(GL_TEXTURE0);  
            albedo->bind();
            sampler->bind(0);
            std::cout << "Setting material albedo in shaders" << std::endl;
            shader->set("material.albedo",0);
        }
        if (specular)
        {
            glActiveTexture(GL_TEXTURE1);  
            specular->bind();
            sampler->bind(1);
            shader->set("material.specular",1);
        }
        if (ambient_occlusion )
        {
            glActiveTexture(GL_TEXTURE2);  
            ambient_occlusion->bind();
            sampler->bind(2);
            shader->set("material.ambient_occlusion",2);
        }
        if (roughness)
        {
            glActiveTexture(GL_TEXTURE3);  
            roughness->bind();
            sampler->bind(3);
            shader->set("material.roughness",3);
        }
        if (emissive)
        {
            glActiveTexture(GL_TEXTURE4);  
            emissive->bind();
            sampler->bind(4);
            shader->set("material.emission",4);
        }
        glActiveTexture(GL_TEXTURE0);
    }

    //the deserializer function to read data from the json object
    void LitMaterial::deserialize(const nlohmann::json& data)
    {
        //calling on the textured material function
        TexturedMaterial::deserialize(data); 

        if(!data.is_object())
        {
            return;
        }
        albedo = AssetLoader<Texture2D>::get(data.value("albedo", ""));
        specular = AssetLoader<Texture2D>::get(data.value("specular", ""));
        ambient_occlusion = AssetLoader<Texture2D>::get(data.value("ambient_occlusion", ""));
        roughness = AssetLoader<Texture2D>::get(data.value("roughness", ""));
        emissive = AssetLoader<Texture2D>::get(data.value("emissive", ""));

        //Get the sampler
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));

    }

}