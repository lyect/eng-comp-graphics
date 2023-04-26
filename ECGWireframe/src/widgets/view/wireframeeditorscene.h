#ifndef WIREFRAMEEDITORSCENE_H
#define WIREFRAMEEDITORSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QSet>

#include "src/widgets/view/graphics/tickarrowitem.h"
#include "src/widgets/view/graphics/pointwrapper.h"

class WireframeEditorScene : public QGraphicsScene {

	Q_OBJECT

public:

	explicit WireframeEditorScene(QObject *parent = nullptr);

	void repaintAxis(double viewZoomFactor);

	bool isControlPoint(const QPointF &scenePoint, const QTransform &transform);
	void addControlPoint(const QPointF &scenePoint, const QTransform &transform);
	void removeControlPoint(const QPointF &scenePoint, const QTransform &transform);

	void setSplinePartition(qsizetype splinePartition);

	void clear();
	QVector<QPointF> getControlPoints() const;

	void selectNextControlPoint();
	void selectPrevControlPoint();
	void deleteSelectedControlPoint();
	void updateSelectedControlPointCoordinates(qreal x, qreal y);

private:

	QVector<PointWrapper> m_wrappers;

	TickArrowItem *m_xAxis;
	TickArrowItem *m_yAxis;

	qsizetype m_splinePartition;

	QGraphicsItem* m_lastSelection;

	void addControlLines(int controlPointIndex);
	void removeControlLines(int controlPointIndex);

	void updateSplines(int firstControlPointIndex, int secondControlPointIndex);

signals:

	void infoUpdated(qsizetype controlPointsCount, qsizetype controlPointNumber, const QPointF &point);

private slots:

	void controlPointMoved();
	void updateSelection();
};

#endif // WIREFRAMEEDITORSCENE_H
