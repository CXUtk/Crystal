#include "Shape.h"


float Shape::PdfLight(const SurfaceInfo& surface_w, const Vector3f& wi) const
{
	Ray ray = surface_w.SpawnRay(wi);
	SurfaceInteraction isec_l;
	if (!Intersect(ray, &isec_l)) return 0.f;

    Float cosTheta = std::max(0.f, glm::dot(-glm::normalize(wi), isec_l.GetNormal()));
	return glm::length2(wi)
		/ (cosTheta * SurfaceArea());
}
