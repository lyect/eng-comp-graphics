#include "borderedgraphicsview.h"

#include "constants.h"

BorderedGraphicsView::BorderedGraphicsView(QWidget *parent)
	: QGraphicsView(parent) {}

void BorderedGraphicsView::paintEvent(QPaintEvent *event) {
	QGraphicsView::paintEvent(event);
	QPainter painter(this->viewport());

	int vpw = this->viewport()->width();
	int vph = this->viewport()->height();

	painter.setPen(QPen(Qt::black, Constants::VIEW_BORDER_WIDTH, Qt::DashLine));
	painter.drawLine(
				Constants::VIEW_BORDER_WIDTH / 2,
				Constants::VIEW_BORDER_WIDTH / 2,
				vpw - Constants::VIEW_BORDER_WIDTH / 2,
				Constants::VIEW_BORDER_WIDTH / 2
	);
	painter.drawLine(
				vpw - Constants::VIEW_BORDER_WIDTH / 2,
				Constants::VIEW_BORDER_WIDTH / 2,
				vpw - Constants::VIEW_BORDER_WIDTH / 2,
				vph - Constants::VIEW_BORDER_WIDTH / 2
	);
	painter.drawLine(
				vpw - Constants::VIEW_BORDER_WIDTH / 2,
				vph - Constants::VIEW_BORDER_WIDTH / 2,
				Constants::VIEW_BORDER_WIDTH / 2,
				vph - Constants::VIEW_BORDER_WIDTH / 2
	);
	painter.drawLine(
				Constants::VIEW_BORDER_WIDTH / 2,
				vph - Constants::VIEW_BORDER_WIDTH / 2,
				Constants::VIEW_BORDER_WIDTH / 2,
				Constants::VIEW_BORDER_WIDTH / 2
	);


}
