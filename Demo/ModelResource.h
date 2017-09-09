
#ifndef CORE_MODELRESOURCE_H
#define CORE_MODELRESOURCE_H

#include <iostream>
#include <vector>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Reference.h"

using namespace std;

class MeshResource;
class ImageManager;

class ModelResource :public Referenced
{
public:
	ModelResource();
	virtual ~ModelResource();

	bool load(const string& path, ImageManager* imageManager);

	vector<MeshResource*> getMeshResources() const;

	MeshResource* getMesh(const string& name);
private:
	Assimp::Importer m_impoter;

	ImageManager* m_imageManger;
	vector<MeshResource*> m_meshResources;
	string m_name;
	string m_directory;
};

#endif