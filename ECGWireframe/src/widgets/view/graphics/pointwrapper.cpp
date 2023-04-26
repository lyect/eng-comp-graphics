#include "pointwrapper.h"

PointWrapper::PointWrapper(ControlPointItem *point) {
	m_point = point;
	m_prevLine = nullptr;
	m_nextLine = nullptr;
	m_spline = nullptr;
}

ControlPointItem *PointWrapper::getPoint() const {
	return m_point;
}

QGraphicsLineItem *PointWrapper::getPrevLine() const {
	return m_prevLine;
}

QGraphicsLineItem *PointWrapper::getNextLine() const {
	return m_nextLine;
}

SplineItem* PointWrapper::getSpline() const {
	return m_spline;
}

void PointWrapper::setPrevLine(QGraphicsLineItem *prevLine) {
	m_prevLine = prevLine;
}

void PointWrapper::setNextLine(QGraphicsLineItem *nextLine) {
	m_nextLine = nextLine;
}

void PointWrapper::setSpline(SplineItem *spline) {
	m_spline = spline;
}
