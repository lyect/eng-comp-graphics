#include "displayarea.h"

//#include "constants.h"

DisplayArea::DisplayArea(QWidget *parent) : QWidget(parent) {
	mainLayout = new QVBoxLayout(this);

	view = new BorderedGraphicsView(this);
	scene = new QGraphicsScene(this);
	view->setScene(scene);
	view->setDragMode(QGraphicsView::ScrollHandDrag);
	view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

	mainLayout->addWidget(view);

	setLayout(mainLayout);

	lastPixmapItem = nullptr;

	sceneWidth = 0;
	sceneHeight = 0;
}
/*
void DisplayArea::setRealSizedDisplayMode() {
	displayMode = Constants::DisplayMode::REAL_SIZED;
	view->resetTransform();
	update();
}

void DisplayArea::setFittedDisplayMode() {
	displayMode = Constants::DisplayMode::FITTED;
	view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
	update();
}
*/
void DisplayArea::drawImage(QImage image) {
	if (lastPixmapItem != nullptr) {
		scene->removeItem(lastPixmapItem);
		delete lastPixmapItem;
	}

	view->setSceneRect(0, 0, image.width(), image.height());
	scene->setSceneRect(0, 0, image.width(), image.height());

	auto pm = QPixmap::fromImage(image);
	lastPixmapItem = scene->addPixmap(pm);
}

QRect DisplayArea::getViewportRect() const {
	return view->viewport()->rect();
}

/*
void DisplayArea::resizeEvent(QResizeEvent *event) {
	QWidget::resizeEvent(event);

	if (displayMode == Constants::DisplayMode::FITTED) {
		view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
	}
}
*/
