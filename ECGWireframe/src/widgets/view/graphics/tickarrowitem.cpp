#include "tickarrowitem.h"

#include "src/common/constants.h"

// ##############
// #   PUBLIC   #
// ##############

TickArrowItem::TickArrowItem(
		const QPointF &beginPoint,
		const QPointF &endPoint,
		qreal tickWidth,
		qreal tickStep,
		qreal tickStart,
		bool tickOnStart,
		qreal arrowWidth,
		qreal arrowLength,
		const QPen &pen,
		const QBrush &brush
) {
	m_beginPoint = beginPoint;
	m_endPoint = endPoint;
	m_tickWidth = tickWidth;
	m_tickStep = tickStep;
	m_tickStart = tickStart;
	m_tickOnStart = tickOnStart;
	m_arrowWidth = arrowWidth;
	m_arrowLength = arrowLength;
	m_pen = pen;
	m_brush = brush;

	// Calculate length of the arrow
	qreal xDelta = m_endPoint.x() - m_beginPoint.x();
	qreal yDelta = m_endPoint.y() - m_beginPoint.y();
	m_length = qSqrt(xDelta * xDelta + yDelta * yDelta);

	// Calculate a direction vector
	m_dirVector = QPointF(xDelta / m_length, yDelta / m_length);

	// Rotate direction vector
	m_rRDirVector = QPointF(-m_dirVector.y(), m_dirVector.x());
	m_rLDirVector = QPointF(m_dirVector.y(), -m_dirVector.x());

	setZValue(-3);
}

// ###############
// #   PRIVATE   #
// ###############

QRectF TickArrowItem::boundingRect() const {
	qreal grWidth = qMax(m_tickWidth, m_arrowWidth);

	QPointF bl(m_beginPoint.x() + m_rLDirVector.x() * grWidth, m_beginPoint.y() + m_rLDirVector.y() * grWidth);
	QPointF br(m_beginPoint.x() + m_rRDirVector.x() * grWidth, m_beginPoint.y() + m_rRDirVector.y() * grWidth);
	QPointF el(m_endPoint.x() + m_rLDirVector.x() * grWidth, m_endPoint.y() + m_rLDirVector.y() * grWidth);
	QPointF er(m_endPoint.x() + m_rRDirVector.x() * grWidth, m_endPoint.y() + m_rRDirVector.y() * grWidth);

	qreal xMax = qMax(qMax(bl.x(), br.x()), qMax(el.x(), er.x()));
	qreal xMin = qMin(qMin(bl.x(), br.x()), qMin(el.x(), er.x()));
	qreal yMax = qMax(qMax(bl.y(), br.y()), qMax(el.y(), er.y()));
	qreal yMin = qMin(qMin(bl.y(), br.y()), qMin(el.y(), er.y()));

	return QRectF(QPointF(xMin, yMin), QPointF(xMax, yMax));
}

void TickArrowItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(option);
	Q_UNUSED(widget);

	QPen prevPainterPen = painter->pen();
	QBrush prevPainterBrush = painter->brush();

	painter->setPen(m_pen);
	painter->setBrush(m_brush);

	// Line
	painter->drawLine(m_beginPoint, m_endPoint);

	// Arrow head
	QPolygonF headPolygon;
	headPolygon.append(m_endPoint);
	headPolygon.append(QPointF(
				m_endPoint.x() - m_dirVector.x() * m_arrowLength + m_rLDirVector.x() * m_arrowWidth,
				m_endPoint.y() - m_dirVector.y() * m_arrowLength + m_rLDirVector.y() * m_arrowWidth
	));
	headPolygon.append(QPointF(
				m_endPoint.x() - m_dirVector.x() * m_arrowLength + m_rRDirVector.x() * m_arrowWidth,
				m_endPoint.y() - m_dirVector.y() * m_arrowLength + m_rRDirVector.y() * m_arrowWidth
	));
	painter->drawPolygon(headPolygon);

	// Ticks
	QPointF ticksStartPoint(
				m_beginPoint.x() + m_dirVector.x() * m_length * m_tickStart,
				m_beginPoint.y() + m_dirVector.y() * m_length * m_tickStart
	);
	QPointF headStartPoint(
				m_endPoint.x() - m_dirVector.x() * m_arrowLength,
				m_endPoint.y() - m_dirVector.y() * m_arrowLength
	);

	auto pointComparatorLess = [](const QPointF &a, const QPointF &b) -> bool {
		if (a.x() <= b.x() - Constants::EPSILON) {
			return true;
		}
		else if (b.x() - Constants::EPSILON < a.x() && a.x() < b.x() + Constants::EPSILON) {
			return (a.y() >= b.y() + Constants::EPSILON);
		}
		return false;
	};

	QPointF currentPoint;
	if (m_tickOnStart) {
		currentPoint = ticksStartPoint;
	}
	else {
		currentPoint.setX(ticksStartPoint.x() + m_dirVector.x() * m_tickStep);
		currentPoint.setY(ticksStartPoint.y() + m_dirVector.y() * m_tickStep);
	}
	while (pointComparatorLess(currentPoint, headStartPoint)) {
		QPointF tickStart(
					currentPoint.x() + m_rLDirVector.x() * m_tickWidth,
					currentPoint.y() + m_rLDirVector.y() * m_tickWidth
		);
		QPointF tickEnd(
					currentPoint.x() + m_rRDirVector.x() * m_tickWidth,
					currentPoint.y() + m_rRDirVector.y() * m_tickWidth
		);
		painter->drawLine(tickStart, tickEnd);
		currentPoint.setX(currentPoint.x() + m_dirVector.x() * m_tickStep);
		currentPoint.setY(currentPoint.y() + m_dirVector.y() * m_tickStep);
	}

	currentPoint.setX(ticksStartPoint.x() - m_dirVector.x() * m_tickStep);
	currentPoint.setY(ticksStartPoint.y() - m_dirVector.y() * m_tickStep);
	while (pointComparatorLess(m_beginPoint, currentPoint)) {
		QPointF tickStart(
					currentPoint.x() + m_rLDirVector.x() * m_tickWidth,
					currentPoint.y() + m_rLDirVector.y() * m_tickWidth
		);
		QPointF tickEnd(
					currentPoint.x() + m_rRDirVector.x() * m_tickWidth,
					currentPoint.y() + m_rRDirVector.y() * m_tickWidth
		);
		painter->drawLine(tickStart, tickEnd);
		currentPoint.setX(currentPoint.x() - m_dirVector.x() * m_tickStep);
		currentPoint.setY(currentPoint.y() - m_dirVector.y() * m_tickStep);
	}

	painter->setPen(prevPainterPen);
	painter->setBrush(prevPainterBrush);
}
