#ifndef CORE_MESHRESOURCE_H
#define CORE_MESHRESOURCE_H

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Reference.h"

using namespace std;

class Image;
class ImageManager;

class MeshResource :public Referenced
{
public:
	MeshResource();
	bool load(aiMesh* mesh, const aiScene* scene, 
		ImageManager* imageManager, const string& directory);
	virtual ~MeshResource();

	void clean();

    vector<float>& getPositon();
	vector<float>& getNormal();
	vector<float>& getUV();
	vector<float>& getTangent();
	vector<float>& getBitangent();
	vector<int>& getIndices();

	void setDiffuseMap(const string& name);
	void setNormalMap(const string& name);
	void setMatallicMap(const string& name);
	void setSpecularMap(const string& name);
	void setEmissinMap(const string& name);
	void setReflectionMap(const string& name);
	void setOpacityMap(const string& name);
	
	Image* getDiffuseMap() const;
	Image* getNormalMap() const;
	Image* getGlossyMap() const;
	Image* getMatallicMap() const;
	Image* getSpecularMap() const;
	Image* getEmissinMap() const;
	Image* getReflectionMap() const;
	Image* getOpacityMap() const;

	string getName() const;

protected:
	Image* loadMaterialTexture(aiMaterial * material, 
		aiTextureType type, const string& directory, ImageManager* imageManager);
private:
	vector<float> m_position;
	vector<float> m_normal;
	vector<float> m_uvs;
	vector<float> m_tangent;
	vector<float> m_biTangnt;
	vector<int>   m_indices;

	ImageManager* m_imageManager;

	Image* m_diffuseMap;
	Image* m_normalMap;
	Image* m_glossyMap;
	Image* m_matallicMap;
	Image* m_specularMap;
	Image* m_emissionMap;
	Image* m_reflectionMap;
	Image* m_opacityMap;

	string m_name;
};

#endif
