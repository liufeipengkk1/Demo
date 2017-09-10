#include "OptixRenderEngine.h"
#include "optixcore_base_define.h"
#include <optix_world.h>
using namespace optix;

OptixRenderEngine::OptixRenderEngine()
{

}


OptixRenderEngine::~OptixRenderEngine()
{
}

void OptixRenderEngine::beforeRender(OptixRenderState& renderState) {
	Context context = renderState.context;

	context[renderState.eyeName]->setFloat(renderState.eye);
	context[renderState.uName]->setFloat(renderState.u);
	context[renderState.vName]->setFloat(renderState.v);
	context[renderState.wName]->setFloat(renderState.w);
	context->setRayGenerationProgram(renderState.launchIdx, renderState.rayGen);
	context->setExceptionProgram(renderState.launchIdx, renderState.exception);
	context->setMissProgram(Shading_Ray, renderState.backGound);
	context[renderState.topGroupName]->set(renderState.group);

	try{
		context->validate();
	}
	catch (optix::Exception& e) {
		cout << "Error Code: " << e.getErrorCode() << endl;
		cout << "Error String" << e.getErrorString() << endl;
	}
}

void OptixRenderEngine::doRender(OptixRenderState& renderState) {
	renderState.context->launch(renderState.launchIdx, renderState.width, renderState.height);
}