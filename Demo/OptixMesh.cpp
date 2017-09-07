#include "OptixMesh.h"


const static char * VERTEX_BUFFER =  "vertex_buffer";
const static char * NORMAL_BUFFER =  "normal_buffer";
const static char * UV_BUFFER     = "texcoord_buffer";
const static char * TANGENT_BUFFER   = "tangent_buffer";
const static char * BITANGENT_BUFFER = "bigTangent_buffer";
const static char * INDICES_BUFFER    =  "tri_indice";

OptixMesh::OptixMesh()
{
}


OptixMesh::~OptixMesh()
{
}

bool OptixMesh::load(OptixMeshResource* meshResource, Context context,
	OptixGeometryShader& gshader) {
	if (meshResource == nullptr)
		return false;
	m_meshGeometry = context->createGeometry();
	m_meshGeometry->setPrimitiveCount(meshResource->getFaceNums());
	m_meshGeometry->setBoundingBoxProgram(gshader.getBoundProgram());
	m_meshGeometry->setIntersectionProgram(gshader.getIntesectProgram());

	m_name = meshResource->getName();

	m_meshGeometry[VERTEX_BUFFER]->setBuffer(meshResource->getPositon());
	m_meshGeometry[NORMAL_BUFFER]->setBuffer(meshResource->getNormal());
	m_meshGeometry[UV_BUFFER]->setBuffer(meshResource->getUV());
	m_meshGeometry[TANGENT_BUFFER]->setBuffer(meshResource->getTangent());
	m_meshGeometry[BITANGENT_BUFFER]->setBuffer(meshResource->getBitangent());
	m_meshGeometry[INDICES_BUFFER]->setBuffer(meshResource->getIndices());

	return true;
}