#ifndef OPTIXCORE_OPTIXCONTEXT_
#define OPTIXCORE_OPTIXCONTEXT_

#include <iostream>
#include <optix_world.h>
#include "OptixShader.h"
#include "Reference.h"
#include "optixcore_base_define.h"
#include "SharePtr.h"

using namespace std;
using namespace optix;

class OptixContext :public Referenced
{
public:
	OptixContext();
	void setViewPort(int w, int h);
	void setPrintEnable(bool enable = false);
	void setCurEntryPoint(int idx);

	int getW() { return m_width; }
	int getH() { return m_height; }
	int getCurEntryPoint() { return m_curEntryPoint; }
	Context getContext() { return m_context; }

	~OptixContext();
	
	Handle<VariableObj> operator[](const string& varStr);

private:
	Context m_context;
	int m_width;
	int m_height;
	bool m_enablePrint;
	int m_curEntryPoint;
};

#endif