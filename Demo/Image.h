
#ifndef CORE_IMAGE_H
#define CORE_IMAGE_H

#include <iostream>
#include <string>
#include "Reference.h"
using namespace std;

typedef enum ImageType
{
	RGB,
	RGBA,
	R,
	NONE
}ImageType;

class Image:public Referenced
{
public:
	Image();
	Image(const string& filePath);
	virtual ~Image();
	
	void setImage(const string& filePath);
	unsigned char* getData() { return m_data; };
	unsigned int getWidth() const { return m_width; }
	unsigned int getHeight() const { return m_heigth; }
	unsigned int getChannel() const { return m_channel; }
	ImageType getType() const { return m_type; }
	string getName() const { return m_name; }
	void setName(const string& name) { m_name = name; }

private:
	unsigned char* m_data;
	unsigned int   m_width;
	unsigned int   m_heigth;
	unsigned int   m_channel;
	ImageType      m_type;
	string         m_name;
};

unsigned char* readImage(const string& filePath, 
	unsigned int& widht, unsigned int& height,unsigned int& channel);

#endif