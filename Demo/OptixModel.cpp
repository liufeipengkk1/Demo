#include "OptixModel.h"
#include "optixcore_base_define.h"

OptixModel::OptixModel()
{
	m_acce = OPTIX_ACCE_METHOD_BVH;
}


OptixModel::~OptixModel()
{
	for (auto& mesh : m_mesh) {
		delete mesh;
		mesh = nullptr;
	}

	m_mesh.clear();
	m_meshGeometryInstance.clear();
}


bool OptixModel::load(OptixContext* context, OptixModelResource* modelResouce,
	OptixMaterial& material, OptixGeometryShader& gshader) {

	if (modelResouce == nullptr)
		return false;

	m_modelResouce = SharePtr<OptixModelResource>(modelResouce);

	m_geometryGroup = context->getContext()->createGeometryGroup();
	m_geometryGroup->setAcceleration(context->getContext()->createAcceleration(m_acce));
	m_material = material;
	for (auto& meshResouce : modelResouce->getMeshResouces()) {
		OptixMesh* mesh = new OptixMesh();
		mesh->load(meshResouce, context, gshader);
		m_mesh.push_back(mesh);
		
		GeometryInstance instance = context->getContext()->createGeometryInstance();
		instance->setGeometry(mesh->getGeometry());
		instance->setMaterialCount(1);
		instance->setMaterial(0, material.getMaterial());
		m_meshGeometryInstance.push_back(instance);
		m_geometryGroup->addChild(instance);
	}

	return true;
}
