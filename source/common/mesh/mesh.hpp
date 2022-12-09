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
            elementCount = elements.size();

            // create the vertex array object to define how to read the vertex and element buffer during rendering
            GLuint VAO;
            glGenVertexArrays(1,&VAO);
            glBindVertexArray(VAO);

            // create a vertex buffer and bind it, then store the verteces data to it
            GLuint VBO;
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            // the byte size of the vertices array is equal to size of one vertex in bytes multiplied by the size of the whole array
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

            // create an element buffer, and store the elements data in it
            GLuint EBO;
            glGenBuffers(1,&EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            // the byte size of the elements array is equal to size of one vertex in bytes multiplied by the size of the whole array
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,elements.size() * sizeof(unsigned int), &elements[0], GL_STATIC_DRAW);


            // We can tell OpenGL how it should interpret the vertex data (per vertex attribute) using glVertexAttribPointer: 
            // position attribute
            glEnableVertexAttribArray(ATTRIB_LOC_POSITION);
            glVertexAttribPointer(ATTRIB_LOC_POSITION, 2 , GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));

            // color attribute
            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, color));

            // normal attribute
            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);
            glVertexAttribPointer(ATTRIB_LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

            // texture attribute
            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);
            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tex_coord));

        }

        // this function should render the mesh
        void draw() 
        {
            //TODO: (Req 2) Write this function 
            glDrawElement(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, 0);
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