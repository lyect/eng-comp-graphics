#include "controlpointitem.h"

#include <QGraphicsScene>

#include "src/common/constants.h"

// ##############
// #   PUBLIC   #
// ##############

ControlPointItem::ControlPointItem(qreal radius, const QPen &pen, const QBrush &brush) {
	m_radius = radius;
	m_pen = pen;
	m_brush = brush;

	m_pressed = false;
}

QPointF ControlPointItem::getCenter() const {
	return {x() + m_radius + m_pen.widthF(), y() + m_radius + m_pen.widthF()};
}

// ###############
// #   PRIVATE   #
// ###############

QRectF ControlPointItem::boundingRect() const {
	return QRectF(0, 0, 2 * m_radius, 2 * m_radius);
}

void ControlPointItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	Q_UNUSED(widget);

	QPen prevPainterPen = painter->pen();
	QBrush prevPainterBrush = painter->brush();

	if (option->state & QStyle::State_Selected) {
		QPen dashedPen = QPen(Constants::EDITOR_SELECTION_COLOR);
		dashedPen.setStyle(Qt::DashLine);

		painter->setPen(dashedPen);
		painter->setBrush(Qt::NoBrush);

		painter->drawRect(boundingRect());
	}

	if (m_pressed) {
		painter->setPen(m_pen);
		painter->setBrush(m_brush);
	}
	else {
		painter->setPen(m_pen);
		painter->setBrush(Qt::NoBrush);
	}
	painter->drawEllipse(boundingRect());

	painter->setPen(prevPainterPen);
	painter->setBrush(prevPainterBrush);
}

void ControlPointItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		m_pressed = true;
		update();
	}
	QGraphicsItem::mousePressEvent(event);
}

void ControlPointItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {

	QGraphicsItem::mouseMoveEvent(event);
	if (x() < 0) {
		setPos(0, y());
	}
	else if (x() + boundingRect().right() > scene()->width()) {
		setPos(scene()->width() - boundingRect().width(), y());
	}

	if (y() < 0) {
		setPos(x(), 0);
	}
	else if (y() + boundingRect().bottom() > scene()->height()) {
		setPos(x(), scene()->height() - boundingRect().height());
	}

	if (event->buttons() & Qt::LeftButton) {
		emit moved();
	}
}

void ControlPointItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		m_pressed = false;
		update();
	}
	QGraphicsItem::mouseReleaseEvent(event);
}
