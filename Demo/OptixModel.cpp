#include "OptixModel.h"
#include "optixcore_base_define.h"
#include "ModelResource.h"
#include "ImageManager.h"

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

	m_context = context->getContext();
	m_transformGroup = context->getContext()->createTransform();
	return true;
}

bool OptixModel::load(OptixContext* context, const string& modelPath, OptixMaterial& material, OptixGeometryShader& gshader, ImageManager* manager){

	ModelResource* model = new ModelResource();
	model->load(modelPath, manager);
	OptixModelResource *modelResouce = new OptixModelResource();
	modelResouce->load(model, context->getContext());

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

	m_context = context->getContext();
	m_transformGroup = context->getContext()->createTransform();
	return true;
}

Transform OptixModel::getTransformGroup() {
	m_transformGroup->setChild(m_geometryGroup);
	m_transformGroup->setMatrix(false, m_transform.getData(), 0);
	
	return m_transformGroup;
}
