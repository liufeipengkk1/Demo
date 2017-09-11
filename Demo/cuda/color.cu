
#include <optix_world.h>
#include "cudaUtil.h"

using namespace optix;

rtDeclareVariable(ShaderRay, prd_radiance, rtPayload, );

RT_PROGRAM void fragment()
{
	prd_radiance.m_color = make_float4(1, 0, 1, 1.0);
	prd_radiance.m_done = true;
}
