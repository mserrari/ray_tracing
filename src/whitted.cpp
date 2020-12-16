#include "integrator.h"
#include "scene.h"

class WhittedIntegrator : public Integrator {
public:
    WhittedIntegrator(const PropertyList &props) {
        /* No parameters this time */
        m_maxRecursion = props.getInteger("maxRecursion", 0);
    }

    Color3f Li(const Scene *scene, const Ray &ray) const {
        /** TODO : Find the surface that is visible in the requested direction
                   Return its ambiant color */
    
        /// Reached the end of the recursion
        if (ray.recursionLevel > m_maxRecursion) {
            return Color3f(0.0f);
        }

        /// Integrate the ray
        Hit hit = Hit();
        scene->intersect(ray, hit);
        LightList lights = scene->lightList();

        if (hit.foundIntersection())
        {
            Vector3f n = hit.normal();
            Vector3f viewDir = -ray.direction;
            Point3f secondOrigin = ray.at(hit.t()) + hit.normal() * float(1e-4);
            Point3f x = ray.at(hit.t());

            Color3f color = Color3f(0.0f);

            /// Looping over light sources
            for (size_t i = 0; i < lights.size(); i++)
            {
                float lightDist = 0.0f;

                Vector3f lightDir = lights[i]->direction(x, &lightDist);
                Color3f lightInt = lights[i]->intensity(x);
                Ray shadowRay = Ray(secondOrigin, lightDir);
                Hit shadowH = Hit();

                scene->intersect(shadowRay, shadowH);

                /// Found intersection with object and the light source is further than said object, we ignore the shadow
                if (shadowH.foundIntersection() && shadowH.t() < lightDist) continue;

                Color3f rho = hit.shape()->material()->brdf(viewDir, lightDir, n);
                color = color + rho * std::max(n.dot(lightDir), 0.0f) * lightInt;
            }

            Vector3f bounceDir = (-viewDir + (2 * n * n.dot(viewDir))).normalized();
            Ray newRay = Ray(x + n * float(1e-4), bounceDir);
            newRay.recursionLevel = ray.recursionLevel + 1;

            return color + hit.shape()->material()->reflectivity() * Li(scene, newRay);
        }
        else {
            return Color3f(scene->backgroundColor());
        }
    }

    std::string toString() const {
        return 
            tfm::format("WhittedIntegrator[ \n"
                "maxRecursion : %s\n"
                "]",
                m_maxRecursion);
    }
};

REGISTER_CLASS(WhittedIntegrator, "whitted")
