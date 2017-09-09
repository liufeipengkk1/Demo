#ifndef OPTIXCORE_BASE_DEFINE_H
#define OPTIXCORE_BASE_DEFINE_H

#ifdef _DEBUG
#include <assert.h>
#define Assert(_Expression) assert(_Expression)
#else
#define Assert(_Expression) ((void)0)
#endif

typedef enum OptixRayType{
	Shading_Ray = 0,
	Shadow_ray = 1
}RayType;

typedef enum OptixShaderType{
	OPTIX_SHADER = 0,
	OPTIX_SHADOW =1,
	OPTIX_BOUND = 2,
	OPTIX_INTESECT = 3,
	OPTIX_RAYGEN = 4,
	OPTIX_EXCEPTION = 5,
	OPTIX_MISS =6
}OptixShaderType;

#endif