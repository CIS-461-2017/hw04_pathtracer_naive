#include "mattematerial.h"
#include "lambertbrdf.h"

void MatteMaterial::ProduceBSDF(Intersection *isect) const
{
    // Important! Must initialize the intersection's BSDF!
    BSDF* bsdf = new BSDF(*isect);
    isect->bsdf = std::shared_ptr<BSDF>(bsdf);

    Color3f color = Kd;
    if(this->textureMap)
    {
        color *= Material::GetImageColor(isect->uv, this->textureMap.get());
    }
    if(this->normalMap)
    {
        bsdf->normal = bsdf->tangentToWorld *  Material::GetImageColor(isect->uv, this->textureMap.get());
    }

    if(sigma == 0.f)
    {
        bsdf->Add(new LambertBRDF(color));
    }
    //Else do Oren-Nayar (not required implementation)
}
