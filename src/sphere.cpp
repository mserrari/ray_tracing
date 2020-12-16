#include "sphere.h"
#include <iostream>

Sphere::Sphere(float radius)
    : m_radius(radius)
{
}

Sphere::Sphere(const PropertyList &propList)
{
    m_radius = propList.getFloat("radius",1.f);
    m_center = propList.getPoint("center",Point3f(0,0,0));
}

Sphere::~Sphere()
{
}

bool Sphere::intersect(const Ray& ray, Hit& hit) const
{
    /// TODO: compute ray-sphere intersection

    /// throw RTException("Sphere::intersect not implemented yet.");

    Vector3f o = ray.origin;
    Vector3f d = ray.direction;

    float a = d.squaredNorm();
    float b = 2 * d.dot(o - m_center);
    float c = (o - m_center).squaredNorm() - m_radius * m_radius;

    float delta = b*b - 4*a*c;

    if (delta < 0)
    {
        return false;
    }
    else
    {
        float t1 = (- b + sqrt(delta)) / (2 * a);
        float t2 = (- b - sqrt(delta)) / (2 * a);

        
        float t;
        if (t1 < 0 && t2 < 0) return false;
        else if (t1 >= 0 && t2 >= 0) t = std::min(t1, t2);
        else if (t1 >= 0 && t2 < 0) t = t1;
        else t = t2;

        if (t < hit.t() && t > 0) {
            Vector3f n = (ray.at(t) - m_center).normalized();
            hit.setNormal(n);
            hit.setT(t);
            hit.setShape(this);
            return true;
        }
        else {
            return false;
        }

    }
}

REGISTER_CLASS(Sphere, "sphere")
