#include "integrator.h"
#include "scene.h"

class WhittedIntegrator : public Integrator {
public:
    WhittedIntegrator(const PropertyList &props) {
        /* No parameters this time */
        m_maxRecursion = props.getInteger("maxRecursion", 4);
    }

    Color3f Li(const Scene *scene, const Ray &ray) const {

        Color3f radiance = Color3f::Zero();

        // stopping criteria is recursion level > maxRecursion
        // STOP 
        if (ray.recursionLevel > m_maxRecursion) {
            return radiance;
        }

        /* Find the surface that is visible in the requested direction */

        Hit hit;
        scene->intersect(ray, hit);
        LightList lights = scene->lightList();

        if (hit.foundIntersection())
        {
            // DIRECT LIGHTING IMPLEMENTATION goes here 
            Vector3f n = hit.normal();
            Vector3f viewDir = -ray.direction;
            Point3f secondOrigin = ray.at(hit.t()) + hit.normal() * float(1e-4);
            Point3f x = ray.at(hit.t());

            //Color3f color = Color3f(0.0f);

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
                radiance += rho * std::max(n.dot(lightDir), 0.0f) * lightInt;
            }

            Vector3f bounceDir = (-viewDir + (2 * n * n.dot(viewDir))).normalized();
            Ray newRay = Ray(x + n * float(1e-4), bounceDir);

            // Decrease recursion level
            newRay.recursionLevel = ray.recursionLevel - 1;

            return radiance + hit.shape()->material()->reflectivity() * Li(scene, newRay);

        }
        else if (ray.recursionLevel == 0)
            return scene->backgroundColor();

        return radiance;
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
