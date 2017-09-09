


#include <optix.h>
#include <optix_math.h>
#include <optixu/optixu_math_namespace.h>
#include "RayInfoStruct.h"
#include "random.h"
#include "helper.h"
using namespace optix;

//variable
rtDeclareVariable(int, radiance_ray_type, , );
rtDeclareVariable(int, shadow_ray_type, , );
rtDeclareVariable(int, max_depth, , );
rtDeclareVariable(float, scene_epsilon, , );
rtDeclareVariable(float3, bd_color, , );
rtDeclareVariable(float3, bg_color, , );
rtDeclareVariable(float3, eye, , );
rtDeclareVariable(float3, ambient_color, , );
rtDeclareVariable(float3, U, , );
rtDeclareVariable(float3, V, , );
rtDeclareVariable(float3, W, , );
rtDeclareVariable(rtObject, top_object, , );
rtDeclareVariable(rtObject, top_shadower, , );
rtDeclareVariable(unsigned int, sqrt_num_samples, , );
rtDeclareVariable(unsigned int, frame_number, , );

rtBuffer<float4, 2>  output_buffer;
rtBuffer<float4, 2> hdr_buffer;

//sematic variable
rtDeclareVariable(float3, geometric_normal, attribute geometric_normal, );
rtDeclareVariable(float3, shading_normal, attribute shading_normal, );
rtDeclareVariable(float2, texture_coord, attribute texture_coord, );
rtDeclareVariable(optix::Ray, ray, rtCurrentRay, );
rtDeclareVariable(float, t_hit, rtIntersectionDistance, );
rtDeclareVariable(uint2, launch_index, rtLaunchIndex, );


//we also need to know current ray payload for ray travesal information
rtDeclareVariable(PerRayData_radiance, prd_radiance, rtPayload, );
rtDeclareVariable(PerRayData_shadow, prd_shadow, rtPayload, );

//光的衰减系数
rtDeclareVariable(float3, Kcrq, , );

RT_PROGRAM void pinhole_camera()
{
	size_t2 screen = output_buffer.size();
	float2 pixel = make_float2(launch_index) / make_float2(screen) * 2 - 1;
	
	float3 pixel_color;

	//jitter
	float2 dt = 1.0 / make_float2(screen) * 2;

	unsigned int samples_per_pixel = sqrt_num_samples * sqrt_num_samples;
	unsigned int seed = tea<16>(screen.x*launch_index.y + launch_index.x, frame_number);
	
	PerRayData_radiance prd;
	//init prd 
	prd.depth = 0;
	prd.result = make_float3(0, 0, 0);
	prd.radiance = make_float3(0, 0, 0);
	prd.attenuation = make_float3(1.f);
	prd.countEmitted = true;
	prd.done = false;
	prd.seed = seed;

	//如果等于1 一个像素 不让他产生随机量，不然会有更明显的噪点
	if (sqrt_num_samples == 1){
		float2 d = pixel;

		float3 ray_origin = eye;
		float3 ray_direction = normalize(d.x * U + d.y*V + W);
		optix::Ray ray(ray_origin, ray_direction, radiance_ray_type, scene_epsilon);

		rtTrace(top_object, ray, prd);

		prd.result = prd.radiance;
	}
	else{
		//一个像素取samples_per_pixel根光线
		for (int y = 0; y < sqrt_num_samples; y++){
			for (int x = 0; x < sqrt_num_samples; x++){

				//use jitter for random ray ... 
				float2 r = make_float2(x - rnd(seed), y - rnd(seed)) * dt;


				float2 d = pixel + r;

				float3 ray_origin = eye;
				float3 ray_direction = normalize(d.x * U + d.y*V + W);
				optix::Ray ray(ray_origin, ray_direction, radiance_ray_type, scene_epsilon);

				rtTrace(top_object, ray, prd);

				prd.result += prd.radiance;
			}
		}
	}
	

	
	prd.result /= samples_per_pixel;
	
	pixel_color = prd.radiance;
	if (frame_number > 1)
	{
		float a = 1.0f / (float)frame_number;
		float b = 1 - a;
		float3 old_color = make_float3(hdr_buffer[launch_index]);

		pixel_color = a * pixel_color + b * old_color;
		hdr_buffer[launch_index] = make_float4(pixel_color, 1);
	}
	else
	{
		output_buffer[launch_index] = make_float4(pixel_color, 1.0f);
		hdr_buffer[launch_index] = make_float4(pixel_color, 1.0f);
	}
	
	output_buffer[launch_index] = make_float4(pixel_color, 1.0f);
}

RT_PROGRAM void miss()
{
	prd_radiance.radiance = bg_color;
	prd_radiance.done = true;
}


RT_PROGRAM void exception(){
	output_buffer[launch_index] = make_float4(bg_color, 1);
}

rtDeclareVariable(float3, color, , );

RT_PROGRAM void closest_hit_program()
{	
	prd_radiance.radiance = color ;
}

