#include "OptixMaterial.h"
#include "OptixShader.h"


OptixMaterial::OptixMaterial()
{
	m_material = nullptr;
}


void OptixMaterial::setShader(OptixShaderType type, OptixShader& shader){
	m_shaders[type] = shader;
}

void OptixMaterial::linkShader(Context context) {
	m_material = context->createMaterial();
	auto& itr1 = m_shaders.find(OPTIX_SHADER);
	auto& itr2 = m_shaders.find(OPTIX_SHADOW);
	if (itr1 != m_shaders.end()) {
		m_material->setClosestHitProgram(Shading_Ray, itr1->second.getHandle());
	}
	else {
		cout << "OptixMaterial: No Shader Program" << endl;
	}
	if (itr2 != m_shaders.end()) {
		m_material->setAnyHitProgram(Shadow_Ray, itr2->second.getHandle());
	}
	else {
		cout << "OptixMaterial: No Shadow Program" << endl;
	}
}

OptixMaterial::~OptixMaterial()
{
}
