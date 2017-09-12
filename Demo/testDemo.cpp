#include <opencv2\opencv.hpp>
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

int screenW = 1280;
int screenH = 720;
int main() {
	cout << "liufeipeng" << endl;
	optix::Matrix4x4 transform = optix::Matrix4x4::translate(optix::make_float3(0));
	cout << transform << endl;
	
	// create Context
	OptixContext* context = new OptixContext();
	context->setViewPort(screenW, screenH);
	//create resource

	//image manager module
	ImageManager* imgMg = ImageManager::getInstance();

	//5 Basic Shader for run engine
	OptixShader intesectShader(context->getContext(), "cudaPtx\\triangle_mesh.cu.ptx", "intersect");
	OptixShader boundShader(context->getContext(), "cudaPtx\\triangle_mesh.cu.ptx", "mesh_bounds");
	OptixShader exceptionShader(context->getContext(), "cudaPtx\\cameraShader.cu.ptx", "exception");
	OptixShader bgShader(context->getContext(), "cudaPtx\\cameraShader.cu.ptx", "backGround");
	OptixShader rayGenShader(context->getContext(), "cudaPtx\\cameraShader.cu.ptx", "pinhole_camera");
	OptixShader closestShader(context->getContext(), "cudaPtx\\color.cu.ptx", "fragment");//法线测试
	OptixShader closestShaderColor(context->getContext(), "cudaPtx\\color.cu.ptx", "color");
	closestShaderColor["col"]->setFloat(1, 1, 0);

	//贴图测试
	Image *evMapImg = new Image("image\\hdr.jpg");
	OptixTexture2D evMap(context->getContext());
	evMap.write(evMapImg);
	bgShader["envmap"]->setTextureSampler(evMap.getHandle());
	delete evMapImg;

	OptixGeometryShader geSH;
	geSH.setBoundShader(boundShader);
	geSH.setIntesectShader(intesectShader);

	//material
	OptixMaterial material ;
	material.setName("colorMaterial");
	material.setShader(OptixShaderType::OPTIX_SHADER, closestShader);
	material.linkShader(context->getContext());

	OptixMaterial materialBox;
	materialBox.setName("boxMaterial");
	materialBox.setShader(OptixShaderType::OPTIX_SHADER, closestShaderColor);
	materialBox.linkShader(context->getContext());

	//create OptixModel
	OptixModel* opModel = new OptixModel();
	opModel->load(context, "D:\\RealityRendering\\Demo\\resource\\teapot.obj", material, geSH, imgMg);
	opModel->translate(make_float3(-10,0,0));

	OptixModel* opModel2 = opModel->clone("clone Model");
	opModel2->scale(make_float3(2, 2, 2));
	opModel2->rotation(90, make_float3(0, 1, 0)); // 用Trasform非常不保险，比如这样的旋转操作，似乎会使法线数据出现问题，还是在shader中手动计算，建议
	opModel2->translate(make_float3(20, 0, -10));

	//shader 复用性测试，几何shader进行复用
	OptixModel* cube = new OptixModel();
	cube->load(context, "D:\\RealityRendering\\Demo\\resource\\cube.obj", materialBox, geSH, imgMg);
	cube->translate(make_float3(-8, 5, 20));
	//create Camera
	OptixCamera* camera = new OptixCamera();
	camera->setName("MainCamera");
	camera->setBackGroundShader(bgShader);
	camera->setRayGenShader(rayGenShader);
	camera->setExceptionShader(exceptionShader);
	camera->setCameraParameter(60,
		(float)screenW / (float)screenH,
		make_float3(0, 0, 50),
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
	scene.addOptixModel(opModel2);
	scene.addOptixModel(cube);
	scene.activeCamera("MainCamera");

	scene.beforeRender();
	scene.doRender();
	Image* result = scene.getRenderResult();

	cv::Mat image;
	image.create(cv::Size(screenW, screenH), CV_8UC4);
	float * data = (float *)view->getRenderBuffer()->map();
	for (int i = image.rows - 1; i >= 0; i--){
		uchar * ptr = image.ptr(i);
		for (int j = 0; j < image.cols; j++){
			ptr[4 * j + 2] = *(data)* 255 + 0.5;
			ptr[4 * j + 1] = *(data + 1) * 255 + 0.5;
			ptr[4 * j] = *(data + 2) * 255 + 0.5;
			ptr[4 * j + 3] = 255;
			data = data + 4;

		}
	}
	context->getContext()["output_buffer"]->getBuffer()->unmap();

	while (true) {
		cv::imshow("renderTarget", image);

		if (cv::waitKey(30) == 27)
			break;
	}

	imgMg->release();

	return 0;
}