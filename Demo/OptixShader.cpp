#include "OptixShader.h"


OptixShader::OptixShader()
{
}

OptixShader::~OptixShader()
{

}

void OptixShader::setShadingFunction(Context context,
	const string& ptxFile, PTXFunction function) {
	m_program = context->createProgramFromPTXFile(ptxFile, function);
	m_name = function;
}
Handle<VariableObj> OptixShader::operator[](const std::string& varname) {
	return m_program[varname];
}