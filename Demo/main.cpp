#include <iostream>
#include <optix_world.h>

using namespace std;

class A{
public:
	int a;
	int b;
};

int main() {
	cout << "liufeipeng" << endl;
	optix::Matrix4x4 transform = optix::Matrix4x4::translate(optix::make_float3(0));
	cout << transform << endl;
	
	getchar();
	return 0;
}