
#ifndef OPTIXCORE_SCENE_
#define OPTIXCORE_SCENE_

#include <iostream>
#include <optix_world.h>
#include <vector>
#include <map>
#include "SharePtr.h"
#include "OptixRenderState.h"
#include "optixcore_base_define.h"
#include "Image.h"

using namespace std;
using namespace optix;

class OptixModel;
class OptixCamera;
class OptixContext;

class OptixScene:public Referenced
{
public:
	OptixScene();
	~OptixScene();

	void setOptixContext(OptixContext* context);
	void addOptixModel(OptixModel* node);
	void addOptixCamera(OptixCamera* camera);
	void activeCamera(const string& name);
	void setAcceMethod(const string & method);
	
	OptixCamera* getActiveCamera();
	vector<SharePtr<OptixModel>>& getModels();
	string getAcceMethod();

	void beforeRender();
	void doRender();
	Image* getRenderResult();

private:
	vector<SharePtr<OptixModel>> m_models;
	vector<GeometryGroup> m_geGroup;
	map<string, SharePtr<OptixCamera>> m_cameras;
	OptixContext* m_context;
	OptixCamera* m_curCamera;
	string m_accMethod;
	Group m_group;
	OptixRenderState m_state;
};

#endif

