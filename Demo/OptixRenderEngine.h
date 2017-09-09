
#ifndef OPTIXRENDERENGINE_H
#define OPTIXRENDERENGINE_H

#include <optix_world.h>
#include <iostream>
#include "Reference.h"
#include "Image.h"
#include "OptixRenderState.h"

using namespace std;
using namespace optix;

class OptixRenderEngine :public Referenced
{
public:
	OptixRenderEngine();
	virtual ~OptixRenderEngine();
	
	void beforeRender(OptixRenderState& renderState);
	void doRender(OptixRenderState& renderState);
};

#endif