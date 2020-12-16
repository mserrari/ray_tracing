#include "integrator.h"
#include "scene.h"

class DirectIntergrator : public Integrator {
public:
    DirectIntergrator(const PropertyList &props) {
        /* No parameters this time */
    }

    Color3f Li(const Scene *scene, const Ray &ray) const {
        /** TODO : Find the surface that is visible in the requested direction
                   Return its ambiant color */

        Hit hit = Hit();
        scene->intersect(ray, hit);
        LightList lights = scene->lightList();

        /// Looping over light sources
        if (hit.foundIntersection())
        {
            Vector3f n = hit.normal();
            Vector3f viewDir = -ray.direction;
            Point3f secondOrigin = ray.at(hit.t()) + hit.normal() * float(1e-4);
            Point3f x = ray.at(hit.t());

            Color3f color = Color3f(0.0f);

            for (size_t i = 0; i < lights.size(); i++)
            {
                float lightDist = 0.0f;

                Vector3f lightDir = lights[i]->direction(x, &lightDist);
                Color3f lightInt = lights[i]->intensity(x);
                Ray shadowRay = Ray(secondOrigin, lightDir);
                Hit shadowH = Hit();

                scene->intersect(shadowRay, shadowH);

                /// Found intersection with object and the light source is further than said object, we ignore the shadow
                /// This is to prevent light from going through objects
                if (shadowH.foundIntersection() && shadowH.t() < lightDist) continue;

                Color3f rho = hit.shape()->material()->brdf(viewDir, lightDir, n);
                color = color + rho * std::max(n.dot(lightDir), 0.0f) * lightInt;
            }
            return color;
        }
        return Color3f(scene->backgroundColor());
    }

    std::string toString() const {
        return "DirectIntergrator[]";
    }
};

REGISTER_CLASS(DirectIntergrator, "direct")
