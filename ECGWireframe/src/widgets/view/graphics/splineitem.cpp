#include "splineitem.h"

#include "src/common/constants.h"

// ##############
// #   PUBLIC   #
// ##############

SplineItem::SplineItem(
		const QPointF &p1,
		const QPointF &p2,
		const QPointF &p3,
		const QPointF &p4,
		qsizetype splinePartition,
		const QPen &pen,
		QGraphicsItem *parent
) : QGraphicsItem(parent) {
	m_p1 = p1;
	m_p2 = p2;
	m_p3 = p3;
	m_p4 = p4;
	m_splinePartition = splinePartition;
	m_pen = pen;

	cubicCurve = [](qreal a, qreal b, qreal c, qreal d, qreal x) -> qreal {
		return a * x * x * x + b * x * x + c * x + d;
	};

	ax = (-m_p1.x() + 3 * m_p2.x() - 3 * m_p3.x() + m_p4.x()) / 6;
	bx = (3 * m_p1.x() - 6 * m_p2.x() + 3 * m_p3.x()) / 6;
	cx = (-3 * m_p1.x() + 3 * m_p3.x()) / 6;
	dx = (m_p1.x() + 4 * m_p2.x() + m_p3.x()) / 6;

	ay = (-m_p1.y() + 3 * m_p2.y() - 3 * m_p3.y() + m_p4.y()) / 6;
	by = (3 * m_p1.y() - 6 * m_p2.y() + 3 * m_p3.y()) / 6;
	cy = (-3 * m_p1.y() + 3 * m_p3.y()) / 6;
	dy = (m_p1.y() + 4 * m_p2.y() + m_p3.y()) / 6;

	setZValue(-1);
}

void SplineItem::updateSpline(
		const QPointF &p1,
		const QPointF &p2,
		const QPointF &p3,
		const QPointF &p4,
		qsizetype splinePartition
) {
	m_p1 = p1;
	m_p2 = p2;
	m_p3 = p3;
	m_p4 = p4;
	m_splinePartition = splinePartition;

	ax = (-m_p1.x() + 3 * m_p2.x() - 3 * m_p3.x() + m_p4.x()) / 6;
	bx = (3 * m_p1.x() - 6 * m_p2.x() + 3 * m_p3.x()) / 6;
	cx = (-3 * m_p1.x() + 3 * m_p3.x()) / 6;
	dx = (m_p1.x() + 4 * m_p2.x() + m_p3.x()) / 6;

	ay = (-m_p1.y() + 3 * m_p2.y() - 3 * m_p3.y() + m_p4.y()) / 6;
	by = (3 * m_p1.y() - 6 * m_p2.y() + 3 * m_p3.y()) / 6;
	cy = (-3 * m_p1.y() + 3 * m_p3.y()) / 6;
	dy = (m_p1.y() + 4 * m_p2.y() + m_p3.y()) / 6;

	prepareGeometryChange();
}

// ###############
// #   PRIVATE   #
// ###############

QPair<qreal, qreal> SplineItem::findMinMaxOnCubicCurve(qreal a, qreal b, qreal c, qreal d, qreal tMin, qreal tMax) const {
	qreal ad = 3 * a;
	qreal bd = 2 * b;
	qreal cd = c;
	qreal D = bd * bd - 4 * ad * cd;

	qreal min = qMin(cubicCurve(a, b, c, d, tMin), cubicCurve(a, b, c, d, tMax));
	qreal max = qMax(cubicCurve(a, b, c, d, tMin), cubicCurve(a, b, c, d, tMax));

	if (-Constants::EPSILON < D && D < Constants::EPSILON) { // derivative has one solution
		qreal t = -bd / (2 * ad);
		if (tMin - Constants::EPSILON < t && t < tMax + Constants::EPSILON) {
			min = qMin(min, cubicCurve(a, b, c, d, t));
			max = qMax(max, cubicCurve(a, b, c, d, t));
		}
	}
	else if (Constants::EPSILON <= D) { // derivative has two solutions
		qreal t0 = (-bd - qSqrt(D)) / (2 * ad);
		qreal t1 = (-bd + qSqrt(D)) / (2 * ad);
		if (tMin - Constants::EPSILON < t0 && t0 < tMax + Constants::EPSILON) {
			min = qMin(min, cubicCurve(a, b, c, d, t0));
			max = qMax(max, cubicCurve(a, b, c, d, t0));
		}
		if (tMin - Constants::EPSILON < t1 && t1 < tMax + Constants::EPSILON) {
			min = qMin(min, cubicCurve(a, b, c, d, t1));
			max = qMax(max, cubicCurve(a, b, c, d, t1));
		}
	}

	return {min, max};
}

QRectF SplineItem::boundingRect() const {
	auto [xMin, xMax] = findMinMaxOnCubicCurve(ax, bx, cx, dx, 0.0, 1.0);
	auto [yMin, yMax] = findMinMaxOnCubicCurve(ay, by, cy, dy, 0.0, 1.0);

	xMin = qMin(qMin(qMin(m_p1.x(), m_p2.x()), qMin(m_p3.x(), m_p4.x())), xMin);
	xMax = qMax(qMax(qMax(m_p1.x(), m_p2.x()), qMax(m_p3.x(), m_p4.x())), xMax);
	yMin = qMin(qMin(qMin(m_p1.y(), m_p2.y()), qMin(m_p3.y(), m_p4.y())), yMin);
	yMax = qMax(qMax(qMax(m_p1.y(), m_p2.y()), qMax(m_p3.y(), m_p4.y())), yMax);


	return {QPointF(xMin, yMin), QPointF(xMax, yMax)};
}

void SplineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(option);
	Q_UNUSED(widget);

	QPen prevPainterPen = painter->pen();

	painter->setPen(m_pen);

	QPointF prevPoint(cubicCurve(ax, bx, cx, dx, 0), cubicCurve(ay, by, cy, dy, 0));
	for (qsizetype i = 1; i <= m_splinePartition; ++i) {
		qreal t =  1.0 / m_splinePartition * i;
		QPointF currPoint(cubicCurve(ax, bx, cx, dx, t), cubicCurve(ay, by, cy, dy, t));

		painter->drawLine(prevPoint, currPoint);

		prevPoint = currPoint;
	}

	painter->setPen(prevPainterPen);
}
