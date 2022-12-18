#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

//transformation matrix is used to transform the vertex positions from the object's local space to the world space

namespace our {

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const {
        //TODO: (Req 3) Write this function
        glm::mat4 scaling_matrix=glm::scale(glm::mat4(1.0f),scale);
        // yawPitchRoll is a function that takes in the yaw, pitch and roll and returns a rotation matrix
        // where yaw is the rotation around the y axis, pitch is the rotation around the x axis and roll is the rotation around the z axis
        glm::mat4 rotation_matrix=glm::yawPitchRoll(rotation.y,rotation.x,rotation.z);
        // translation matrix is a matrix that translates the vertex positions by the given vector
        glm::mat4 translation_matrix=glm::translate(glm::mat4(1.0f),position);

        // the order of transformations is: Scaling, Rotation then Translation
        glm::mat4 transformation_matrix=translation_matrix*rotation_matrix*scaling_matrix;

        return transformation_matrix; 
    }

     // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json& data){
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale    = data.value("scale", scale);
    }

}