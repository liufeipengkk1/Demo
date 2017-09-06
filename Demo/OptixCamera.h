
#ifndef OPTIXCORE_CAMERA_H
#define OPTIXCORE_CAMERA_H

#include <iostream>
#include "OptixNode.h"
using namespace std;

//optix camera as a ray launcher
class OptixCamera:public OptixNode
{
public:
	OptixCamera();
	~OptixCamera();

	void setCameraParameter(const float& fov, const float& aspect,
		const float3& position, const float3& lookAt, const float3& wUp);
	
	float3 getPosition();
	float3 getU();
	float3 getV();
	float3 getW();

private:
	void calculateUVW();

private:
	float m_fov;
	float m_aspect;
	float3 m_up;
	float3 m_position;
	float3 m_lookAt;

	float3 m_u; //x
	float3 m_v; //y
	float3 m_w; //z
};

#endif