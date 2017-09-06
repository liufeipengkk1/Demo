
#ifndef OPTIXCORE_OPTIXMESHRESOURCE_H
#define OPTIXCORE_OPTIXMESHRESOURCE_H

#include <iostream>
#include <optix_world.h>

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
	TextureSampler m_diffuseMap;
	TextureSampler m_normalMap;
	TextureSampler m_glossyMap;
	TextureSampler m_matallicMap;
	TextureSampler m_specularMap;
	TextureSampler m_emissionMap;
	TextureSampler m_reflectionMap;
	TextureSampler m_opacityMap;

	string m_name;
};

Buffer createBuffer(Context context, unsigned int type, RTformat format, int width, 
	 void* data, unsigned int elementSize);
Buffer createBuffer(Context context, unsigned int type, RTformat format, int width,
	int height, void * data, unsigned int elementSize);

#endif
