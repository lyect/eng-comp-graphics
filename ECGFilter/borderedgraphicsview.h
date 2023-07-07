#ifndef BORDEREDGRAPHICSVIEW_H
#define BORDEREDGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QPaintEvent>

class BorderedGraphicsView : public QGraphicsView {

	Q_OBJECT

public:

	BorderedGraphicsView(QWidget *parent = nullptr);

private:

	void paintEvent(QPaintEvent *event) override;
};

#endif // BORDEREDGRAPHICSVIEW_H
