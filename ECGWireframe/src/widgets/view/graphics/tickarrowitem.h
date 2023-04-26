#ifndef TICKARROWITEM_H
#define TICKARROWITEM_H

#include <QGraphicsItem>
#include <QPen>
#include <QBrush>
#include <QPainter>

class TickArrowItem : public QGraphicsItem {
public:

	TickArrowItem(
			const QPointF &beginPoint,
			const QPointF &endPoint,
			qreal tickWidth,
			qreal tickStep,
			qreal tickStart,
			bool tickOnStart,
			qreal arrowWidth,
			qreal arrowLength,
			const QPen &pen = QPen(),
			const QBrush &brush = QBrush()
	);

private:

	QPointF m_beginPoint;
	QPointF m_endPoint;
	qreal m_tickWidth;
	qreal m_tickStep;
	qreal m_tickStart;
	bool m_tickOnStart;
	qreal m_arrowWidth;
	qreal m_arrowLength;
	QPen m_pen;
	QBrush m_brush;

	qreal m_length;

	QPointF m_dirVector;
	QPointF m_rRDirVector;
	QPointF m_rLDirVector;

	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // TICKARROWITEM_H
