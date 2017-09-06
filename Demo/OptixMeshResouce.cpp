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

TextureSampler createRGBATextureFromImage(Context context,
	unsigned int type, Image* image) {
	if (image == nullptr)
		return nullptr;
	int width = image->getWidth();
	int height = image->getHeight();
	int channel = image->getChannel();
	ImageType type = image->getType();
	Buffer imageBuffer = context->createBuffer(RT_BUFFER_INPUT, RT_FORMAT_UNSIGNED_BYTE4, width, height);
	unsigned char * data = (unsigned char *)imageBuffer->map();
	unsigned char* imageData = image->getData();
	if (type == RGBA) {
		memcpy(data, imageData, width * height* channel);
	}
	else if (type == R) {
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++)
			{
				data[i*width * 4 + j * 4]     =  imageData[i*width + j];
				data[i*width * 4 + j * 4 + 1] = imageData[i*width + j];
				data[i*width * 4 + j * 4 + 2] = imageData[i*width + j];
				data[i*width * 4 + j * 4 + 3] = imageData[i*width + j];
			}
		}
	}
	else if (type == RGB) {
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++)
			{
				data[i*width * 4 + j * 4]     = imageData[i*width * channel + j * channel];
				data[i*width * 4 + j * 4 + 1] = imageData[i*width * channel + j * channel + 1];
				data[i*width * 4 + j * 4 + 2] = imageData[i*width * channel + j * channel + 2];
				data[i*width * 4 + j * 4 + 3] = 255;
			}
		}
	}

	imageBuffer->unmap();
	TextureSampler ts = context->createTextureSampler();
	ts->setWrapMode(0, RT_WRAP_REPEAT);
	ts->setWrapMode(1, RT_WRAP_REPEAT);
	ts->setFilteringModes(RT_FILTER_LINEAR, RT_FILTER_LINEAR, RT_FILTER_NONE);
	ts->setIndexingMode(RT_TEXTURE_INDEX_NORMALIZED_COORDINATES);
	ts->setReadMode(RT_TEXTURE_READ_NORMALIZED_FLOAT);
	ts->setMaxAnisotropy(1);
	ts->setMipLevelCount(1);
	ts->setArraySize(1);
	ts->setBuffer(imageBuffer);

	return  ts;
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

//if we use optixCore.
//because in our system shader, we use rgba texture. so ,we need the texture channel equal to 4.
void OptixMeshResource::createTextureSampler(MeshResource* mesh, Context context) {
	Buffer diffuseMap  = createRGBATextureFromImage(context, RT_BUFFER_INPUT, mesh->getDiffuseMap());
	Buffer normalMap   = createRGBATextureFromImage(context, RT_BUFFER_INPUT, mesh->getDiffuseMap());
	Buffer glossyMap   = createRGBATextureFromImage(context, RT_BUFFER_INPUT, mesh->getDiffuseMap());
	Buffer matallicMap = createRGBATextureFromImage(context, RT_BUFFER_INPUT, mesh->getDiffuseMap());
	Buffer specularMap = createRGBATextureFromImage(context, RT_BUFFER_INPUT, mesh->getDiffuseMap());
	Buffer emissionMap = createRGBATextureFromImage(context, RT_BUFFER_INPUT, mesh->getDiffuseMap());
	Buffer reflectionMap  = createRGBATextureFromImage(context, RT_BUFFER_INPUT, mesh->getDiffuseMap());
	Buffer opacityMap  = createRGBATextureFromImage(context, RT_BUFFER_INPUT, mesh->getDiffuseMap());
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