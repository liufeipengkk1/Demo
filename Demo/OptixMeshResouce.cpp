#include "OptixMeshResource.h"

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
m_indices(nullptr)
{
	m_diffuseMap = nullptr;
}


OptixMeshResource::~OptixMeshResource()
{
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

	m_normal = createBuffer(context, RT_BUFFER_INPUT, RT_FORMAT_FLOAT3,
		mesh->getNormal().size() / PER_NORMAL_FACTOR, &(mesh->getNormal()[0]), sizeof(float)*PER_NORMAL_FACTOR);

	m_uvs = createBuffer(context, RT_BUFFER_INPUT, RT_FORMAT_FLOAT3,
		mesh->getUV().size() / PER_UV_FACTOR, &(mesh->getUV()[0]), sizeof(float)*PER_UV_FACTOR);

	m_tangent = createBuffer(context, RT_BUFFER_INPUT, RT_FORMAT_FLOAT3,
		mesh->getTangent().size() / PER_TANGENT_FACTOR, &(mesh->getTangent()[0]), sizeof(float)*PER_TANGENT_FACTOR);

	m_biTangent = createBuffer(context, RT_BUFFER_INPUT, RT_FORMAT_FLOAT3,
		mesh->getBitangent().size() / PER_BITANGENT_FACTOR, &(mesh->getBitangent()[0]), sizeof(float)*PER_BITANGENT_FACTOR);

	m_indices = createBuffer(context, RT_BUFFER_INPUT, RT_FORMAT_INT3,
		mesh->getIndices().size() / PER_FACE_FACTOR, &(mesh->getIndices()[0]), sizeof(int)*PER_FACE_FACTOR);
}

void OptixMeshResource::createTextureSampler(MeshResource* mesh, Context context) {

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