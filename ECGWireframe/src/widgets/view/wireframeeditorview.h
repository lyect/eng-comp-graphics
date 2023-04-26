#ifndef WIREFRAMEEDITORVIEW_H
#define WIREFRAMEEDITORVIEW_H

#include <QGraphicsView>
#include <QShowEvent>
#include <QWheelEvent>
#include <QMouseEvent>

#include "src/widgets/view/wireframeeditorscene.h"

class WireframeEditorView : public QGraphicsView {

	Q_OBJECT

public:

	explicit WireframeEditorView(QWidget *parent = nullptr);

	void clear();
	void setControlPoints(const QVector<QPointF> &points);
	QVector<QPointF> getControlPoints() const;

private:

	bool m_wasOpened;

	QVector<qreal> m_zoomFactors;
	qsizetype m_currentZoomFactorIndex;

	WireframeEditorScene *m_wfeScene;

	void resetZoom();
	void zoomIn();
	void zoomOut();

	void showEvent(QShowEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;

signals:

	void sizeUpdated(int width, int height);
	void infoUpdated(qsizetype controlPointsCount, qsizetype controlPointNumber, const QPointF &point);

public slots:

	void selectPrevControlPoint();
	void selectNextControlPoint();
	void addControlPointToCenter();
	void deleteSelectedControlPoint();
	void updateSelectedControlPointCoordinates(qreal x, qreal y);
	void sceneInfoUpdated(qsizetype controlPointsCount, qsizetype controlPointNumber, const QPointF &point);
	void setSplinePartition(qsizetype splinePartition);
};

#endif // WIREFRAMEEDITORVIEW_H
