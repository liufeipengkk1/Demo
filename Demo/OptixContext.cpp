#include "OptixContext.h"


OptixContext::OptixContext()
{
	m_context = Context::create();
	m_width = 640;
	m_height = 480;
	m_enablePrint = true;
	m_curEntryPoint = 0;

	m_context->setRayTypeCount(RAY_TYPE_COUNT_);
	m_context->setEntryPointCount(OPTIX_ENTRY_POINT_COUNTS);
	m_context->setStackSize(OPTIX_STACK_SIZE);

	m_context->setPrintEnabled(m_enablePrint);
	m_context->setPrintBufferSize(OPTIX_PRINT_BUFFER_SIZE);
}


OptixContext::~OptixContext()
{
}

void OptixContext::setExceptionShader(OptixShader& shader) {
	m_context->setExceptionProgram(Shading_Ray, shader.getHandle());
}

void OptixContext::setViewPort(int w, int h) {
	m_width = w;
	m_height = h;
}

Handle<VariableObj> OptixContext::operator[](const string& varStr) {
	return m_context[varStr];
}

void OptixContext::setPrintEnable(bool enable) {
	m_enablePrint = enable;
	m_context->setPrintEnabled(m_enablePrint);
}

void OptixContext::setCurEntryPoint(int idx) {
	Assert(idx > OPTIX_ENTRY_POINT_COUNTS);
	if (idx < OPTIX_ENTRY_POINT_COUNTS) {
		m_curEntryPoint = idx;
	}
}