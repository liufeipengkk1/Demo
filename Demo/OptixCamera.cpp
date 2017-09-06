#include "OptixCamera.h"


OptixCamera::OptixCamera() :OptixNode()
{
}


OptixCamera::~OptixCamera()
{
}

void OptixCamera::setCameraParameter(const float& fov, const float& aspect, 
	const float3& position, const float3& lookAt, const float3& wUp) {
	m_fov = fov;
	m_aspect = aspect;
	m_position = position;
	m_lookAt = lookAt;
	m_up = wUp;
	calculateUVW();
}

void OptixCamera::calculateUVW() {
	float ulen, vlen, wlen;

	m_w = m_lookAt - m_position; 
	wlen = length(m_w);
	m_u = normalize(cross(m_w, m_up));	//x
	m_v = normalize(cross(m_u, m_w));   //y

	vlen = wlen * tanf(0.5 * m_fov*M_PIf / 180);
	ulen = vlen*m_aspect;
	m_u *= ulen;
	m_v *= vlen;
}

float3 OptixCamera::getPosition() {
	float4 position = m_transform * make_float4(m_position, 1);
	return make_float3(position.x, position.y, position.z);
}

float3 OptixCamera::getU() {
	float4 u = make_float4(m_u.x, m_u.y, m_u.z, 0);
	u = m_transform * u;
	return make_float3(u.x, u.y, u.z);
}

float3 OptixCamera::getV() {
	float4 v = make_float4(m_v.x, m_v.y, m_v.z, 0);
	v = m_transform * v;
	return make_float3(v.x, v.y, v.z);
}

float3 OptixCamera::getW() {
	float4 w = make_float4(m_w.x, m_w.y, m_w.z, 0);
	w = m_transform * w;
	return make_float3(w.x, w.y, w.z);
}