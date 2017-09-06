
#ifndef OPTIXCORE_OPTIX_TEXTURE2D_H
#define OPTIXCORE_OPTIX_TEXTURE2D_H

#include <optix_world.h>
#include <iostream>
using namespace std;
using namespace optix;

class Image;

class OptixTexture2D{
public:
	OptixTexture2D(Context& context);
	OptixTexture2D(Context context, Image* image);
	~OptixTexture2D();
	void setWrapModeR(RTwrapmode mode);
	void setWrapModeT(RTwrapmode mode);
	void setFilterMode(RTfiltermode min, RTfiltermode max, 
		RTfiltermode MiniMapFilter);
	void miplevelCount(int count = 1);
	void write(Image* image);
	void setUsage(unsigned int usage = RT_BUFFER_INPUT);

	bool isValid() { return m_isValid; }

	TextureSampler getHandle() { return m_handle; }
private:
	OptixTexture2D(){};
	void setFormat(RTformat format);
private:
	TextureSampler m_handle;
	int m_width;
	int m_height;
	int m_channels;

	RTwrapmode m_wrapModeR;
	RTwrapmode m_wrapModeS;
	RTfiltermode m_minFilter;
	RTfiltermode m_maxFilter;
	RTfiltermode m_mipFilter;
	RTformat m_internalFormat;
	unsigned int m_usage;
	int m_mipCount;
	Context m_context;

	bool m_isValid;
};

#endif
