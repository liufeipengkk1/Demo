#include "OptixModel.h"

const static char* TRANSFORM_MATRIX = "modelMatrix";
OptixModel::OptixModel()
{
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


bool OptixModel::load(Context context, OptixModelResource* modelResouce,
	OptixMaterial& material, OptixGeometryShader& gshader) {

	if (modelResouce == nullptr)
		return false;

	m_geometryGroup = context->createGeometryGroup();
	m_geometryGroup->setAcceleration(context->createAcceleration("Bvh"));
	m_material = material;
	for (auto& meshResouce : modelResouce->getMeshResouces()) {
		OptixMesh* mesh = new OptixMesh();
		mesh->load(meshResouce, context, gshader);
		m_mesh.push_back(mesh);
		
		GeometryInstance instance = context->createGeometryInstance(
			mesh->getGeometry(), &(material.getMaterial()), &(material.getMaterial()) + 1);
		instance[TRANSFORM_MATRIX]->setMatrix4x4fv(false, m_transform.getData());
		m_meshGeometryInstance.push_back(instance);
		m_geometryGroup->addChild(instance);
	}

	return true;
}

string OptixModel::getTransformParser() {
	return string(TRANSFORM_MATRIX);
}