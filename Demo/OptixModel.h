
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
#include "OptixContext.h"
#include "SharePtr.h"

class OptixModel:public OptixNode
{
public:
	OptixModel();
	~OptixModel();

	bool load(OptixContext* context, const string& modelPath, OptixMaterial& material, OptixGeometryShader& gshader, ImageManager* manager);
	bool load(OptixContext* context, OptixModelResource* modelResouce, 
		OptixMaterial& m_material, OptixGeometryShader& gshader);

	string getName() { return m_name; }
	GeometryGroup getGeometryGroup() { return m_geometryGroup; }
	Transform getTransformGroup();

	void setAcce(const string& acce) { m_acce = acce; }
	string getAcce() { return m_acce; }

	OptixModel* clone(const string& name) {
		OptixModel* newModel = new OptixModel();
		newModel->m_transform = Matrix4x4::translate(make_float3(0, 0, 0));
		newModel->m_modelResouce = m_modelResouce;
		newModel->m_material = m_material;
		newModel->m_mesh = m_mesh;
		newModel->m_meshGeometryInstance = m_meshGeometryInstance;
		newModel->m_name = name;
		newModel->m_acce = m_acce;
		newModel->m_geometryGroup = m_geometryGroup;
		newModel->m_context = m_context;
		if (m_context.get() != nullptr) {
			newModel->m_transformGroup = m_context->createTransform();
			newModel->m_transformGroup->setChild(newModel->m_geometryGroup);
			newModel->m_transformGroup->setMatrix(false, newModel->m_transform.getData(), 0);
		}

		return newModel;
	}

private:
	Context m_context;
	SharePtr<OptixModelResource> m_modelResouce;
	OptixMaterial m_material;
	GeometryGroup m_geometryGroup;
	Transform m_transformGroup;
	vector<OptixMesh*> m_mesh;
	vector<GeometryInstance> m_meshGeometryInstance;
	string m_name;
	string m_acce;
};

#endif

