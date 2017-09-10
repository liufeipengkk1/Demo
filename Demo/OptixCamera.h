
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

	void setRayGenShader(OptixShader& shader) {
		m_cameraShader = shader;
	}

	void setBackGroundShader(OptixShader& shader) {
		m_bgShader = shader;
	}

	void setExceptionShader(OptixShader& shader);
	Program getExceptionProgram() { return m_exceptionShader.getHandle(); }

	void setBuffer(const string& bufferName);

	void setGroupName(const string& name);
	string getGroupName() { return m_topRootName; }

	void setView(OptixView* view){ m_optixView = SharePtr<OptixView>(view); }
	OptixView* getView() { return m_optixView.get(); }

	bool update(string eyeName, string eyeU, string eyeV, string eyeW);

	OptixShader getORayGenShader() const { return m_cameraShader; }
	OptixShader getBackGroundShader() const { return m_bgShader; }

	void beforRender(OptixRenderState& renderState);
	void doRender(OptixRenderState& renderState);

	float3 getPosition();
	float3 getU();
	float3 getV();
	float3 getW();

private:
	void calculateUVW();

private:
	
	string m_bufferName;
	string m_topRootName;
	float m_fov;
	float m_aspect;
	float3 m_up;
	float3 m_position;
	float3 m_lookAt;

	float3 m_u; //x
	float3 m_v; //y
	float3 m_w; //z

	string m_eyeName;
	string m_uName;
	string m_vName;
	string m_wName;

	OptixShader m_cameraShader;
	SharePtr<OptixView> m_optixView;
	OptixShader m_bgShader;
	OptixRenderEngine m_engine;
	OptixShader m_exceptionShader;
};

#endif