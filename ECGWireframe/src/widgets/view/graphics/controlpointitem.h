#ifndef CONTROLPOINTITEM_H
#define CONTROLPOINTITEM_H

#include <QGraphicsObject>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>

class ControlPointItem : public QGraphicsObject {

	Q_OBJECT

public:

	explicit ControlPointItem(qreal radius, const QPen &pen = QPen(), const QBrush &brush = QBrush());

	QPointF getCenter() const;

private:

	qreal m_radius;
	QPen m_pen;
	QBrush m_brush;

	bool m_pressed;

	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

signals:

	void moved();
};

#endif // CONTROLPOINTITEM_H
