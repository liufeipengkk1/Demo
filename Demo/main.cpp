#include <iostream>
#include <optix_world.h>
#include "ModelResource.h"
#include "ImageManager.h"
#include "OptixScene.h"
#include "OptixShader.h"
#include "OptixContext.h"
#include "OptixModel.h"
#include "OptixCamera.h"
#include "OptixView.h"
using namespace std;

int screenW = 640;
int screenH = 480;
int main() {
	cout << "liufeipeng" << endl;
	optix::Matrix4x4 transform = optix::Matrix4x4::translate(optix::make_float3(0));
	cout << transform << endl;
	
	//create resource
	ImageManager* imgMg = ImageManager::getInstance();
	ModelResource* mr = new ModelResource();
	mr->load("D:\\RealityRendering\\Demo\\resourcecube.obj", imgMg);

	//5 Basic Shader for run engine
	OptixShader intesectShader;
	OptixShader boundShader;
	OptixShader exceptionShader;
	OptixShader bgShader;
	OptixShader rayGenShader;
	OptixShader closestShader;
	OptixShader anyHitShader;

	OptixGeometryShader geSH;
	geSH.setBoundShader(boundShader);
	geSH.setIntesectShader(intesectShader);

	//material
	OptixMaterial material ;
	
	// create Context
	OptixContext* context = new OptixContext();
	context->setViewPort(screenW, screenW);
	context->setExceptionShader(exceptionShader);
	context->setTopGeometryName("Top_Object");
	
	//create OptixModel
	OptixModelResource* opMR = new OptixModelResource();
	opMR->load(mr, context->getContext());
	OptixModel* opModel = new OptixModel();
	opModel->load(context, opMR, material, geSH);

	//create Camera
	OptixCamera* camera = new OptixCamera();
	camera->setName("MainCamera");
	camera->setBackGroundShader(bgShader);
	camera->setRayGenShader(rayGenShader);
	//View for render target
	OptixView* view = new OptixView(context->getContext());
	view->setViewSize(screenW, screenH);
	camera->setView(view);

	OptixScene scene;
	scene.addOptixCamera(camera);
	scene.setOptixContext(context);
	scene.addOptixModel(opModel);

	//scene.beforeRender();
	//scene.doRender();
	//Image* result = scene.getRenderResult();

	getchar();
	return 0;
}