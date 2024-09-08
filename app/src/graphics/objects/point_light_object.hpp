#include "object.hpp"

namespace nova {
class PointLightObject : public nova_Object {
public:
    float lightIntensity = 1.0f;
    vec4 lightColor = vec4(1.f, 0.f, 0.f, 1.f);
};

} // namespace nova