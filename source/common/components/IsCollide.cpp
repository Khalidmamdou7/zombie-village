#include "light.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <asset-loader.hpp>
#include "IsCollide.hpp"
namespace our
{
    // Reads light parameters from the given json object
    void IsCollide::deserialize(const nlohmann::json &data)
    {
        if(!data.is_object()) return;

        Raduis = data.value("Raduis", 1.0f);
    }

}
