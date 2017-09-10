
#include <optix_world.h>
#include "cudaUtil.h"

using namespace optix;

rtDeclareVariable(float3, color, , );

rtDeclareVariable(ShaderRay, prd_radiance, rtPayload, );

RT_PROGRAM void fragment()
{
	prd_radiance.m_color = make_float4(color, 1.0);
	prd_radiance.m_done = true;
}
