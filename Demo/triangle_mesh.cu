

#include <optix.h>
#include <optixu/optixu_math_namespace.h>
#include <optixu/optixu_matrix_namespace.h>
#include <optixu/optixu_aabb_namespace.h>
//#include "RayInfoStruct.h"

using namespace optix;

rtBuffer<float3> vertex_buffer;	//vertex
rtBuffer<float3> normal_buffer; //normal
rtBuffer<float2> texcoord_buffer; //uv
rtBuffer<float3> tangent_buffer;  //tangent
rtBuffer<float3> bigTangent_buffer;// bigTangent
rtBuffer<int3> tri_indice;

rtDeclareVariable(float2, texcoord, attribute texcoord, );
rtDeclareVariable(float3, geometric_normal, attribute geometric_normal, );
rtDeclareVariable(float3, shading_normal, attribute shading_normal, );
rtDeclareVariable(float3, tangent, attribute tangent, );
rtDeclareVariable(float3, bigTangent, attribute bigTangent, );
rtDeclareVariable(optix::Ray, ray, rtCurrentRay, );


RT_PROGRAM void intersect(int primIdx)
{
	const float3 p0 = vertex_buffer[tri_indice[primIdx].x];
	const float3 p1 = vertex_buffer[tri_indice[primIdx].y];
	const float3 p2 = vertex_buffer[tri_indice[primIdx].z];

	const float3 t1 = tangent_buffer[tri_indice[primIdx].x];
	const float3  bt1 = bigTangent_buffer[tri_indice[primIdx].x];
	//intersect ray with triangle
		
	float3 n;
	float  t, beta, gamma;
	if (intersect_triangle(ray, p0, p1, p2, n, t, beta, gamma)) {

		if (rtPotentialIntersection(t)) {

			geometric_normal = normalize(n);
			tangent = t1;
			bigTangent = bt1;
			if (normal_buffer.size() == 0) {
				shading_normal = geometric_normal;
			}
			else {
				float3 n0 = normal_buffer[3 * primIdx];
				float3 n1 = normal_buffer[3 * primIdx+1];
				float3 n2 = normal_buffer[3 * primIdx+2];
				//shading_normal = normalize(n1*beta + n2*gamma + n0*(1.0f - beta - gamma));
				shading_normal = geometric_normal;
			}

			if (texcoord_buffer.size() == 0) {
				texcoord = make_float2(0.0f, 0.0f);
			}
			else {
				float2 t0 = texcoord_buffer[3 * primIdx];
				float2 t1 = texcoord_buffer[3 * primIdx+1];
				float2 t2 = texcoord_buffer[3 * primIdx+2];
				texcoord = t1*beta + t2*gamma + t0*(1.0f - beta - gamma);
			}
			rtReportIntersection(0);
		}
	}
	
		/*
	//pom = t1 * p0p1 + t2 * p0p2;
	//==>m = t1*p1 + t2 * p2 + (1 - t1 - t2) * p0;
	//点M在三角形内满足 t1 + t2 < 1,t1 t2都小于0 ，求出t1 和t2 的作用则是用于方便求uv坐标和法向量
	
	float3 e0 = p1 - p0;
	float3 e1 = p2 - p0;

	float3 n = cross(e0, e1);
	float3 origin = ray.origin;
	float3 direction = ray.direction;
	float dt = dot(direction, n);  
	
	//相交
	if (dt != 0){
		float d = 1 / dot(direction, n);
		float t = dot(p0 - origin, n)*d;

		if (t >= ray.tmin && t <= ray.tmax){
			
			//计算交点
			float3 p = origin + t * direction;
			float3 R = p - p0;
			float3 Q1 = p1 - p0;
			float3 Q2 = p2 - p0;

			//R = t1* Q1 + t2 * Q2
			float Q1Q1 = dot(Q1, Q1);
			float Q2Q2 = dot(Q2, Q2);
			float RQ1 = dot(R, Q1);
			float RQ2 = dot(R, Q2);
			float Q1Q2 = dot(Q1, Q2);

			float dd = 1 / (Q1Q1 * Q2Q2 - Q1Q2*Q1Q2);
			float t1 = (Q2Q2*RQ1 - Q1Q2*RQ2) / dd;
			float t2 = (Q1Q1*RQ2 - Q1Q2*RQ1) / dd;

			if ((t1 + t2) <= 1 && t1 >= 0 && t2 >= 0){
				if (rtPotentialIntersection(t)){

					geometric_normal = normalize(n);
					if (normal_buffer.size() == 0) {
						shading_normal = geometric_normal;
					}
					else {
						float3 n0 = normal_buffer[3 * primIdx];
						float3 n1 = normal_buffer[3 * primIdx + 1];
						float3 n2 = normal_buffer[3 * primIdx + 2];
						shading_normal = normalize(n1*t1 + n2*t2 + n0*(1.0f - t1 - t2));
					}

					if (texcoord_buffer.size() == 0) {
						texcoord = make_float3(0.0f, 0.0f, 0.0f);
					}
					else {
						float2 tex0 = texcoord_buffer[3 * primIdx];
						float2 tex1 = texcoord_buffer[3 * primIdx + 1];
						float2 tex2 = texcoord_buffer[3 * primIdx + 2];
						texcoord = make_float3(tex1*t1 + tex2*t2 + tex0*(1.0f - t1 - t2));
					}

					rtReportIntersection(0);
				}
			}
		}

	}
	*/
	
}


RT_PROGRAM void mesh_bounds(int primIdx, float result[6])
{
	//const int3 v_idx = index_buffer[primIdx];
	/*const float3 v0 = vertex_buffer[v_idx.x];
	const float3 v1 = vertex_buffer[v_idx.y];
	const float3 v2 = vertex_buffer[v_idx.z];*/

	const float3 v0 = vertex_buffer[tri_indice[primIdx].x];
	const float3 v1 = vertex_buffer[tri_indice[primIdx].y];
	const float3 v2 = vertex_buffer[tri_indice[primIdx].z];

	const float  area = length(cross(v1 - v0, v2 - v0));

	optix::Aabb* aabb = (optix::Aabb*)result;

	if (area > 0.0f && !isinf(area)) {
		aabb->m_min = fminf(fminf(v0, v1), v2);
		aabb->m_max = fmaxf(fmaxf(v0, v1), v2);
	}
	else {
		aabb->invalidate();
	}
}
