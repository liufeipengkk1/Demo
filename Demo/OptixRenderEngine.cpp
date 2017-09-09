#include "OptixRenderEngine.h"
#include "optixcore_base_define.h"

OptixRenderEngine::OptixRenderEngine()
{

}


OptixRenderEngine::~OptixRenderEngine()
{
}

void OptixRenderEngine::beforeRender(OptixRenderState& renderState) {
	Context context = renderState.context;
	context->setRayGenerationProgram(renderState.launchIdx, renderState.rayGen);
	context->setExceptionProgram(renderState.launchIdx, renderState.exception);
	context->setMissProgram(Shading_Ray, renderState.backGound);
	context[renderState.topGroupName]->set(renderState.group);

	context->validate();
}

void OptixRenderEngine::doRender(OptixRenderState& renderState) {
	renderState.context->launch(renderState.launchIdx, renderState.width, renderState.height);
}