#include <optix.h>
#include <optixu/optixu_math_namespace.h>
#include "../helper.h"
#include "cudaUtil.h"

using namespace optix;

rtDeclareVariable(float3, eye, ,);
rtDeclareVariable(float3, u, , );
rtDeclareVariable(float3, v, , );
rtDeclareVariable(float3, w, , );

//global
rtDeclareVariable(rtObject, top_Object, , );

rtDeclareVariable(uint2, launch_index, rtLaunchIndex, );
rtDeclareVariable(optix::Ray, ray, rtCurrentRay, );
rtDeclareVariable(ShaderRay, prd_radiance, rtPayload, );
rtDeclareVariable(int, max_depth, , );

rtBuffer<float4, 2>    output_buffer;

RT_PROGRAM void pinhole_camera()
{
	size_t2 screen = output_buffer.size();
	float2 d = make_float2(launch_index) / make_float2(screen) * 2.f - 1.f;
	float3 ray_origin = eye;
	float3 ray_direction = normalize(d.x*u + d.y*v + w);

	optix::Ray ray(ray_origin, ray_direction, SHADING_RAY, SCENE_EPSILON);
	
	ShaderRay prd;
	prd.m_depth = 0;
	prd.m_done = false;

	rtTrace(top_Object, ray, prd);
	
	output_buffer[launch_index] = prd.m_color;
}

rtTextureSampler<float4, 2> envmap;
rtDeclareVariable(float3, bgColor,,);
RT_PROGRAM void backGround()
{
	//float theta = atan2f(ray.direction.x, ray.direction.z);
	//float phi = M_PIf * 0.5f - acosf(ray.direction.y);
	//float u = (theta + M_PIf) * (0.5f * M_1_PIf);
	//float v = 0.5f * (1.0f + sin(phi));
	//prd_radiance.m_color = tex2D(envmap, u, v);

	prd_radiance.m_color = make_float4(0,1,0, 1.0);
}

RT_PROGRAM void exception()
{
	output_buffer[launch_index] = make_float4(1, 1, 0, 1);
}