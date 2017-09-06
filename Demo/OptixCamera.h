
//#include <optix_world.h>
#include <iostream>
using namespace std;

class OptixCamera
{
public:
	OptixCamera();
	~OptixCamera();

	//void setCameraParameter(const float& fov, const float& aspect);
	

private:
	float m_fov;
	float m_aspect;
};

