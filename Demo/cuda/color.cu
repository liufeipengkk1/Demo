
#include <optix_world.h>
#include "cudaUtil.h"
#include "../helper.h"
#include "../random.h"

using namespace optix;

//global
rtDeclareVariable(rtObject, top_Object, , );

rtDeclareVariable(float, t_hit, rtIntersectionDistance, );
rtDeclareVariable(ShaderRay, prd_radiance, rtPayload, );

rtDeclareVariable(float2, texcoord, attribute texcoord, );
rtDeclareVariable(float3, geometric_normal, attribute geometric_normal, );
rtDeclareVariable(float3, shading_normal, attribute shading_normal, );

rtDeclareVariable(optix::Ray, ray, rtCurrentRay, );
rtTextureSampler<float4, 2> normalMap;

RT_PROGRAM void fragment()
{
	float3 world_shade_normal = normalize(rtTransformNormal(RT_OBJECT_TO_WORLD, shading_normal));
	float3 T = normalize(rtTransformNormal(RT_OBJECT_TO_WORLD, shading_normal));
	float3 B = normalize(rtTransformNormal(RT_OBJECT_TO_WORLD, shading_normal));
	float3 texNormal = make_float3(tex2D(normalMap, texcoord.x, texcoord.y));
	texNormal = 2 * texNormal - 1;
	prd_radiance.m_depth += 1;
	if (prd_radiance.m_depth < 7 && !prd_radiance.m_done) {
		//if (dot(-ray.direction, geometric_normal) > 0) {
			float3 hit_point = ray.origin + t_hit * ray.direction;
			float3 R = reflect(ray.direction, texNormal);
			ShaderRay refl_prd;
			refl_prd.m_radiance = make_float3(0, 0, 0);
			refl_prd.m_depth = prd_radiance.m_depth;
			optix::Ray refl_ray(hit_point, R, SHADING_RAY, SCENE_EPSILON);
			rtTrace(top_Object, refl_ray, refl_prd);
			prd_radiance.m_radiance = refl_prd.m_radiance;
		//}
	}
}


rtDeclareVariable(float3, pointLightPos, , );
rtDeclareVariable(float, pointLightRadius, ,);
RT_PROGRAM void glossy_BRDF() {
	float3 world_shade_normal = normalize(rtTransformNormal(RT_OBJECT_TO_WORLD, shading_normal));
	float3 hit_point = ray.origin + t_hit * ray.direction;

	prd_radiance.m_origin = hit_point;

	float2 sample;
	sample.x = rnd(prd_radiance.m_seed);
	sample.y = rnd(prd_radiance.m_seed);
	float bsdf_pdf;
	float bsdf_val;
	float3 dir_out = -ray.direction;
	float3 texNormal = make_float3(tex2D(normalMap, texcoord.x, texcoord.y));
	float3 dir_r = reflect(ray.direction, texNormal);
	float3 v1, v2;
	createONB(dir_r, v1, v2);
	prd_radiance.m_direction = sample_phong_lobe(sample, 10000, v1, v2, dir_r, bsdf_pdf, bsdf_val);  //这个灯泡函数到底是什么  好特么的关键啊   我日。
	if (bsdf_val > 0) {
		//菲涅尔项 
		float3 d = normalize(-ray.direction) + normalize(prd_radiance.m_direction);
		d = normalize(d);
		float rf0 = 0.15;
		float cosAlpha = dot(d, texNormal);
		float fre = 0;
		if (cosAlpha > 0) {
			fre = rf0 + (1 - rf0)* pow(1 - cosAlpha, 5);
			prd_radiance.m_attenuation = make_float3(bsdf_val / bsdf_pdf);
		}

		prd_radiance.m_attenuation =  make_float3(bsdf_val / bsdf_pdf);
	}
}

RT_PROGRAM void microfacet_BSDF(){

}

rtDeclareVariable(float3, col, ,);
RT_PROGRAM void color()
{
	prd_radiance.m_radiance = col;
	prd_radiance.m_done = true;
}
