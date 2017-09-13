
#ifndef UTIL_CUDAUTIL_H
#define UTIL_CUDAUTIL_H

#include <optix_world.h>
using namespace optix;

typedef struct ShaderRay{
	int m_depth; // 反弹次数，有一定的限制
	float4 m_color;
	float3 m_result;
	float m_refCoefficient;  //反射系数
	float m_rafCoefficient;  //投射系数
	
	unsigned int m_seed; //目的是为了产生随机数
	float3 m_origin;
	float3 m_direction;
	bool m_done;
	float3 m_attenuation;
	float3 m_radiance;
	bool   m_countEmitted;
	bool   m_inside;
}ShaderRay;

#endif