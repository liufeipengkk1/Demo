#include "OptixModelResource.h"
#include "OptixMeshResource.h"
#include "ModelResource.h"

OptixModelResource::OptixModelResource()
{
	m_name = "";
}

OptixModelResource::~OptixModelResource()
{
	for (auto& mesh : m_meshResources) {
		delete mesh;
		mesh = nullptr;
	}

	m_meshResources.clear();
}

vector<OptixMeshResource*>& OptixModelResource::getMeshResouces() {
	return m_meshResources;
}

OptixMeshResource* OptixModelResource::getMesh(const string& name) {
	for (auto& mesh : m_meshResources) {
		if (mesh->getName() == name) {
			return mesh;
		}
	}

	return nullptr;
}

bool OptixModelResource::load(ModelResource* modelResource, Context context) {
	if (modelResource == nullptr)
		return false;

	m_modelResource = SharePtr<ModelResource>(modelResource);

	for (auto& meshResouce : modelResource->getMeshResources()) {
		OptixMeshResource* optixMesh = new OptixMeshResource();
		optixMesh->setMeshCache(meshResouce, context);
		m_meshResources.push_back(optixMesh);
	}

	return true;
}
