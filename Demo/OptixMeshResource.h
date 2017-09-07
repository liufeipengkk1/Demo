
#ifndef OPTIXCORE_OPTIXMESHRESOURCE_H
#define OPTIXCORE_OPTIXMESHRESOURCE_H

#include <iostream>
#include <optix_world.h>

#include "OptixTexture2D.h"
#include "MeshResource.h"

using namespace std;
using namespace optix;

class OptixMeshResource
{
public:
	OptixMeshResource();
	virtual ~OptixMeshResource();

	bool setMeshCache(MeshResource* mesh, optix::Context context);

	void setName(const string& name) { m_name = name; }
	string getName() const { return m_name; }

	Buffer getPositon();
	Buffer getNormal();
	Buffer getUV();
	Buffer getTangent();
	Buffer getBitangent();
	Buffer getIndices();

	OptixTexture2D* getDiffuseMap() const { return m_diffuseMap; }
	OptixTexture2D* getNormalMap() const { return m_normalMap; }
	OptixTexture2D* getGlossyMap() const { return m_glossyMap; }
	OptixTexture2D* getMatallicMap() const { return m_matallicMap; }
	OptixTexture2D* getSpecularMap() const { return m_specularMap; }
	OptixTexture2D* getEmissinMap() const { return m_emissionMap; }
	OptixTexture2D* getReflectionMap() const { return m_reflectionMap; }
	OptixTexture2D* getOpacityMap() const { return m_opacityMap; }

private:
	void createMeshBuffer(MeshResource* mesh, Context context);
	void createTextureSampler(MeshResource* mesh, Context context);

private:
	Buffer m_position;
	Buffer m_normal;
	Buffer m_uvs;
	Buffer m_tangent;
	Buffer m_biTangent;
	Buffer m_indices;

	//RGBA
	OptixTexture2D* m_diffuseMap;
	OptixTexture2D* m_normalMap;
	OptixTexture2D* m_glossyMap;
	OptixTexture2D* m_matallicMap;
	OptixTexture2D* m_specularMap;
	OptixTexture2D* m_emissionMap;
	OptixTexture2D* m_reflectionMap;
	OptixTexture2D* m_opacityMap;

	string m_name;
};

Buffer createBuffer(Context context, unsigned int type, RTformat format, int width, 
	 void* data, unsigned int elementSize);
Buffer createBuffer(Context context, unsigned int type, RTformat format, int width,
	int height, void * data, unsigned int elementSize);
#endif
