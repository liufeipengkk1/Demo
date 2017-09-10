#ifndef OPTIXCORE_OPTIXSHADER_H
#define OPTIXCORE_OPTIXSHADER_H

#include <optix_world.h>
#include <iostream>

#include "Reference.h"

using namespace std;
using namespace optix;

typedef string PTXFunction;

class OptixShader :public Referenced
{
public:
	OptixShader();
	OptixShader(Context context, const string& ptxFile, PTXFunction function) {
		setShadingFunction(context, ptxFile, function);
	}
	virtual ~OptixShader();
	
	void setShadingFunction(Context context, const string& ptxFile, PTXFunction function);
	string getName();

	Handle<VariableObj> operator[](const std::string& varname);

	Program getHandle() { return m_program; }

private:
	PTXFunction  m_name;
	Program m_program;
};

#endif