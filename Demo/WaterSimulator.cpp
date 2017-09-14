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


int main() {

	int screenW = 1280;
	int screenH = 720;
	// create Context
	OptixContext* context = new OptixContext();
	context->setViewPort(screenW, screenH);
	Buffer hdr_buffer = context->getContext()->createBuffer(RT_BUFFER_INPUT_OUTPUT,
		RT_FORMAT_FLOAT3, screenW, screenH);
	context->getContext()["hdr_buffer"]->setBuffer(hdr_buffer);
	context->getContext()["Y_log_av"]->setFloat(1.0f);
	context->getContext()["Y_max"]->setFloat(1.0e38f);

	//image manager module
	ImageManager* imgMg = ImageManager::getInstance();

	//5 Basic Shader for run engine
	OptixShader intesectShader(context->getContext(), "cudaPtx\\triangle_mesh.cu.ptx", "intersect");
	OptixShader boundShader(context->getContext(), "cudaPtx\\triangle_mesh.cu.ptx", "mesh_bounds");
	OptixShader exceptionShader(context->getContext(), "cudaPtx\\cameraShader.cu.ptx", "exception");
	OptixShader bgShader(context->getContext(), "cudaPtx\\cameraShader.cu.ptx", "backGround");
	OptixShader rayGenShader(context->getContext(), "cudaPtx\\cameraShader.cu.ptx", "pinhole_camera");
	OptixShader reflectionShader(context->getContext(), "cudaPtx\\color.cu.ptx", "fragment");
	OptixShader closestShaderColor(context->getContext(), "cudaPtx\\color.cu.ptx", "color");
	closestShaderColor["col"]->setFloat(1, 1, 1);

	//»·¾³hdr
	Image *evMapImg = new Image("image\\Sky_Mirrored_25.jpg");
	OptixTexture2D evMap(context->getContext());
	evMap.write(evMapImg);
	bgShader["envmap"]->setTextureSampler(evMap.getHandle());
	delete evMapImg;

	Image *waterNomImg = new Image("image\\T_Water_N.png");
	OptixTexture2D waterNormalMap(context->getContext());
	waterNormalMap.write(waterNomImg);
	reflectionShader["normalMap"]->setTextureSampler(waterNormalMap.getHandle());
	delete waterNomImg;

	OptixGeometryShader geSH;
	geSH.setBoundShader(boundShader);
	geSH.setIntesectShader(intesectShader);

	OptixMaterial pointLightMaterial;
	pointLightMaterial.setName("pointLightMaterial");
	pointLightMaterial.setShader(OptixShaderType::OPTIX_SHADER, closestShaderColor);
	pointLightMaterial.linkShader(context->getContext());

	OptixMaterial waterPlaneMaterial;
	waterPlaneMaterial.setName("waterPlaneMaterial");
	waterPlaneMaterial.setShader(OptixShaderType::OPTIX_SHADER, reflectionShader);
	waterPlaneMaterial.linkShader(context->getContext());

	OptixModel* pointLight = new OptixModel();
	pointLight->load(context, "D:\\RealityRendering\\Demo\\resource\\pointLight.obj", pointLightMaterial, geSH, imgMg);
	pointLight->scale(make_float3(3));
	pointLight->translate(make_float3(0,10,0));
	reflectionShader["pointLightPos"]->setFloat(0,10,0);
	reflectionShader["pointLightRadius"]->setFloat(1.3798);
	OptixModel* waterPlane = new OptixModel();
	waterPlane->load(context, "D:\\RealityRendering\\Demo\\resource\\waterPlane.FBX", waterPlaneMaterial, geSH, imgMg);
	waterPlane->rotation(90, make_float3(1, 0, 0));
	waterPlane->scale(make_float3(20,20,20));
	waterPlane->translate(make_float3(0, 0, 0));

	//create Camera
	OptixCamera* camera = new OptixCamera();
	camera->setName("MainCamera");
	camera->setBackGroundShader(bgShader);
	camera->setRayGenShader(rayGenShader);
	camera->setExceptionShader(exceptionShader);
	camera->setCameraParameter(60,
		(float)screenW / (float)screenH,
		make_float3(0, 2, 100),
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
	scene.addOptixModel(pointLight);
	scene.addOptixModel(waterPlane);
	scene.activeCamera("MainCamera");
	scene.beforeRender();
	
	cv::Mat image;
	image.create(cv::Size(screenW, screenH), CV_8UC4);
	unsigned int frames = 0;
	while (true) {

		scene.doRender();
		context->getContext()["frame_number"]->setUint(frames++);
		//float Y_log_av = 0;
		//float Y_max = 0;
		//const float inv_size = 1.0f / (screenW * screenH);
		//Buffer hdrBuff = context->getContext()["hdr_buffer"]->getBuffer();
		//float3 *buf = static_cast<float3*>(hdrBuff->map());
		//for (size_t i = 0; i < screenW * screenH; i++) {
		//	const float Y = luminanceCIE(buf[i]);
		//	Y_log_av += log(0.001f + Y);
		//	Y_max = std::max(Y_max, Y);
		//}
		//Y_log_av = exp(Y_log_av * inv_size);
		//hdrBuff->unmap();
		//context->getContext()["Y_log_av"]->setFloat(Y_log_av);
		//context->getContext()["Y_max"]->setFloat(Y_max);

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
		cv::imshow("renderTarget", image);
		if (cv::waitKey(30) == 27)
			break;
	}

	imgMg->release();

	return 0;
}