#include "material.h"

Phong::Phong(const PropertyList &propList)
    : Diffuse(propList.getColor("diffuse",Color3f(0.2)))
{
    //std::cout << "Got diffuse from xml and value is : " << m_diffuseColor;

    m_specularColor = propList.getColor("specular",Color3f(0.9));
    m_reflectivity = propList.getColor("reflectivity",Color3f(0.0));
    m_exponent = propList.getFloat("exponent",0.2);

    std::string texturePath = propList.getString("texture","");
    if(texturePath.size()>0){
        filesystem::path filepath = getFileResolver()->resolve(texturePath);
        loadTextureFromFile(filepath.str());
        setTextureScale(propList.getFloat("scale",1));
        setTextureMode(TextureMode(propList.getInteger("mode",0)));
    }
}

Color3f Phong::brdf(const Vector3f& viewDir, const Vector3f& lightDir, const Normal3f& normal, const Vector2f& uv) const
{
    /// TODO: implement Phong brdf
    // Calculating the reflected vector
    Vector3f r =  2 * normal * lightDir.dot(normal) - lightDir;

    Color3f rho = m_diffuseColor + m_specularColor * std::powf(std::max(viewDir.dot(r), 0.0f), m_exponent);

    // Calling the function diffuseColor give me alot of errors on windows (values in the range of -1e42)
    //Color3f rho = diffuseColor(uv) + m_specularColor * std::powf(std::max(r.dot(viewDir), 0.0f), m_exponent);

    return rho;
}

std::string Phong::toString() const {
    return tfm::format(
        "Phong[\n"
        "  diffuse color = %s\n"
        "  specular color = %s\n"
        "  exponent = %f\n"
        "]", m_diffuseColor.toString(),
             m_specularColor.toString(),
             m_exponent);
}

REGISTER_CLASS(Phong, "phong")
