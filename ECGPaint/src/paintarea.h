#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QImage>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QColor>
#include <QStack>

#include "src/conf/settings.h"
#include "src/span.h"

class PaintArea : public QGraphicsView {

	Q_OBJECT

public:

	PaintArea(
			int paintSceneWidth,
			int paintSceneHeight,
			QColor backgroundColor,
			QWidget *parent = nullptr
	);

	int getWidth() const;
	int getHeight() const;

	void resize(const int &width, const int &height);
	void drawLine(
			const QPoint &p1,
			const QPoint &p2,
			const int &thickness,
			const QColor &color
	);
	void drawPolygon(
			const QPoint &p,
			const int &thickness,
			const int &vertices,
			const int &radius,
			const int &angle,
			const QColor &color
	);
	void drawStar(
			const QPoint &p,
			const int &thickness,
			const int &vertices,
			const int &radius,
			const int &angle,
			const QColor &color
	);
	void fill(
			const QPoint &p,
			const QColor &newColor
	);
	void clear();
	void drawImage(const QImage &image);

	bool canFit(const QImage &image) const;

	const QImage &getImage() const;

private:

	QColor backgroundColor;
	QGraphicsScene *paintScene;

	QImage newPaintImage;
	QImage paintImage;

	QGraphicsPixmapItem *lastPixmapItem;
	qint32 paintSceneWidth;
	qint32 paintSceneHeight;

	void redrawImage();

	QVector<QPoint> getPolygonPoints(
			const QPoint &p,
			const int &vertices,
			const int &radius,
			const int &angle
	);

	void drawCommonLine(
			const QPoint &p1,
			const QPoint &p2,
			const int &thickness,
			const QColor &color
	);
	void drawBresenhamLine(
			QPoint p1,
			QPoint p2,
			const QColor &color
	);
	void drawFigureEdge(
			const QPoint &p1,
			const QPoint &p2,
			const int &thickness,
			const QColor &color
	);
};

#endif // PAINTAREA_H
