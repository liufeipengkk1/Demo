#include "OptixView.h"


OptixView::OptixView(Context context)
{
	m_context = context;
	m_renderImage = SharePtr<Image>(new Image());
}

void OptixView::setViewSize(int w, int h) {
	m_w = w;
	m_h = h;
	m_renderBuffer = m_context->createBuffer(RT_BUFFER_INPUT_OUTPUT, 
		RT_FORMAT_FLOAT4, m_w, m_h);
	m_renderImage->createEmptyImage(w, h, 4, RGBA);
}


Image* OptixView::getRenderResult() {
	unsigned char* data = (unsigned char*)m_renderBuffer->map();
	unsigned char* ptr = m_renderImage->getData();
	for (int i = 0 ; i < m_h; i++){
		for (int j = 0; j < m_w ; j++){
			ptr[4 * j] = *(data) * 255 + 0.5;
			ptr[4 * j + 1] = *(data + 1) * 255 + 0.5;
			ptr[4 * j + 2] = *(data + 2)* 255 + 0.5;
			ptr[4 * j + 3] = 255;
			data = data + 4;
		}
	}
	m_renderBuffer->unmap();

	return m_renderImage.get();
}


OptixView::~OptixView()
{
}
