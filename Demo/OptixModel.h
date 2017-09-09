
#ifndef OPTIXCORE_OPTIXMODEL_H
#define OPTIXCORE_OPTIXMODEL_H

#include <iostream>
#include <optix_world.h>
using namespace std;
using namespace optix;

#include "OptixNode.h"
#include"OptixMaterial.h"
#include "OptixModelResource.h"
#include "OptixGeometryShader.h"
#include "OptixMesh.h"

class OptixModel:public OptixNode
{
public:
	OptixModel();
	~OptixModel();

	bool load(Context context, OptixModelResource* modelResouce, 
		OptixMaterial& m_material, OptixGeometryShader& gshader);

	string getName() { return m_name; }
	GeometryGroup getGeometryGroup() { return m_geometryGroup; }

	void setAcce(const string& acce) { m_acce = acce; }
	string getAcce() { return m_acce; }

private:
	OptixMaterial m_material;
	GeometryGroup m_geometryGroup;
	vector<OptixMesh*> m_mesh;
	vector<GeometryInstance> m_meshGeometryInstance;
	string m_name;
	string m_acce;
};

#endif

