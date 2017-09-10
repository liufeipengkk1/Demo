
#ifndef OPTIXCORE_OPTIX_RENDER_STATE_H
#define OPTIXCORE_OPTIX_RENDER_STATE_H

#include <iostream>
#include <optix_world.h>
#include "Reference.h"

using namespace std;
using namespace optix;

class OptixRenderState :public Referenced
{
public:
	Context context;
	Group group;
	string acce;
	int width;
	int height;
	Program rayGen;
	Program exception;
	Program backGound;
	string topGroupName;
	int launchIdx;

	float3 eye;
	float3 u;
	float3 v;
	float3 w;
	string eyeName;
	string uName;
	string vName;
	string wName;
};

#endif