#include "OptixScene.h"
#include "OptixCamera.h"
#include "OptixModel.h"
#include "OptixRenderState.h"
#include "OptixContext.h"

OptixScene::OptixScene()
{
	m_curCamera = nullptr;
	m_accMethod = OPTIX_ACCE_METHOD_BVH;
	m_context = nullptr;
}


OptixScene::~OptixScene()
{
	if (m_context)
		delete m_context;
	m_context = nullptr;
}


void OptixScene::setOptixContext(OptixContext* context) {
	m_context = context;
}

void OptixScene::addOptixModel(OptixModel* node) {
	m_models.push_back(node);
	m_geGroup.push_back(node->getGeometryGroup());
}

void OptixScene::addOptixCamera(OptixCamera* node) {
	m_cameras[node->getName()] = node;
}

void OptixScene::activeCamera(const string& name) {
	auto& itr = m_cameras.find(name);
	if (itr != m_cameras.end()) {
		m_curCamera = itr->second.get();
	}
}

void OptixScene::setAcceMethod(const string & method) {
	m_accMethod = method;
}

OptixCamera* OptixScene::getActiveCamera() {
	return m_curCamera;
}

vector<SharePtr<OptixModel>>& OptixScene::getModels(){ 
	return m_models; 
}

string OptixScene::getAcceMethod() {
	return m_accMethod;
}

Image* OptixScene::getRenderResult() {
	if (m_curCamera == nullptr)
		return nullptr;

	return m_curCamera->getView()->getRenderResult();
}

void OptixScene::beforeRender() {
	if (m_context != nullptr) {
		m_group = m_context->getContext()->createGroup();
		m_group->setAcceleration(m_context->getContext()->createAcceleration(m_accMethod));
		for (auto& model : m_models){
			m_group->addChild(model->getGeometryGroup());
		}
	}

	if (m_curCamera != nullptr) {
		m_state.context = m_context->getContext();
		m_state.acce = m_accMethod;
		m_state.group = m_group;
		m_state.width = m_context->getW();
		m_state.height = m_context->getH();
		m_state.launchIdx = m_context->getCurEntryPoint();
		m_state.topGroupName = m_context->getTopGeometryName();

		m_curCamera->beforRender(m_state);
	}
}

void OptixScene::doRender() {
	if (m_curCamera != nullptr) {
		m_curCamera->doRender(m_state);
	}
}