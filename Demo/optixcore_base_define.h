#ifndef OPTIXCORE_BASE_DEFINE_H
#define OPTIXCORE_BASE_DEFINE_H

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