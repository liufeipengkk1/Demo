

#include "OptixTexture2D.h"
#include "Image.h"

OptixTexture2D::OptixTexture2D(Context& context) {
	m_width = 0;
	m_height = 0;
	m_channels = 0;
	m_wrapModeR = RTwrapmode::RT_WRAP_REPEAT;
	m_wrapModeS = RTwrapmode::RT_WRAP_REPEAT;
	m_minFilter = m_maxFilter = RT_FILTER_LINEAR;
	m_mipFilter = RTfiltermode::RT_FILTER_NONE;
	m_mipCount = 1;
	m_handle = nullptr;
	m_usage = RT_BUFFER_INPUT;
	m_internalFormat = RT_FORMAT_UNSIGNED_BYTE4;
	m_context = context;
	m_isValid = false;
}

OptixTexture2D::OptixTexture2D(Context context, Image* image){
	m_width = 0;
	m_height = 0;
	m_channels = 0;
	m_wrapModeR = RTwrapmode::RT_WRAP_REPEAT;
	m_wrapModeS = RTwrapmode::RT_WRAP_REPEAT;
	m_minFilter = m_maxFilter = RT_FILTER_LINEAR;
	m_mipFilter = RTfiltermode::RT_FILTER_NONE;
	m_mipCount = 1;
	m_handle = nullptr;
	m_usage = RT_BUFFER_INPUT;
	m_internalFormat = RT_FORMAT_UNSIGNED_BYTE4;
	m_context = context;
	write(image);
	m_isValid = false;
}

OptixTexture2D::~OptixTexture2D(){
	m_width = 0;
	m_height = 0;
	m_channels = 0;
	m_wrapModeR = RTwrapmode::RT_WRAP_REPEAT;
	m_wrapModeS = RTwrapmode::RT_WRAP_REPEAT;
	m_minFilter = m_maxFilter = RT_FILTER_LINEAR;
	m_mipFilter = RTfiltermode::RT_FILTER_NONE;
	m_mipCount = 1;
	m_handle = nullptr;
	m_context = nullptr;
	m_isValid = false;
}

void OptixTexture2D::setWrapModeR(RTwrapmode mode) {
	m_wrapModeR = mode;
	m_isValid = false;
}
void OptixTexture2D::setWrapModeT(RTwrapmode mode) {
	m_wrapModeS = mode;
	m_isValid = false;
}
void OptixTexture2D::setFilterMode(RTfiltermode min, RTfiltermode max,
	RTfiltermode MiniMapFilter) {
	m_minFilter = min;
	m_maxFilter = max;
	m_mipFilter = MiniMapFilter;
	m_isValid = false;
}
void OptixTexture2D::miplevelCount(int count) {
	m_mipCount = count;
	m_isValid = false;
}

void OptixTexture2D::setFormat(RTformat format){
	m_internalFormat = format;
	m_isValid = false;
}

void OptixTexture2D::setUsage(unsigned int usage) {
	m_usage = usage;
	m_isValid = false;
}

//just allow rgba
void OptixTexture2D::write(Image* image) {
	if (image != nullptr)
	{
		int width = image->getWidth();
		int height = image->getHeight();
		int channel = image->getChannel();
		m_width = width;
		m_height = height;
		m_channels = channel;

		ImageType type = image->getType();
		Buffer imageBuffer = m_context->createBuffer(m_usage, m_internalFormat, width, height);
		unsigned char * data = (unsigned char *)imageBuffer->map();
		unsigned char* imageData = image->getData();
		if (type == RGBA) {
			memcpy(data, imageData, width * height* channel);
		}
		else if (type == R) {
			for (int i = 0; i < height; i++){
				for (int j = 0; j < width; j++)
				{
					data[i*width * 4 + j * 4] = imageData[i*width + j];
					data[i*width * 4 + j * 4 + 1] = imageData[i*width + j];
					data[i*width * 4 + j * 4 + 2] = imageData[i*width + j];
					data[i*width * 4 + j * 4 + 3] = imageData[i*width + j];
				}
			}
		}
		else if (type == RGB) {
			for (int i = 0; i < height; i++){
				for (int j = 0; j < width; j++)
				{
					data[i*width * 4 + j * 4] = imageData[i*width * channel + j * channel];
					data[i*width * 4 + j * 4 + 1] = imageData[i*width * channel + j * channel + 1];
					data[i*width * 4 + j * 4 + 2] = imageData[i*width * channel + j * channel + 2];
					data[i*width * 4 + j * 4 + 3] = 255;
				}
			}
		}

		imageBuffer->unmap();
		m_handle = m_context->createTextureSampler();
		m_handle->setWrapMode(0, m_wrapModeR);
		m_handle->setWrapMode(1, m_wrapModeS);
		m_handle->setFilteringModes(m_minFilter, m_maxFilter, m_mipFilter);
		m_handle->setIndexingMode(RT_TEXTURE_INDEX_NORMALIZED_COORDINATES);
		m_handle->setReadMode(RT_TEXTURE_READ_NORMALIZED_FLOAT);
		m_handle->setMaxAnisotropy(1);
		m_handle->setMipLevelCount(1);
		m_handle->setArraySize(1);
		m_handle->setBuffer(imageBuffer);
		m_isValid = true;
	}
	else {
		cout << "OptixTexture2D: image is null" << endl;
		m_isValid = false;
	}
}