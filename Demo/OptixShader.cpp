#include "OptixShader.h"


OptixShader::OptixShader()
{
}

OptixShader::~OptixShader()
{

}

void OptixShader::setName(const string& name) {
	m_name = name;
}

void OptixShader::setShadingFunction(Context context,
	const string& ptxFile, PTXFunction function) {
	m_program = context->createProgramFromPTXFile(ptxFile, function);
}
Handle<VariableObj> OptixShader::operator[](const std::string& varname) {
	return m_program[varname];
}