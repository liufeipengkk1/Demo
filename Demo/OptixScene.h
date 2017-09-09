
#ifndef OPTIXCORE_SCENE_
#define OPTIXCORE_SCENE_

#include <iostream>
#include <optix_world.h>
#include <vector>

#include "OptixNode.h"

using namespace std;
using namespace optix;

class OptixScene :public Referenced
{
public:
	OptixScene();
	~OptixScene();

	void addNode(OptixNode* node);
};

#endif

