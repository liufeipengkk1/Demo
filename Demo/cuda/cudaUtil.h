
#ifndef UTIL_CUDAUTIL_H
#define UTIL_CUDAUTIL_H

#include <optix_world.h>
using namespace optix;

typedef struct ShaderRay{
	int m_depth; // ������������һ��������
	float4 m_color;
	float m_refCoefficient;  //����ϵ��
	float m_rafCoefficient;  //Ͷ��ϵ��
	
	unsigned int m_seed; //Ŀ����Ϊ�˲��������
	float3 m_direction;
	bool m_done;
}ShaderRay;

#endif