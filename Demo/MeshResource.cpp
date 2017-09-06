#include "MeshResource.h"
#include "Image.h"
#include "ImageManager.h"

MeshResource::MeshResource():
m_name(""),
m_diffuseMap(nullptr),
m_normalMap(nullptr),
m_glossyMap(nullptr),
m_matallicMap(nullptr),
m_specularMap(nullptr),
m_emissionMap(nullptr),
m_reflectionMap(nullptr),
m_opacityMap(nullptr)
{
	m_imageManager = nullptr;
}


MeshResource::~MeshResource()
{
	clean();
}

void MeshResource::clean() {
	m_position.clear();
	m_normal.clear();
	m_uvs.clear();
	m_tangent.clear();
	m_biTangnt.clear();

	if (m_imageManager) {
		m_imageManager->deleteImage(m_diffuseMap->getName());
		m_imageManager->deleteImage(m_normalMap->getName());
		m_imageManager->deleteImage(m_glossyMap->getName());
		m_imageManager->deleteImage(m_matallicMap->getName());
		m_imageManager->deleteImage(m_specularMap->getName());
		m_imageManager->deleteImage(m_emissionMap->getName());
		m_imageManager->deleteImage(m_reflectionMap->getName());
		m_imageManager->deleteImage(m_opacityMap->getName());
	}

	m_diffuseMap = nullptr;
	m_normalMap = nullptr;
	m_glossyMap = nullptr;
	m_matallicMap = nullptr;
	m_specularMap = nullptr;
	m_emissionMap = nullptr;
	m_reflectionMap = nullptr;
	m_opacityMap = nullptr;
}

bool MeshResource::load(aiMesh* mesh, const aiScene* scene, 
	ImageManager* imageManager, const string& directory) {
	if (nullptr != mesh && nullptr != scene && nullptr != imageManager) {
		m_imageManager = imageManager;
		if (mesh->HasFaces()) {
			for (int i = 0; i < mesh->mNumFaces; i++) {
				aiFace& face = mesh->mFaces[i];
				if (face.mNumIndices != 3) {
					cout << "MeshResource: error: the mesh is not triangle" << endl;
					m_position.clear();
					m_normal.clear();
					m_uvs.clear();
					m_tangent.clear();
					m_biTangnt.clear();
					return false;
				}
				else {
					for (int k = 0; k < 3; k++) {
						int index = face.mIndices[k];
						m_indices.push_back(index);
						m_position.push_back(mesh->mVertices[index].x);
						m_position.push_back(mesh->mVertices[index].y);
						m_position.push_back(mesh->mVertices[index].z);

						if (mesh->HasNormals()) {
							m_normal.push_back(mesh->mNormals[index].x);
							m_normal.push_back(mesh->mNormals[index].y);
							m_normal.push_back(mesh->mNormals[index].z);
						}

						if (mesh->mTextureCoords[0]) {
							m_uvs.push_back(mesh->mTextureCoords[0][index].x);
							m_uvs.push_back(mesh->mTextureCoords[0][index].y);
						}

						if (mesh->HasTangentsAndBitangents()) {
							m_tangent.push_back(mesh->mTangents[index].x);
							m_tangent.push_back(mesh->mTangents[index].y);
							m_tangent.push_back(mesh->mTangents[index].z);

							m_biTangnt.push_back(mesh->mBitangents[index].x);
							m_biTangnt.push_back(mesh->mBitangents[index].y);
							m_biTangnt.push_back(mesh->mBitangents[index].z);
						}
					}
				}
			}

			aiString name = mesh->mName;
			m_name = name.C_Str();

			if (mesh->mMaterialIndex >= 0) {
				m_diffuseMap = loadMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_DIFFUSE,
					directory, imageManager);

				m_normalMap = loadMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_NORMALS,
					directory, imageManager);

				m_glossyMap = loadMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_SHININESS,
					directory, imageManager);

				//assimp ��֧�ֽ����ʸе���ͼ
				//m_matallicMap = loadMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_SPECULAR,
				//	directory, imageManager);

				m_specularMap = loadMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_SPECULAR,
					directory, imageManager);
				
				m_emissionMap = loadMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_EMISSIVE,
					directory, imageManager);

				m_reflectionMap = loadMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_REFLECTION,
					directory, imageManager);

				m_opacityMap = loadMaterialTexture(scene->mMaterials[mesh->mMaterialIndex], aiTextureType_OPACITY,
					directory, imageManager);
			}

			return true;
		}
	}
}

Image* MeshResource::loadMaterialTexture(aiMaterial * material,
	aiTextureType type, const string& directory, ImageManager* imageManager)
{
	if (material->GetTextureCount(type) >= 0) {
		aiString filename;
		material->GetTexture(type, 0, &filename);

		string name(filename.C_Str());  // aiString ---> String

		name = directory + '/' + name;

		return imageManager->getImage(name);

	}
	
	return nullptr;
}

vector<float>& MeshResource::getPositon() {
	return m_position;
}

vector<float>& MeshResource::getNormal() {
	return m_normal;
}

vector<float>& MeshResource::getUV() {
	return m_uvs;
}

vector<float>& MeshResource::getTangent() {
	return m_tangent;
}

vector<float>& MeshResource::getBitangent() {
	return m_biTangnt;
}

vector<int>& MeshResource::getIndices() {
	return m_indices;
}

Image* MeshResource::getDiffuseMap() const{
	return m_diffuseMap;
}

Image* MeshResource::getNormalMap() const{
	return m_normalMap;
}

Image* MeshResource::getGlossyMap() const{
	return m_glossyMap;
}

Image* MeshResource::getMatallicMap() const{
	return m_matallicMap;
}
Image* MeshResource::getSpecularMap() const{
	return m_specularMap;
}

Image* MeshResource::getEmissinMap() const{
	return m_emissionMap;
}

Image* MeshResource::getReflectionMap() const{
	return m_reflectionMap;
}

Image* MeshResource::getOpacityMap() const{
	return m_opacityMap;
}

string MeshResource::getName() const {
	return m_name;
}

void MeshResource::setDiffuseMap(const string& name) {
	if (nullptr != m_diffuseMap)
	{
		if (nullptr == m_imageManager) {
			m_imageManager = ImageManager::getInstance();
		}

		m_imageManager->deleteImage(m_diffuseMap->getName());
		m_diffuseMap = nullptr;
	}

	m_diffuseMap = m_imageManager->getImage(name);
}

void MeshResource::setNormalMap(const string& name) {
	if (nullptr != m_normalMap)
	{
		if (nullptr == m_imageManager) {
			m_imageManager = ImageManager::getInstance();
		}

		m_imageManager->deleteImage(m_normalMap->getName());
		m_normalMap = nullptr;
	}

	m_normalMap = m_imageManager->getImage(name);
}

void MeshResource::setMatallicMap(const string& name) {
	if (nullptr != m_matallicMap)
	{
		if (nullptr == m_imageManager) {
			m_imageManager = ImageManager::getInstance();
		}

		m_imageManager->deleteImage(m_matallicMap->getName());
		m_matallicMap = nullptr;
	}

	m_matallicMap = m_imageManager->getImage(name);
}

void MeshResource::setSpecularMap(const string& name) {
	if (nullptr != m_specularMap)
	{
		if (nullptr == m_imageManager) {
			m_imageManager = ImageManager::getInstance();
		}

		m_imageManager->deleteImage(m_specularMap->getName());
		m_specularMap = nullptr;
	}

	m_specularMap = m_imageManager->getImage(name);
}

void MeshResource::setEmissinMap(const string& name) {
	if (nullptr != m_emissionMap)
	{
		if (nullptr == m_imageManager) {
			m_imageManager = ImageManager::getInstance();
		}

		m_imageManager->deleteImage(m_emissionMap->getName());
		m_emissionMap = nullptr;
	}

	m_emissionMap = m_imageManager->getImage(name);
}

void MeshResource::setReflectionMap(const string& name) {
	if (nullptr != m_reflectionMap)
	{
		if (nullptr == m_imageManager) {
			m_imageManager = ImageManager::getInstance();
		}

		m_imageManager->deleteImage(m_reflectionMap->getName());
		m_reflectionMap = nullptr;
	}

	m_reflectionMap = m_imageManager->getImage(name);
}

void MeshResource::setOpacityMap(const string& name) {
	if (nullptr != m_opacityMap)
	{
		if (nullptr == m_imageManager) {
			m_imageManager = ImageManager::getInstance();
		}

		m_imageManager->deleteImage(m_opacityMap->getName());
		m_opacityMap = nullptr;
	}

	m_opacityMap = m_imageManager->getImage(name);
}