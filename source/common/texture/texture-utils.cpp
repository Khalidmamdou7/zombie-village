#include "texture-utils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

our::Texture2D* our::texture_utils::empty(GLenum format, glm::ivec2 size){
    our::Texture2D* texture = new our::Texture2D();
    //TODO: (Req 11) Finish this function to create an empty texture with the given size and format

    texture->bind();
    // glTexImage2D creates a texture object and allocates memory for it. It also sets the texture parameters
    // it takes 9 arguments
    // 1. target: the target texture, in this case GL_TEXTURE_2D, could be GL_TEXTURE_1D, GL_TEXTURE_3D, GL_TEXTURE_CUBE_MAP
    // 2. level: the level of detail, 0 is the base image level, n is the nth mipmap reduction image
    // 3. internalformat: the number of color components in the texture, could be GL_RED, GL_RG, GL_RGB, GL_RGBA, GL_DEPTH_COMPONENT, GL_DEPTH_STENCIL
    // 4. width: the width of the texture image
    // 5. height: the height of the texture image
    // 6. border: the width of the border, must be 0
    // 7. format: the format of the pixel data, could be GL_RED, GL_RG, GL_RGB, GL_RGBA, GL_DEPTH_COMPONENT, GL_DEPTH_STENCIL
    // 8. type: the data type of the pixel data, could be GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT, GL_FLOAT
    // 9. data: a pointer to the image data in memory, or 0 if no data is to be copied
    glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y, 0, format, GL_UNSIGNED_BYTE, nullptr);


    return texture;
}

our::Texture2D* our::texture_utils::loadImage(const std::string& filename, bool generate_mipmap) {
    glm::ivec2 size;
    int channels;
    //Since OpenGL puts the texture origin at the bottom left while images typically has the origin at the top left,
    //We need to till stb to flip images vertically after loading them
    stbi_set_flip_vertically_on_load(true);
    //Load image data and retrieve width, height and number of channels in the image
    //The last argument is the number of channels we want and it can have the following values:
    //- 0: Keep number of channels the same as in the image file
    //- 1: Grayscale only
    //- 2: Grayscale and Alpha
    //- 3: RGB
    //- 4: RGB and Alpha (RGBA)
    //Note: channels (the 4th argument) always returns the original number of channels in the file
    unsigned char* pixels = stbi_load(filename.c_str(), &size.x, &size.y, &channels, 4);
    if(pixels == nullptr){
        std::cerr << "Failed to load image: " << filename << std::endl;
        return nullptr;
    }
    // Create a texture
    our::Texture2D* texture = new our::Texture2D();
    //Bind the texture such that we upload the image data to its storage
    //TODO: (Req 5) Finish this function to fill the texture with the data found in "pixels"
    texture->bind();
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,size.x,size.y,0,GL_RGBA,GL_UNSIGNED_BYTE,(void*) pixels);

    
    //boolean to know if it's true , it will do mimamp , levels as in slides
    if(generate_mipmap){
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(pixels); //Free image data after uploading to GPU
    return texture;
}