

#ifndef HELPER_H_
#define HELPER_H_
#include <optix_world.h>
using namespace optix;

#define SCENE_EPSILON 1.e-4f
#define SHADING_RAY 0
#define SHADOW_RAY 1
#define TRANSPARENT_RAY 2

static __device__ __inline__ float3 exp(const float3& x)
{
	return make_float3(exp(x.x), exp(x.y), exp(x.z));
}

static __device__ __inline__ float step(float min, float value)
{
	return value<min ? 0 : 1;
}

static __device__ __inline__ float3 mix(float3 a, float3 b, float x)
{
	return a*(1 - x) + b*x;
}

static __device__ __inline__ float3 schlick(float nDi, const float3& rgb)
{
	float r = fresnel_schlick(nDi, 5, rgb.x, 1);
	float g = fresnel_schlick(nDi, 5, rgb.y, 1);
	float b = fresnel_schlick(nDi, 5, rgb.z, 1);
	return make_float3(r, g, b);
}

static __device__ __inline__ float saturate_(const float c) {
	if (c < 0)
		return 0;
	else if (c > 1)
		return 1;
	else
		return c;
}

static __device__ __inline__ uchar4 make_color(const float3& c)
{
	return make_uchar4(static_cast<unsigned char>(saturate_(c.z)*255.99f),  /* B */
		static_cast<unsigned char>(saturate_(c.y)*255.99f),  /* G */
		static_cast<unsigned char>(saturate_(c.x)*255.99f),  /* R */
		255u);                                                 /* A */
}

static 
__device__ __inline__ void createONB(const float3 &n ,float3& U,float3& V,float3& W )
{
	W = normalize(n);

	U = cross(W, make_float3(0, 1, 0));
	
	//保证U在右边
	if (fabsf(U.x) < 0.001f && fabsf(U.y) < 0.001f && fabsf(U.z) < 0.001f)
		U = cross(W, make_float3(1.0f, 0.0f, 0.0f));
	
	U = normalize(U);
	V = cross(W, U);
}

//create reflection coordinate from normalize normal
static 
__device__ __inline__ void createONB(const float3& n, float3& U, float3 & V)
{
	U = cross(n, make_float3(0, 1, 0));

	if (dot(U, U) < 1.e-3f)
	{
		U = cross(n, make_float3(1, 0, 0));
	}

	V = cross(n, U);
}

//sample hemisphere 
static 
__device__ __inline__ void sampleUnitHemisphere(const float2 & sample,float3 &U,float3 &V,
								float3 &W, float3 & point)
{
	float phi = 2 * M_PIf * sample.x;
	float r = sqrt(sample.y);
	float x = r * cos(phi);
	float y = r * sin(phi);
	float z = 1.0f - x*x - y*y;
	z = z > 0.0f ? sqrt(z) : 0.0f;

	point = x*U + y*V + z*W;
}



static
__device__ __inline__ void ConcentricSampleDisk(float2 & sample){

	float phi, r;
	float x = 2 * sample.x - 1;
	float y = 2 * sample.y - 1;		//（a,b） [-1,1]

	//分为8个区域 
	if (x > -y){
		if (x > y){
			r = x;
			phi = M_PIf / 4 * (y / x);
		}
		else{
			r = y;
			phi = (M_PIf / 4)*(2 - x / y);
		}
	}
	else{
		if (x < y){
			r = -x;
			phi = (M_PIf / 4)*(4 + y / x);
		}
		else{
			r = -y;
			//x <= -y && x >= y
			//x == y == 0
			//这里有0的存在
			phi = y != 0 ? (M_PIf / 4)*(6 - x / y) : 0;
		}
	}

	float u = r* cosf(phi);
	float v = r*sinf(phi);

	sample.x = u;
	sample.y = v;
}

static 
__device__ __inline__ void  cosSampleUnitHemisphere(float2 & sample, float3 &U,
			float3 & V,float3 & W,float3 & point)
{
	ConcentricSampleDisk(sample);
	float x = sample.x;
	float y = sample.y;
	float z = sqrtf( 1 - x*x - y*y);

	z = z > 0 ? z : 0;
	
	point = U * x + V*y + W*z;
}

//专门用于glossy 模型的采样函数sample_phong_lobe
static
__host__ __device__ __inline__ optix::float3 sample_phong_lobe(optix::float2 sample,float exponent,
								float3 U,float3 V,float3 W)
{
	const float power = expf(logf(sample.y) / (exponent + 1.0f)); //costhta

	const float phi = sample.x * 2 * (float)M_PIf;
	const float scale = sqrtf(1.0 - power*power);
	
	const float x = cosf(phi) * scale;
	const  float y = sin(phi) * scale;
	const float z = power;

	return x *U + y*V + z*W;
}

static
__host__ __device__ __inline__ float3 sample_phong_lobe(const float2 & sample, float exponent,
										const float3 &U, const float3 &V, const float3 &W,
										float& pdf, float & bdf_val)
{
	const float cos_theta = powf(sample.y, 1.0f / (exponent + 1));	//跟上面那个开根号是一样的，实际上
	const float sin_theta = sqrtf(1 - cos_theta * cos_theta);
	const float phi = sample.x *2.0f * M_PIf;

	const float x = cosf(phi) * sin_theta;
	const float y = sinf(phi) * sin_theta;
	const float z = cos_theta;

	const float powered_cos = powf(cos_theta,exponent);

	pdf = (exponent + 1) / (2.0f*M_PIf) * powered_cos;

	bdf_val = (exponent + 2.0f) / (2.0f*M_PIf) * powered_cos;//		bdf_val代表什么

	return x*U + y*V + z*W;
	
}

//将某一点转化到屏幕的坐标
static
__host__ __device__ __inline__ float2 W2S(const float3 &pos,const float3 &eye, 
	const float3 & U, const float3 & V,const float3 & W)
{

	//投影 将空间的X,Y,Z投影到UVW空间中
	//pos-eye = x*u + y*v + z*w;
	float3 dv = pos - eye;

	float wlen = length(W);
	float ulen = length(U);
	float vlen = length(V);

	float3 u = normalize(U);
	float3 v = normalize(V);
	float3 w = normalize(W);

	float x = dot(dv, u);
	float y = dot(dv, v);
	float z = dot(dv, w);

	
	x = (x *wlen / z) / length(U);
	y = (y * wlen / z) / length(V);

	return make_float2(x, y)*0.5 + 0.5;
}

static
__host__ __device__ __inline__ float3 rgb2hsv(float3 rgb)
{
	float H, S, V;

	int R = 255.0 * rgb.x + 0.5;
	int G = 255.0 * rgb.y + 0.5;
	int B = 255.0 * rgb.z + 0.5;
	
	float max = optix::max(optix::max(R, G), B);
	float min = optix::min(optix::min(R, G), B);

	V = max;
	if (max == 0){
		H = S = V = 0;
		return;
	}
	S = (max - min) / max;
	if (max == min){
		H = 0;
		return;
	}

	if (R == max && G >= B)
		H = (float)(G - B) / (max - min) * 60;
	if (R == max && G < B)
		H = (float)(G - B) / (max - min) * 60 + 360;

	if (G == max) H = 120 + (B - R) / (max - min) * 60;
	if (B == max) H = 240 + (R - G) / (max - min) * 60;

	if (max == min)
		S = 0;
	else{
		S = 1 - min / max;
	}

	V = max;

	return make_float3(H, S, V);
}

static
__host__ __device__ __inline__ float rgb2gray(const float3 &rgb)
{
	return rgb.x*0.299 + rgb.y*0.587 + rgb.z*0.114;
}

static
__host__ __device__ __inline__ float rgb2light(const float3 &rgb)
{
	return max(max(rgb.x, rgb.y), rgb.z);
}


// Color space conversions
static __host__ __device__ __inline__ optix::float3 Yxy2XYZ(const optix::float3& Yxy)
{
	// avoid division by zero
	if (Yxy.z < 1e-4)
		return optix::make_float3(0.0f, 0.0f, 0.0f);

	return optix::make_float3(Yxy.y * (Yxy.x / Yxy.z),
		Yxy.x,
		(1.0f - Yxy.y - Yxy.z) * (Yxy.x / Yxy.z));
}

static __host__ __device__ __inline__ optix::float3 XYZ2rgb(const optix::float3& xyz)
{
	const float R = optix::dot(xyz, optix::make_float3(3.2410f, -1.5374f, -0.4986f));
	const float G = optix::dot(xyz, optix::make_float3(-0.9692f, 1.8760f, 0.0416f));
	const float B = optix::dot(xyz, optix::make_float3(0.0556f, -0.2040f, 1.0570f));
	return optix::make_float3(R, G, B);
}

static __host__ __device__ __inline__ optix::float3 Yxy2rgb(optix::float3 Yxy)
{
	using namespace optix;

	// avoid division by zero
	if (Yxy.z < 1e-4)
		return make_float3(0.0f, 0.0f, 0.0f);

	// First convert to xyz
	float3 xyz = make_float3(Yxy.y * (Yxy.x / Yxy.z),
		Yxy.x,
		(1.0f - Yxy.y - Yxy.z) * (Yxy.x / Yxy.z));

	const float R = dot(xyz, make_float3(3.2410f, -1.5374f, -0.4986f));
	const float G = dot(xyz, make_float3(-0.9692f, 1.8760f, 0.0416f));
	const float B = dot(xyz, make_float3(0.0556f, -0.2040f, 1.0570f));
	return make_float3(R, G, B);
}

static __host__ __device__ __inline__ optix::float3 rgb2Yxy(optix::float3 rgb)
{
	using namespace optix;

	// convert to xyz
	const float X = dot(rgb, make_float3(0.4124f, 0.3576f, 0.1805f));
	const float Y = dot(rgb, make_float3(0.2126f, 0.7152f, 0.0722f));
	const float Z = dot(rgb, make_float3(0.0193f, 0.1192f, 0.9505f));

	// avoid division by zero
	// here we make the simplifying assumption that X, Y, Z are positive
	float denominator = X + Y + Z;
	if (denominator < 1e-4)
		return make_float3(0.0f, 0.0f, 0.0f);

	// convert xyz to Yxy
	return make_float3(Y,
		X / (denominator),
		Y / (denominator));
}

static __host__ __device__ __inline__ optix::float3 tonemap(const optix::float3 &hdr_value, float Y_log_av, float Y_max)
{
	using namespace optix;

	float3 val_Yxy = rgb2Yxy(hdr_value);

	float Y = val_Yxy.x; // Y channel is luminance
	const float a = 0.04f;
	float Y_rel = a * Y / Y_log_av;
	float mapped_Y = Y_rel * (1.0f + Y_rel / (Y_max * Y_max)) / (1.0f + Y_rel);

	float3 mapped_Yxy = make_float3(mapped_Y, val_Yxy.y, val_Yxy.z);
	float3 mapped_rgb = Yxy2rgb(mapped_Yxy);

	return mapped_rgb;
}

#endif