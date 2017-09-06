#include "Image.h"

#include <opencv2\opencv.hpp>

unsigned char* readImage(const string& filePath,
	unsigned int& width, unsigned int& height, unsigned int& channel){
	cv::Mat image;
	try{
		image = cv::imread(filePath);
	}
	catch (exception& e){
		cout << "Image: check file path" << endl;
		return nullptr;
	}
	
	width = image.cols;
	height = image.rows;
	channel = image.channels();

	unsigned char* imageData = new unsigned char[width * height * channel];
	
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			uchar * ptr = image.ptr(i);
			if (1 == channel) {
				imageData[i*width + j ] = ptr[j];
			}
			else if (3 == channel) {
				imageData[i*width * channel + j * channel] = ptr[channel * j + 2];
				imageData[i*width * channel + j * channel + 1] = ptr[channel * j + 1];
				imageData[i*width * channel + j * channel + 2] = ptr[channel * j];
			}
			else if (4 == channel) {
				imageData[i*width * channel + j * channel] = ptr[channel * j + 2];
				imageData[i*width * channel + j * channel + 1] = ptr[channel * j + 1];
				imageData[i*width * channel + j * channel + 2] = ptr[channel * j];
				imageData[i*width * channel + j * channel + 3] = ptr[channel * j + 3];
			}
		}
	}

	return imageData;
}

////////////////////////////////////////////////////////////////////

Image::Image() :
m_data(nullptr), m_width(0),
m_heigth(0), m_channel(0),
m_type(ImageType::NONE),
m_name("")
{
}


Image::~Image()
{
	if (m_data)
		delete[] m_data;
	m_data = nullptr;

	m_width = 0;
	m_heigth = 0;
	m_channel = 0;
	m_type = NONE;
	m_name = "";
}

Image::Image(const string& path) {
	setImage(path);
}

void Image::setImage(const string& path) {
	m_data = readImage(path, m_width, m_heigth, m_channel);
	switch (m_channel)
	{
	case 0:
		m_type = R;
		break;
	case 3:
		m_type = RGB;
		break;
	case 4:
		m_type = RGBA;
		break;
	default:
		break;
	}
}

