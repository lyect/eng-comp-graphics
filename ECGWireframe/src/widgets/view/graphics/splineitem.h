#ifndef SPLINEITEM_H
#define SPLINEITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QPair>

class SplineItem : public QGraphicsItem {
public:

	explicit SplineItem(
			const QPointF &p1 = QPointF(),
			const QPointF &p2 = QPointF(),
			const QPointF &p3 = QPointF(),
			const QPointF &p4 = QPointF(),
			qsizetype splinePartition = 0,
			const QPen &pen = QPen(),
			QGraphicsItem *parent = nullptr
	);

	void updateSpline(
			const QPointF &p1,
			const QPointF &p2,
			const QPointF &p3,
			const QPointF &p4,
			qsizetype splinePartition
	);

private:

	QPointF m_p1;
	QPointF m_p2;
	QPointF m_p3;
	QPointF m_p4;
	qsizetype m_splinePartition;
	QPen m_pen;

	qreal ax, bx, cx, dx;
	qreal ay, by, cy, dy;

	std::function<qreal(qreal, qreal, qreal, qreal, qreal)> cubicCurve;

	QPair<qreal, qreal> findMinMaxOnCubicCurve(qreal a, qreal b, qreal c, qreal d, qreal tMin, qreal tMax) const;

	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // SPLINEITEM_H
