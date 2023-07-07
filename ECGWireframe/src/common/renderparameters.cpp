#include "renderparameters.h"

RenderParameters::RenderParameters() {
	m_N = -1;
	m_M1 = -1;
	m_M2 = -1;
}

RenderParameters::RenderParameters(
		int N,
		int M1,
		int M2
) {
	m_N = N;
	m_M1 = M1;
	m_M2 = M2;
}

void RenderParameters::setN(int N) {
	m_N = N;
}

void RenderParameters::setM1(int M1) {
	m_M1 = M1;
}

void RenderParameters::setM2(int M2) {
	m_M2 = M2;
}

int RenderParameters::getN() const {
	return m_N;
}

int RenderParameters::getM1() const {
	return m_M1;
}

int RenderParameters::getM2() const {
	return m_M2;
}
