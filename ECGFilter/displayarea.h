#ifndef DISPLAYAREA_H
#define DISPLAYAREA_H

#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsLineItem>
#include <QImage>

#include "borderedgraphicsview.h"

class DisplayArea : public QWidget {

	Q_OBJECT

public:

	DisplayArea(QWidget *parent = nullptr);

	void drawImage(QImage image);
	QRect getViewportRect() const;

private:

	QVBoxLayout *mainLayout;

	BorderedGraphicsView *view;
	QGraphicsScene *scene;

	QGraphicsPixmapItem *lastPixmapItem;

	int sceneWidth;
	int sceneHeight;
};

#endif // DISPLAYAREA_H
