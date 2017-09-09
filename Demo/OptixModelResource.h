
#ifndef OPTIXCORE_OPTIXMODELRESOURCE_H
#define OPTIXCORE_OPTIXMODELRESOURCE_H

#include <iostream>
#include <optix_world.h>
#include "Reference.h"
#include "SharePtr.h"
using namespace optix;
using namespace std;

class OptixMeshResource;
class ModelResource;

class OptixModelResource :public Referenced
{
public:
	OptixModelResource();
	virtual ~OptixModelResource();
	bool load(ModelResource* modelResource, Context context);

	vector<OptixMeshResource*>& getMeshResouces();
	OptixMeshResource* getMesh(const string& name);
private:
	SharePtr<ModelResource> m_modelResource;
	Context context;
	vector<OptixMeshResource*>  m_meshResources;
	string m_name;
};

#endif