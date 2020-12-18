#include "integrator.h"
#include "scene.h"

class TexcoordsIntegrator : public Integrator {
public:
    TexcoordsIntegrator(const PropertyList &props) {
        /* No parameters this time */
    }

    Color3f Li(const Scene *scene, const Ray &ray) const {
        /* Find the surface that is visible in the requested direction */

        /* Return the component-wise absolute
           value of the shading Texcoords as a color */
        
        //TODO:  FIX ME ENSEIRB STUDENT
        return Color3f(0.0f,0.0f,0.0f);
    }

    std::string toString() const {
        return "TexcoordsIntegrator[]";
    }
};

REGISTER_CLASS(TexcoordsIntegrator, "texcoords")
