
#ifndef OPTIXCORE_CAMERA_H
#define OPTIXCORE_CAMERA_H

#include <iostream>
#include "OptixNode.h"
#include "OptixShader.h"
#include "SharePtr.h"
#include "OptixView.h"
#include "OptixNode.h"
#include "OptixRenderState.h"
#include "OptixRenderEngine.h"
using namespace std;

//optix camera as a ray launcher
class OptixCamera:public OptixNode
{
public:
	OptixCamera();
	~OptixCamera();

	void setCameraParameter(const float& fov, const float& aspect,
		const float3& position, const float3& lookAt, const float3& wUp);

	void setRayGenShader(OptixShader* shader) {
		m_cameraShader = SharePtr<OptixShader>(shader);
	}

	void setBackGroundShader(OptixShader* shader) {
		m_bgShader = SharePtr<OptixShader>(shader);
	}

	void setView(OptixView* view);
	OptixView* getView();

	OptixShader* getORayGenShader() const { return m_cameraShader.get(); }
	OptixShader* getBackGroundShader() const { return m_bgShader.get(); }

	void beforRender(OptixRenderState& renderState);
	void doRender(OptixRenderState& renderState);

	float3 getPosition();
	float3 getU();
	float3 getV();
	float3 getW();

private:
	void calculateUVW();

private:
	
	float m_fov;
	float m_aspect;
	float3 m_up;
	float3 m_position;
	float3 m_lookAt;

	float3 m_u; //x
	float3 m_v; //y
	float3 m_w; //z

	SharePtr<OptixShader> m_cameraShader;
	SharePtr<OptixView> m_optixView;
	SharePtr<OptixShader> m_bgShader;
	OptixRenderEngine m_engine;
};

#endif