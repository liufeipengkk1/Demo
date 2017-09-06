#pragma once

#include <iostream>
#include <optix_world.h>
#include <optixpp.h>

using namespace optix;
using namespace std;

class OptixNode
{
public:
	OptixNode();
	~OptixNode();

	//optix::Matrix4x4 getTransform() const;
	//void setTransform(const optix::Matrix4x4& transform);
	//void setRotation(const optix::Matrix4x4& rotation);
	//void setPostition(const float3& position);

private:
	optix::Matrix4x4 m_transform;
	optix::Matrix4x4 m_rotation;
	optix::float3    m_position;

	string           m_name;
};

