#include "ModelResource.h"
#include "MeshResource.h"
#include "ImageManager.h"

ModelResource::ModelResource()
{
	m_name = "";
	m_directory = "";
	m_imageManger = nullptr;
}


ModelResource::~ModelResource()
{
	for (auto& meshResource : m_meshResources) {
		delete meshResource;
		meshResource = nullptr;
	}

	m_meshResources.clear();
}

bool  ModelResource::load(const string& path, ImageManager* imageManager) {
	const aiScene * scene = m_impoter.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (imageManager == nullptr) {
		m_imageManger = ImageManager::getInstance();
	}
	else {
		m_imageManger = imageManager;
	}
	//check error
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ModelResource:Error load:: " << m_impoter.GetErrorString() << endl;
		exit(0);
	}
#ifdef _WINDOWS
	m_directory = path.substr(0, path.find_last_of('\\'));
	m_name = path.substr(path.find_last_of('\\')+1);
#else
	m_directory = path.substr(0, path.find_last_of('/'));
	m_name = path.substr(path.find_last_of('/'));
#endif

	for (int i = 0; i < scene->mNumMeshes; i++) {
		MeshResource* meshResource = new MeshResource();
		meshResource->load(scene->mMeshes[i], scene, imageManager, m_directory);
		m_meshResources.push_back(meshResource);
	}
}

vector<MeshResource*> ModelResource::getMeshResources() const {
	return m_meshResources;
}

MeshResource* ModelResource::getMesh(const string& name) {
	for (auto& mesh : m_meshResources) {
		if (mesh->getName() == name) {
			return mesh;
		}
	}

	return nullptr;
}