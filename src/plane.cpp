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

//bool Plane::intersect(const Ray& ray, Hit& hit) const
//{
//    /// TODO
//    Point3f a = m_position;
//    Vector3f n = m_normal;
//    Vector3f d = ray.direction;
//    Vector3f o = ray.origin;
//
//    if (abs(d.dot(n)) < 1e-6) {
//        return false;
//    }
//    else {
//        float t = ((a - o).dot(n)) / (d.dot(n));
//        if (t <= 0) return false;
//        else if(t >0 && t < hit.t()) {
//            hit.setShape(this);
//            hit.setT(t);
//            hit.setNormal(m_normal);
//            return true;
//        }
//        return false;
//    }
//}

bool Plane::intersect(const Ray& ray, Hit& hit) const
{
    // TODO

    // assuming vectors are all normalized
    float denom = ray.direction.dot(this->m_normal);
    if (abs(denom) < 1e-6) return false;
    float t = (this->m_position - ray.origin).dot(this->m_normal) / denom;
    if (t >= 0 && t < hit.t()) {
        hit.setT(t);
        hit.setShape(this);
        hit.setNormal(m_normal);
    }
    return true;
}

REGISTER_CLASS(Plane, "plane")
