#include "plane.h"

Plane::Plane()
{
}

Plane::Plane(const PropertyList &propList)
{
    m_position = propList.getPoint("position",Point3f(0,0,0));
    m_normal = propList.getVector("normal",Point3f(0,0,1));
}

Plane::~Plane()
{
}

bool Plane::intersect(const Ray& ray, Hit& hit) const
{
    // TODO

    float projection = ray.direction.dot(m_normal);

    if (fabs(projection) < 1e-6) {
        return false;
    }
    float t = (m_position - ray.origin).dot(m_normal) / projection;
    if (t >= 0 && t < hit.t()) {
        hit.setT(t);
        hit.setShape(this);
        hit.setNormal(m_normal);
        return true;
    }
    return false;
}

REGISTER_CLASS(Plane, "plane")
