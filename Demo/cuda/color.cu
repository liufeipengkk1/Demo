
#include <optix_world.h>
#include "cudaUtil.h"
#include "../helper.h"

using namespace optix;

//global
rtDeclareVariable(rtObject, top_Object, , );

rtDeclareVariable(float, t_hit, rtIntersectionDistance, );
rtDeclareVariable(ShaderRay, prd_radiance, rtPayload, );

rtDeclareVariable(float2, texcoord, attribute texcoord, );
rtDeclareVariable(float3, geometric_normal, attribute geometric_normal, );
rtDeclareVariable(float3, shading_normal, attribute shading_normal, );

rtDeclareVariable(optix::Ray, ray, rtCurrentRay, );

RT_PROGRAM void fragment()
{
	float3 world_shade_normal = normalize(rtTransformNormal(RT_OBJECT_TO_WORLD, shading_normal));
	prd_radiance.m_depth += 1;
	if (prd_radiance.m_depth < 7 && !prd_radiance.m_done) {
		//if (dot(-ray.direction, geometric_normal) > 0) {
			float3 hit_point = ray.origin + t_hit * ray.direction;
			float3 R = reflect(ray.direction, world_shade_normal);
			ShaderRay refl_prd;
			refl_prd.m_color = make_float4(0, 0, 0, 0);
			refl_prd.m_depth = prd_radiance.m_depth;
			optix::Ray refl_ray(hit_point, R, SHADING_RAY, SCENE_EPSILON);
			rtTrace(top_Object, refl_ray, refl_prd);
			prd_radiance.m_color = refl_prd.m_color;
		//}
	}
}

rtDeclareVariable(float3, col, ,);
RT_PROGRAM void color()
{
	prd_radiance.m_color = make_float4(col, 1);
	prd_radiance.m_depth += 1;
	prd_radiance.m_done = true;
}
