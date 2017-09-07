#ifndef OPTIXCORE_OPTIXMATERIAL_H
#define OPTIXCORE_OPTIXMATERIAL_H

#include <optix_world.h>
#include <iostream>
#include <map>

#include "optixcore_base_define.h"
using namespace std;

class OptixShader;

class OptixMaterial
{
public:
	OptixMaterial();
	~OptixMaterial();

	void setShader(OptixShaderType type ,OptixShader& shader);
	void linkShader(optix::Context context);
	optix::Material getMaterial() { return m_material; }
	
	void setName(const string& name) { m_name = name; }
	string getName() { return m_name; }
private:
	optix::Material m_material;
	string m_name;
	map<int, OptixShader> m_shaders;
};

#endif
