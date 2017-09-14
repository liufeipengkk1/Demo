#include <optix.h>
#include <optixu/optixu_math_namespace.h>
#include "../helper.h"
#include "../random.h"
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
	prd.m_radiance = make_float3(0,0,0);
	prd.m_depth = 0;
	prd.m_done = false;

	rtTrace(top_Object, ray, prd);
	
	output_buffer[launch_index] = make_float4(prd.m_radiance, 1.0);
}

rtDeclareVariable(unsigned int, frame_number, , );
rtDeclareVariable(float, Y_log_av, , );
rtDeclareVariable(float, Y_max, , );
rtBuffer<float3, 2>  hdr_buffer;
////¿¹¾â³Ýcamera
RT_PROGRAM void jitter_pinhole_camera(){
	size_t2 screen = output_buffer.size();
	float2 inv_screen = 1.0f / make_float2(screen) * 2.f;
	float2 pixel = (make_float2(launch_index)) * inv_screen - 1.f;
	unsigned int sqrt_num_samples = 2;//2X²ÉÑù
	float2 jitter_scale = inv_screen / sqrt_num_samples;
	unsigned int samples_per_pixel = sqrt_num_samples*sqrt_num_samples;
	float3 result = make_float3(0.0f);
	unsigned int seed = tea<16>(screen.x*launch_index.y + launch_index.x, frame_number);
	do{
		unsigned int x = samples_per_pixel%sqrt_num_samples;
		unsigned int y = samples_per_pixel / sqrt_num_samples;
		float2 jitter = make_float2(x - rnd(seed), y - rnd(seed));
		float2 d = pixel + jitter*jitter_scale;
		float3 ray_origin = eye;
		float3 ray_direction = normalize(d.x*u + d.y*v + w);

		ShaderRay prd;
		prd.m_color = make_float4(0, 0, 0, 0);
		prd.m_result = make_float3(0);
		prd.m_radiance = make_float3(0);
		prd.m_depth = 0;
		prd.m_done = false;
		prd.m_seed = seed;
		prd.m_attenuation = make_float3(1.f);

		for (;;) {
			optix::Ray ray(ray_origin, ray_direction, SHADING_RAY, SCENE_EPSILON, RT_DEFAULT_MAX);
			rtTrace(top_Object, ray, prd);

			if (prd.m_done) {
				prd.m_result += prd.m_radiance * prd.m_attenuation;
				break;
			}

			if (prd.m_depth >= 2){
				float pcont = fmaxf(prd.m_attenuation);
				if (rnd(prd.m_seed) >= pcont)
					break;
				prd.m_attenuation /= pcont;
			}
			prd.m_depth++;
			prd.m_result += prd.m_radiance * prd.m_attenuation;
			ray_origin = prd.m_origin;
			ray_direction = prd.m_direction;
		} 

		result += prd.m_result;
		seed = prd.m_seed;
	} while (--samples_per_pixel);

	result = result / (sqrt_num_samples*sqrt_num_samples);
	float3 pixel_color = make_float3(result.x, result.y, result.z);
	if (frame_number > 1)
	{
		float a = 1.0f / (float)frame_number;
		float b = ((float)frame_number - 1.0f) * a;
		hdr_buffer[launch_index] = a * pixel_color + b * hdr_buffer[launch_index];
	}
	else
		hdr_buffer[launch_index] = pixel_color;

	//need toemapping for hdr? 
	//output_buffer[launch_index] = make_float4(tonemap(hdr_buffer[launch_index], Y_log_av, Y_max), 1.0f);
	output_buffer[launch_index] = make_float4(hdr_buffer[launch_index], 1.0);
}

//rtTextureSampler<float4, 2> envmap;
rtTextureSampler<float4, 2> envmap;
rtDeclareVariable(float3, bgColor,,);
rtDeclareVariable(float, intensity, , );
RT_PROGRAM void backGround()
{
	/*float theta = atan2f(ray.direction.x, ray.direction.z);
	float phi = M_PIf * 0.5f - acosf(ray.direction.y);
	float u = (theta + M_PIf) * (0.5f * M_1_PIf);
	float v = 0.5f * (1.0f + sin(phi));*/

	float x = ray.direction.x;
	float y = ray.direction.y;
	float z = ray.direction.z;
	float p = acosf(y);
	float h = atan2(x, z);
	float pi = 3.14159265358979323846;
	if (x > 0)
		h = h;
	else
		h = 2 * pi + h;
	
	float v = p / pi;
	float u = h / pi * 0.5;
	prd_radiance.m_radiance = make_float3(tex2D(envmap, u, v));
	//prd_radiance.m_radiance = make_float3(0);
	prd_radiance.m_done = true;
	//prd_radiance.m_color = make_float4(0,1,0, 1.0);
}

RT_PROGRAM void exception()
{
	output_buffer[launch_index] = make_float4(1, 1, 0, 1);
}