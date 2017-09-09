
#include <optix.h>
#include <optix_math.h>
#include "RayInfoStruct.h"
#include "random.h"
#include "helper.h"
using namespace optix;

rtDeclareVariable(float2, texcoord, attribute texcoord, );
rtDeclareVariable(float3, geometric_normal, attribute geometric_normal, );
rtDeclareVariable(float3, shading_normal, attribute shading_normal, );
rtDeclareVariable(float3, tangent, attribute tangent, );
rtDeclareVariable(float3, bigTangent, attribute bigTangent, );
rtDeclareVariable(optix::Ray, ray, rtCurrentRay, );
rtDeclareVariable(float, t_hit, rtIntersectionDistance, );
rtDeclareVariable(float, scene_epsilon, , );
rtDeclareVariable(rtObject, top_object, , );
rtDeclareVariable(rtObject, top_shadower, , );
rtDeclareVariable(int, radiance_ray_type, , );

rtDeclareVariable(float3, Ka, , );
rtDeclareVariable(float3, Ks, , );
rtDeclareVariable(float3, Kd, , );
rtDeclareVariable(float3, ambient_color, , );

rtDeclareVariable(PerRayData_radiance, prd_radiance, rtPayload, );
rtDeclareVariable(InfoRay_mask, prd_maskInfo, rtPayload, );
rtDeclareVariable(int, max_depth, , );
rtDeclareVariable(float3, reflectivity, , );

rtDeclareVariable(int, useNormalMap, , );


rtDeclareVariable(float3, Kcrq, , );
rtDeclareVariable(int, reflectionNums, , );

rtTextureSampler<float4, 2> normal_map;
rtTextureSampler<float4, 2> normal_map1;

rtDeclareVariable(float3, bg_color, , );
rtDeclareVariable(float, y_angle, , );
rtDeclareVariable(float, x_angle, , );
rtDeclareVariable(unsigned int, frame_number, , );

rtDeclareVariable(int, shading_ray, , );
rtDeclareVariable(int, shadow_ray, , );
rtDeclareVariable(int, refract_Ray, , );
rtDeclareVariable(int, light_distri_ray, , );

rtDeclareVariable(float, exponent, , );
rtDeclareVariable(float3, emission_color, , );

RT_PROGRAM void glossy_bsdf()
{
	float3 world_geo_normal = normalize(rtTransformNormal(RT_OBJECT_TO_WORLD, geometric_normal));
	float3 world_shade_normal = normalize(rtTransformNormal(RT_OBJECT_TO_WORLD, shading_normal));
	float3 ffnormal = faceforward(world_shade_normal, -ray.direction, world_geo_normal);

	float t = dot(-ray.direction, world_geo_normal);
	float3 color = Ka * ambient_color;
	float3 hit_point = ray.origin + t_hit * ray.direction;

	float3 result = make_float3(1.0f);

	if (t > 0){
		float2 sample;
		sample.x = rnd(prd_radiance.seed);
		sample.y = rnd(prd_radiance.seed);

		float bsdf_pdf;
		float bsdf_val;
		float3 dir_r = reflect(ray.direction, ffnormal);

		float3 v1, v2;
		createONB(dir_r, v1, v2);
		float3 direction = sample_phong_lobe(sample, exponent, v1, v2,dir_r, bsdf_pdf, bsdf_val);
		direction = normalize(direction);

		PerRayData_radiance prd;
		prd.radiance = make_float3(0);
		prd.attenuation = make_float3(1);
		prd.result = make_float3(0);
		prd.depth = 0;

		optix::Ray ray(hit_point, direction,
			radiance_ray_type, scene_epsilon, RT_DEFAULT_MAX);

		rtTrace(top_object, ray, prd);

		if (bsdf_pdf > 0)
		{
			/*prd.attenuation = prd.attenuation * bsdf_val / bsdf_pdf
				*fabs(dot(direction, ffnormal));*/

			prd.attenuation = prd.attenuation * bsdf_val / bsdf_pdf;
		}

		prd_radiance.radiance = prd.attenuation * prd.radiance;
	}
}

RT_PROGRAM
void diffuseEmetter(){
	prd_radiance.radiance = emission_color;
	prd_radiance.done = true;
}

rtDeclareVariable(float3, point_light_pos, , );
RT_PROGRAM void phong()
{
	float3 world_shading_normal = normalize(rtTransformNormal(RT_OBJECT_TO_WORLD, shading_normal));
	float3 world_geometric_normal = normalize(rtTransformNormal(RT_OBJECT_TO_WORLD, geometric_normal));

	float3 ffnormal = faceforward(world_shading_normal, -ray.direction, world_geometric_normal);

	float kd = 0.3;
	float ks = 0.7;
	
	if (dot(-ray.direction, world_shading_normal) > 0){
		float3 hitpoint = ray.origin + t_hit * ray.direction;
		float3 dir_r = reflect(ray.direction, ffnormal);

		float3 white = make_float3(1);
		float diffuse = kd * dot(normalize(point_light_pos - hitpoint), ffnormal);

		float3 halfV = point_light_pos - hitpoint + ray.direction;
		halfV = normalize(halfV);
		float specular = ks * powf(dot(halfV, ffnormal), exponent);

		prd_radiance.radiance = white *(specular + diffuse);
	}
}



RT_PROGRAM void color(){
	prd_radiance.radiance = make_float3(1,1,0);
}



RT_PROGRAM void shadow()
{
	prd_radiance.attenuation = make_float3(0);
}