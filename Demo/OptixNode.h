#pragma once

#include <iostream>
#include <optix_world.h>
#include <optixpp.h>
#include "Reference.h"

using namespace optix;
using namespace std;

class OptixNode :public Referenced
{
public:
	OptixNode();
	virtual ~OptixNode();

	void translate(const float3& traslate);
	void rotation(const float& angle, const float3& axis);
	void scale(const float3& s);

	optix::Matrix4x4 getTransform();

	void setName(const string& name);
	string getName() const{ return m_name; }
protected:
	optix::Matrix4x4 m_transform;
	string           m_name;
};

