#pragma once

#include <glad/gl.h>
#include "vertex.hpp"

namespace our {

    #define ATTRIB_LOC_POSITION 0
    #define ATTRIB_LOC_COLOR    1
    #define ATTRIB_LOC_TEXCOORD 2
    #define ATTRIB_LOC_NORMAL   3

    class Mesh {
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        unsigned int VBO, EBO;
        unsigned int VAO;
        // We need to remember the number of elements that will be draw by glDrawElements 
        GLsizei elementCount;
    public:

        // The constructor takes two vectors:
        // - vertices which contain the vertex data.
        // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
        // The mesh class does not keep a these data on the RAM. Instead, it should create
        // a vertex buffer to store the vertex data on the VRAM,
        // an element buffer to store the element data on the VRAM,
        // a vertex array object to define how to read the vertex & element buffer during rendering 
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& elements)
        {
           //TODO: (Req 2) Write this function
            // remember to store the number of elements in "elementCount" since you will need it for drawing
            // For the attribute locations, use the constants defined above: ATTRIB_LOC_POSITION, ATTRIB_LOC_COLOR, etc
            

            // create the vertex array object to define how to read the vertex and element buffer during rendering
           
            glGenVertexArrays(1,&VAO);
            glBindVertexArray(VAO);

            // create a vertex buffer and bind it, then store the verteces data to it
       
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            // the byte size of the vertices array is equal to size of one vertex in bytes multiplied by the size of the whole array
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

            // create an element buffer, and store the elements data in it
           
            glGenBuffers(1,&EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            // glBufferData defines the data of the buffer, and it takes the following parameters:
            // 1. the type of the buffer (GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER)
            // 2. the size of the data in bytes
            // 3. the data itself
            // 4. the usage of the buffer (GL_STATIC_DRAW, GL_DYNAMIC_DRAW, or GL_STREAM_DRAW) where 
            //    GL_STATIC_DRAW: the data will most likely not change at all or very rarely.
            //    GL_DYNAMIC_DRAW: the data is likely to change a lot.
            //    GL_STREAM_DRAW: the data will change every time it is drawn.
            // the byte size of the elements array is equal to size of one vertex in bytes multiplied by the size of the whole array
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,elements.size() * sizeof(unsigned int), elements.data(), GL_STATIC_DRAW);


            // We can tell OpenGL how it should interpret the vertex data (per vertex attribute) using glVertexAttribPointer: 
            // position attribute
            glEnableVertexAttribArray(ATTRIB_LOC_POSITION);
            // glVertexAttribPointer takes the following parameters:
            // 1. the index of the vertex attribute
            // 2. the size of the vertex attribute
            // 3. the type of the data
            // 4. whether or not the data should be normalized
            // 5. the stride (byte offset between consecutive attributes)
            // 6. the offset of the first component of the attribute
            glVertexAttribPointer(ATTRIB_LOC_POSITION, 3 , GL_FLOAT, false, sizeof(Vertex), (void *)0);

            // color attribute
            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);
            // the size of the color attribute is 4 because it is a vec4 (r,g,b,a)
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *)offsetof(Vertex, color));

            // normal attribute
            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);
            // the size of the normal attribute is 3 because it is a vec3 (x,y,z)
            glVertexAttribPointer(ATTRIB_LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

            // texture attribute
            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);
            // the size of the texture attribute is 2 because it is a vec2 (u,v)
            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tex_coord));
                        
            glBindVertexArray(0);
            elementCount = elements.size();

        }

        // this function should render the mesh
        void draw() 
        {
            //TODO: (Req 2) Write this function 
            glBindVertexArray(VAO);
            // glDrawElements draws the elements of the mesh, and it takes the following parameters:
            // 1. the type of the elements (GL_TRIANGLES, GL_LINES, GL_POINTS, etc)
            // 2. the number of elements to draw
            // 3. the type of the elements (GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT)
            // 4. the offset of the first element in the element buffer
            glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, (void *)0);

            // unbind the vertex array object
            glBindVertexArray(0);
        }

        // this function should delete the vertex & element buffers and the vertex array object
        ~Mesh(){
            // delete VBO, EBO, and VAO
            glDeleteBuffers(1,&VBO);
            glDeleteBuffers(1,&EBO);
            glDeleteVertexArrays(1,&VAO);
        }

        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };

}