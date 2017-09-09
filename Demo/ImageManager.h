
#ifndef CORE_IMAGEMANAGER_H
#define CORE_IMAGEMANAGER_H

#include <iostream>
#include <map>
using namespace std;

#include "Image.h"

class ImageManager :public Referenced
{
public:
	static ImageManager* getInstance();
	static void release();

public:
	Image* getImage(const string& path);
	void deleteImage(const string& name);

protected:
	void clean();
private:
	ImageManager();
	~ImageManager();
	ImageManager(const ImageManager&){}
	ImageManager& operator=(const ImageManager&){ return *this; };

public:
	static ImageManager*  m_instance;
	
private:
	map<string, Image*> m_images;
};

#endif