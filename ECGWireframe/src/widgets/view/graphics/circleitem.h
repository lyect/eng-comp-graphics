#ifndef CIRCLEITEM_H
#define CIRCLEITEM_H

#include <QGraphicsItem>
#include <QVector4D>
#include <QPainter>

class CircleItem : public QGraphicsItem {
public:

	CircleItem(
			const qreal *xAnglePtr,
			const qreal *yAnglePtr,
			const qreal *zAnglePtr,
			const QVector3D *cameraPointPtr,
			const qreal *znPtr,
			const qreal *matrixPtr,
			QGraphicsItem *parent = nullptr
	);

	void addPoint(const QVector4D &point);
	void setScale(qreal scale);
	void updateCircle(
			const qreal *xAnglePtr,
			const qreal *yAnglePtr,
			const qreal *zAnglePtr,
			const QVector3D *cameraPointPtr,
			const qreal *znPtr,
			const qreal *matrixPtr
	);

private:

	const qreal *m_xAnglePtr;
	const qreal *m_yAnglePtr;
	const qreal *m_zAnglePtr;

	const QVector3D *m_cameraPointPtr;
	const qreal *m_znPtr;

	const qreal *m_matrixPtr;

	QVector<QVector4D> m_points;

	qreal m_scale;

	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // CIRCLEITEM_H
