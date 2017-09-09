#ifndef OPTIXCORE_OPTIXMESH_H
#define OPTIXCORE_OPTIXMESH_H

#include <iostream>
#include <optix_world.h>
#include "OptixMeshResource.h"
#include "OptixMaterial.h"
#include "OptixGeometryShader.h"
using namespace std;
using namespace optix;


class OptixMesh :public Referenced
{
public:
	OptixMesh();
	~OptixMesh();

	bool load(OptixMeshResource* meshResource, Context context,
		OptixGeometryShader& gshader);
	
	void setName(const string& name) { m_name = name; }
	string getName() { return m_name; }

	Geometry getGeometry() { return m_meshGeometry; }

private:
	Geometry m_meshGeometry;
	string m_name;
};

#endif

