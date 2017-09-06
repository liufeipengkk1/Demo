#include "OptixNode.h"

OptixNode::OptixNode()
{
	m_transform = optix::Matrix4x4::translate(optix::make_float3(0));
	m_name = "";
}


OptixNode::~OptixNode()
{
	m_transform = optix::Matrix4x4::translate(optix::make_float3(0));
	m_name = "";
}

void OptixNode::translate(const float3& translate) {
	Matrix4x4 tran = Matrix4x4::translate(translate);
	m_transform = tran * m_transform;
}
void OptixNode::rotation(const float& angle, const float3& axis) {
	float radiance = M_PIf * angle / 180.0;
	Matrix4x4 rot = Matrix4x4::rotate(radiance, axis);
	m_transform = rot * m_transform;
}

void OptixNode::scale(const float3& s) {
	Matrix4x4 sca = Matrix4x4::scale(s);
	m_transform = sca * m_transform;
}

Matrix4x4 OptixNode::getTransform() {
	return m_transform;
}

void OptixNode::setName(const string& name) {
	m_name = name;
}
