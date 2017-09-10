
#ifndef OPTIXVIEW_H
#define OPTIXVIEW_H

#include <optix_world.h>
#include <iostream>

#include "Image.h"
#include "SharePtr.h"
#include "Reference.h"

using namespace std;
using namespace optix;

class OptixView:public Referenced
{
public:
	OptixView(Context context);
	~OptixView();

	void setViewSize(int w, int h);
	Image* getRenderResult();
	Buffer getRenderBuffer() { return m_renderBuffer; }

	int getW() { return m_w; }
	int getH() { return m_h; }

	string getName();
	void setName(const string& name);

private:
	OptixView(){}
	OptixView& operator=(const OptixView&) { return *this; }

private:
	SharePtr<Image>       m_renderImage;
	Buffer                m_renderBuffer;
	
	int m_w;
	int m_h;
	string  m_name;
	Context m_context;
};

#endif