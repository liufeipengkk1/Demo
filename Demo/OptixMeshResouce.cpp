#include "OptixMeshResource.h"
#include "Image.h"

const static int PER_POSITION_FACTOR = 3;
const static int PER_NORMAL_FACTOR = 3;
const static int PER_TANGENT_FACTOR = 3;
const static int PER_BITANGENT_FACTOR = 3;
const static int PER_UV_FACTOR = 2;
const static int PER_FACE_FACTOR = 3;

Buffer createBuffer(Context context, unsigned int type,
	RTformat format, int width, void* data, unsigned int elementSize) {
	Buffer buffer = context->createBuffer(type, format, width);
	
	void* bufferData = buffer->map();
	memcpy(bufferData, data, width * elementSize);
	buffer->unmap();

	return buffer;
}

Buffer createBuffer(Context context, unsigned int type, RTformat format, int width,
	int height, void * data, unsigned int elementSize) {

	Buffer buffer = context->createBuffer(type, format, width, height);

	void* bufferData = buffer->map();
	memcpy(bufferData, data, width * elementSize);
	buffer->unmap();

	return buffer;
}

//////////////////////////////////////////////////////////////////////

OptixMeshResource::OptixMeshResource() :
m_position(nullptr),
m_normal(nullptr),
m_uvs(nullptr),
m_tangent(nullptr),
m_biTangent(nullptr),
m_indices(nullptr),
m_diffuseMap(nullptr),
m_normalMap(nullptr),
m_glossyMap(nullptr),
m_matallicMap(nullptr),
m_specularMap(nullptr),
m_emissionMap(nullptr),
m_reflectionMap(nullptr),
m_opacityMap(nullptr)
{
	m_faceNums = 0;
}


OptixMeshResource::~OptixMeshResource()
{
	if (m_diffuseMap != nullptr) {
		delete m_diffuseMap;
		m_diffuseMap = nullptr;
	}
	if (m_normalMap != nullptr) {
		delete m_normalMap;
		m_normalMap = nullptr;
	}
	if (m_glossyMap != nullptr) {
		delete m_glossyMap;
		m_glossyMap = nullptr;
	}
	if (m_matallicMap != nullptr) {
		delete m_matallicMap;
		m_matallicMap = nullptr;
	}
	if (m_emissionMap != nullptr) {
		delete m_emissionMap;
		m_emissionMap = nullptr;
	}
	if (m_reflectionMap != nullptr) {
		delete m_reflectionMap;
		m_reflectionMap = nullptr;
	}
	if (m_opacityMap != nullptr) {
		delete m_opacityMap;
		m_opacityMap = nullptr;
	}
}


//triangle mesh
bool OptixMeshResource::setMeshCache(MeshResource* mesh,
	optix::Context context) {
	if (mesh == nullptr)
		return false;

	createMeshBuffer(mesh, context);
	createTextureSampler(mesh, context);
	
	return true;
}

void OptixMeshResource::createMeshBuffer(MeshResource* mesh, Context context) {
	m_position = createBuffer(context, RT_BUFFER_INPUT, RT_FORMAT_FLOAT3,
		mesh->getPositon().size() / PER_POSITION_FACTOR, &(mesh->getPositon()[0]), sizeof(float)*PER_POSITION_FACTOR);
	RTsize w = 0;
	m_position->getSize(w);
	m_normal = createBuffer(context, RT_BUFFER_INPUT, RT_FORMAT_FLOAT3,
		mesh->getNormal().size() / PER_NORMAL_FACTOR, &(mesh->getNormal()[0]), sizeof(float)*PER_NORMAL_FACTOR);

	m_uvs = createBuffer(context, RT_BUFFER_INPUT, RT_FORMAT_FLOAT2,
		mesh->getUV().size() / PER_UV_FACTOR, &(mesh->getUV()[0]), sizeof(float)*PER_UV_FACTOR);

	m_tangent = createBuffer(context, RT_BUFFER_INPUT, RT_FORMAT_FLOAT3,
		mesh->getTangent().size() / PER_TANGENT_FACTOR, &(mesh->getTangent()[0]), sizeof(float)*PER_TANGENT_FACTOR);

	m_biTangent = createBuffer(context, RT_BUFFER_INPUT, RT_FORMAT_FLOAT3,
		mesh->getBitangent().size() / PER_BITANGENT_FACTOR, &(mesh->getBitangent()[0]), sizeof(float)*PER_BITANGENT_FACTOR);

	m_indices = createBuffer(context, RT_BUFFER_INPUT, RT_FORMAT_INT3,
		mesh->getIndices().size() / PER_FACE_FACTOR, &(mesh->getIndices()[0]), sizeof(int)*PER_FACE_FACTOR);

	m_faceNums = mesh->getIndices().size() / PER_FACE_FACTOR;
}

//if we use optixCore.
//because in our system shader, we use rgba texture. so ,we need the texture channel equal to 4.
void OptixMeshResource::createTextureSampler(MeshResource* mesh, Context context) {
	m_diffuseMap = new OptixTexture2D(context);
	m_normalMap = new OptixTexture2D(context);
	m_glossyMap = new OptixTexture2D(context);
	m_matallicMap = new OptixTexture2D(context);
	m_specularMap = new OptixTexture2D(context);
	m_emissionMap = new OptixTexture2D(context);
	m_reflectionMap = new OptixTexture2D(context);
	m_opacityMap = new OptixTexture2D(context);

	m_diffuseMap->write(mesh->getDiffuseMap());
	m_normalMap->write(mesh->getNormalMap());
	m_glossyMap->write(mesh->getGlossyMap());
	m_matallicMap->write(mesh->getMatallicMap());
	m_specularMap->write(mesh->getSpecularMap());
	m_emissionMap->write(mesh->getEmissinMap());
	m_reflectionMap->write(mesh->getReflectionMap());
	m_opacityMap->write(mesh->getOpacityMap());
}

Buffer OptixMeshResource::getPositon() {
	return m_position;
}


Buffer OptixMeshResource::getNormal() {
	return m_normal;
}

Buffer OptixMeshResource::getUV() {
	return m_uvs;
}

Buffer OptixMeshResource::getTangent() {
	return m_tangent;
}

Buffer OptixMeshResource::getBitangent() {
	return m_biTangent;
}

Buffer OptixMeshResource::getIndices() {
	return m_indices;
}