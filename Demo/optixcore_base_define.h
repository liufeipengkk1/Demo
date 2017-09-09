#ifndef OPTIXCORE_BASE_DEFINE_H
#define OPTIXCORE_BASE_DEFINE_H

#ifdef _DEBUG
#include <assert.h>
#define Assert(_Expression) assert(_Expression)
#else
#define Assert(_Expression) ((void)0)
#endif

#define BACK_GROUND_PROGRAM "ray_missing_program"
#define VERTEX_BOUND_PROGRAM "bound_program"
#define VERTEX_INTESECT_PROGRAM  "intesect_program"
#define CAMERA_PROGRAM       "camera_program"
#define EXCEPTION_PROGRAM    "exception_program"
#define FRAGMENT_SHADER_PROGRAM "shader_program"
#define FRAGMENT_SHADOW_PROGRAM "shadow_program"

typedef enum OptixRayType{
	Shading_Ray = 0,
	Shadow_Ray = 1,
	TRANSPARENT_Ray
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