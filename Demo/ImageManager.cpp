#include "ImageManager.h"
#include "Image.h"

ImageManager* ImageManager::m_instance = nullptr;

ImageManager::ImageManager()
{
}


ImageManager::~ImageManager()
{
}

ImageManager* ImageManager::getInstance() {
	if (nullptr == m_instance) {
		m_instance = new ImageManager();
	}

	return m_instance;
	return nullptr;
}

void ImageManager::release() {
	if (m_instance) {
		m_instance->clean();
		delete m_instance;
	}

	m_instance = nullptr;
}

void ImageManager::clean() {
	for (auto& image : m_images) {
		if (image.second != nullptr) {
			delete image.second;
			image.second = nullptr;
		}
	}

	m_images.clear();
}

void ImageManager::deleteImage(const string& name) {
	auto itr = m_images.find(name);
	if (itr != m_images.end()) {
		delete itr->second;
		m_images.erase(itr);
	}
}

Image* ImageManager::getImage(const string& path) {
	auto itr = m_images.find(path);
	if (itr == m_images.end()) {
		Image *image = new Image(path);
		image->setName(path);
		m_images[path] = image;
		if (image->getData() == nullptr)
		{
			delete image;
			image = nullptr;
		}
		return image;
	}

	return itr->second;

	return nullptr;
}

