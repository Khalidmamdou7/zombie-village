#pragma once

#include <glad/gl.h>
#include <glm/vec4.hpp>
#include <json/json.hpp>

namespace our {
    // There are some options in the render pipeline that we cannot control via shaders
    // such as blending, depth testing and so on
    // Since each material could require different options (e.g. transparent materials usually use blending),
    // we will encapsulate all these options into a single structure that will also be responsible for configuring OpenGL's pipeline
    struct PipelineState {
        // This set of pipeline options specifies whether face culling will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum culledFace = GL_BACK;
            GLenum frontFace = GL_CCW;
        } faceCulling;

        // This set of pipeline options specifies whether depth testing will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum function = GL_LEQUAL;
        } depthTesting;

        // This set of pipeline options specifies whether blending will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum equation = GL_FUNC_ADD;
            GLenum sourceFactor = GL_SRC_ALPHA;
            GLenum destinationFactor = GL_ONE_MINUS_SRC_ALPHA;
            glm::vec4 constantColor = {0, 0, 0, 0};
        } blending;

        // These options specify the color and depth mask which can be used to
        // prevent the rendering/clearing from modifying certain channels of certain targets in the framebuffer
        glm::bvec4 colorMask = {true, true, true, true}; // To know how to use it, check glColorMask
        bool depthMask = true; // To know how to use it, check glDepthMask


        // This function should set the OpenGL options to the values specified by this structure
        // For example, if faceCulling.enabled is true, you should call glEnable(GL_CULL_FACE), otherwise, you should call glDisable(GL_CULL_FACE)
        void setup() const {
            //TODO: (Req 4) Write this function

            if (faceCulling.enabled) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
            // Configure face culling options
            // Configure which face to cull (back, front or both)
            glCullFace(faceCulling.culledFace);
            // Configure front face winding order (which side is considered front (counter-clockwise or clockwise))
            glFrontFace(faceCulling.frontFace);


            if (depthTesting.enabled) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
            // Configure depth testing options
            // Configure depth testing function (GL_LESS, GL_LEQUAL, GL_GREATER, GL_GEQUAL, GL_EQUAL, GL_NOTEQUAL, GL_ALWAYS, GL_NEVER)
            glDepthFunc(depthTesting.function);

            
            // Enable/disable depth buffer writing
            glDepthMask(depthMask);

            // Enable/disable Writing color components into the framebuffer
            // glColorMask specifies whether the individual color components in the frame buffer can or cannot be written.
            // If red is GL_FALSE, for example, no change is made to the red component of any pixel in any of the color buffers,
            // regardless of the drawing operation attempted. 
            glColorMask(colorMask.r, colorMask.g, colorMask.b, colorMask.a);

            if (blending.enabled) glEnable(GL_BLEND); else glDisable(GL_BLEND);
            // Configure blending options
            // Configure blending equation (GL_FUNC_ADD, GL_FUNC_SUBTRACT, GL_FUNC_REVERSE_SUBTRACT, GL_MIN, GL_MAX)
            glBlendEquation(blending.equation);
            // Configure blending function (GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA, GL_SRC_ALPHA_SATURATE, GL_SRC1_COLOR, GL_ONE_MINUS_SRC1_COLOR, GL_SRC1_ALPHA, GL_ONE_MINUS_SRC1_ALPHA)
            glBlendFunc(blending.sourceFactor, blending.destinationFactor);
            // Configure blending constant color
            glBlendColor(blending.constantColor.r, blending.constantColor.g, blending.constantColor.b, blending.constantColor.a);

        }

        // Given a json object, this function deserializes a PipelineState structure
        void deserialize(const nlohmann::json& data);
    };

}