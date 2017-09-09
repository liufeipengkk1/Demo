#ifndef OptixGeometryShader_H
#define OptixGeometryShader_H
#include <iostream>

#include "OptixShader.h"
#include "optixcore_base_define.h"
#include "SharePtr.h"

class OptixGeometryShader :public Referenced
{
public:
	OptixGeometryShader();
	~OptixGeometryShader();

	void setBoundShader(OptixShader& shader) {
		m_boundProgram = shader.getHandle();
	}
	void setIntesectShader(OptixShader& shader) {
		m_intesectProgram = shader.getHandle();
	}

	Program getBoundProgram() { return m_boundProgram; }
	Program getIntesectProgram() { return m_intesectProgram; }

	void setName(const string& name) { m_name = name; }
	string getName() const { return m_name; };

private:
	Program m_boundProgram;
	Program m_intesectProgram;
	std::string m_name;
};

#endif