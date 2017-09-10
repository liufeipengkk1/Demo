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
	
	// create Context
	OptixContext* context = new OptixContext();
	context->setViewPort(screenW, screenH);
	Buffer hdr_buffer = context->getContext()->createBuffer(RT_BUFFER_INPUT_OUTPUT, RT_FORMAT_FLOAT4, screenW, screenH);
	context->getContext()["output_buffer"]->setBuffer(hdr_buffer);

	//create resource
	ImageManager* imgMg = ImageManager::getInstance();
	ModelResource* mr = new ModelResource();
	mr->load("D:\\RealityRendering\\Demo\\resource\\cube.obj", imgMg);

	//5 Basic Shader for run engine
	OptixShader intesectShader(context->getContext(), "cudaPtx\\triangle_mesh.cu.ptx", "intersect");
	OptixShader boundShader(context->getContext(), "cudaPtx\\triangle_mesh.cu.ptx", "mesh_bounds");
	OptixShader exceptionShader(context->getContext(), "cudaPtx\\cameraShader.cu.ptx", "exception");
	OptixShader bgShader(context->getContext(), "cudaPtx\\cameraShader.cu.ptx", "backGround");
	OptixShader rayGenShader(context->getContext(), "cudaPtx\\cameraShader.cu.ptx", "pinhole_camera");
	OptixShader closestShader(context->getContext(), "cudaPtx\\color.cu.ptx", "fragment");


	OptixGeometryShader geSH;
	geSH.setBoundShader(boundShader);
	geSH.setIntesectShader(intesectShader);

	//material
	OptixMaterial material ;
	material.setName("colorMaterial");
	material.setShader(OptixShaderType::OPTIX_SHADER, closestShader);
	material.linkShader(context->getContext());

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
	camera->setExceptionShader(exceptionShader);
	camera->setCameraParameter(60,
		(float)screenW / (float)screenH,
		make_float3(0, 0, 10),
		make_float3(0, 0, 0),
		make_float3(0, 1, 0));
	OptixView* view = new OptixView(context->getContext());
	view->setViewSize(screenW, screenH);
	camera->setView(view);
	camera->setGroupName("top_Object");
	camera->setBuffer("output_buffer");
	camera->update("eye", "u", "v", "w");

	OptixScene scene;
	scene.addOptixCamera(camera);
	scene.setOptixContext(context);
	scene.addOptixModel(opModel);
	scene.activeCamera("MainCamera");

	scene.beforeRender();
	scene.doRender();
	Image* result = scene.getRenderResult();

	getchar();
	return 0;
}