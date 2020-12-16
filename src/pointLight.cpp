#include "light.h"

class PointLight : public Light
{
public:
    PointLight(const PropertyList &propList)
        : Light(propList.getColor("intensity", Color3f(1.f)))
    {
        m_position = propList.getPoint("position", Point3f::UnitX());
    }

    Vector3f direction(const Point3f& x, float* dist = 0) const
    {
        /// TODO
        //throw RTException("PointLight::direction not implemented yet.");
        if (dist)
            *dist = (m_position - x).norm();
        Vector3f direction = (m_position - x).normalized();
        return direction;
    }

    Color3f intensity(const Point3f& x) const
    {
        /// TODO
        /// + 1e-6 for stability when dividing
        float value = (m_position - x).squaredNorm() + 1e-6;
        return m_intensity / value;
    }

    std::string toString() const {
        return tfm::format(
            "PointLight[\n"
            "  intensity = %s\n"
            "  position = %s\n"
            "]", m_intensity.toString(),
                 ::toString(m_position));
    }

protected:
    Point3f m_position;
};

REGISTER_CLASS(PointLight, "pointLight")
