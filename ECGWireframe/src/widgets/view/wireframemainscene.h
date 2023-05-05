#ifndef WIREFRAMEMAINSCENE_H
#define WIREFRAMEMAINSCENE_H

#include <QGraphicsScene>
#include <QVector3D>

#include "src/common/wireframe.h"
#include "src/widgets/view/graphics/formingitem.h"
#include "src/widgets/view/graphics/circleitem.h"

class WireframeMainScene : public QGraphicsScene {

	Q_OBJECT

public:

	explicit WireframeMainScene(QObject *parent = nullptr);

	void showWireframe(const Wireframe &wireframe, bool reset);

	void resetAngles();
	void setAngleX(qreal xAngle);
	void setAngleY(qreal yAngle);
	void setAngleZ(qreal zAngle);
	qreal getAngleX() const;
	qreal getAngleY() const;
	qreal getAngleZ() const;

	void setCameraPoint(const QVector3D &cameraPoint);
	qreal getZn() const;
	void setZn(qreal zn);

	void repaint();

private:

	QVector<FormingItem *> m_formings;
	QVector<CircleItem *> m_circles;

	qreal m_xAngle;
	qreal m_yAngle;
	qreal m_zAngle;

	QVector3D m_cameraPoint;
	qreal m_zn;

	qreal m_matrix[16];

	bool m_itemsFilled;
	bool m_figuresAdded;

	qreal m_scale;

	std::function<qreal(qreal, qreal, qreal, qreal, qreal)> cubicCurve;

	QPair<QVector<QPointF>, QVector<QPointF>> makeForming(const QVector<QPointF> &controlPoints, qsizetype splinePartition);
	void calculateMatrix(qreal w, qreal h);
	void showBlank();
	void showProjection();
};

#endif // WIREFRAMEMAINSCENE_H
