
#ifndef UTIL_CUDAUTIL_H
#define UTIL_CUDAUTIL_H

#include <optix_world.h>
using namespace optix;

typedef struct ShaderRay{
	int m_depth; // ������������һ��������
	float4 m_color;
	float3 m_result;
	float m_refCoefficient;  //����ϵ��
	float m_rafCoefficient;  //Ͷ��ϵ��
	
	unsigned int m_seed; //Ŀ����Ϊ�˲��������
	float3 m_origin;
	float3 m_direction;
	bool m_done;
	float3 m_attenuation;
	float3 m_radiance;
	bool   m_countEmitted;
	bool   m_inside;
}ShaderRay;

#endif