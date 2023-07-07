#ifndef CONTROLOBJECTSCONNECTION_H
#define CONTROLOBJECTSCONNECTION_H

#include "src/widgets/view/graphics/controlpointitem.h"
#include "src/widgets/view/graphics/splineitem.h"

class PointWrapper {
public:

	explicit PointWrapper(ControlPointItem *point);

	ControlPointItem *getPoint() const;
	QGraphicsLineItem *getPrevLine() const;
	QGraphicsLineItem *getNextLine() const;
	SplineItem *getSpline() const;
	void setPrevLine(QGraphicsLineItem *prevLine);
	void setNextLine(QGraphicsLineItem *nextLine);
	void setSpline(SplineItem *spline);

private:

	ControlPointItem *m_point;
	QGraphicsLineItem *m_prevLine;
	QGraphicsLineItem *m_nextLine;
	SplineItem *m_spline;
};

#endif // CONTROLOBJECTSCONNECTION_H
