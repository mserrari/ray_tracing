#include "integrator.h"
#include "scene.h"

class NormalsIntergrator : public Integrator {
public:
    NormalsIntergrator(const PropertyList &props) {
        /* No parameters this time */
    }

    Color3f Li(const Scene *scene, const Ray &ray) const {
        /** TODO : Find the surface that is visible in the requested direction
                   Return its ambiant color */
        Hit h = Hit();
        scene->intersect(ray, h);

        if (h.foundIntersection())
        {
            //return Color3f(h.shape()->material()->ambientColor());

            Vector3f n = h.normal();
            return Color3f(fabs(n.x()), fabs(n.y()), fabs(n.z()));
        }

        //return Color3f(0.f);
        return Color3f(scene->backgroundColor());

    }

    std::string toString() const {
        return "NormalsIntergrator[]";
    }
};

REGISTER_CLASS(NormalsIntergrator, "normals")
