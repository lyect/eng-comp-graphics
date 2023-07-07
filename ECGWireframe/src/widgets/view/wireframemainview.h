#ifndef WIREFRAMEMAINVIEW_H
#define WIREFRAMEMAINVIEW_H

#include <QGraphicsView>
#include <QResizeEvent>

#include "src/common/wireframe.h"
#include "src/widgets/view/wireframemainscene.h"

class WireframeMainView : public QGraphicsView {

	Q_OBJECT

public:

	explicit WireframeMainView(QWidget *parent = nullptr);

	void showWireframe(const Wireframe &wireframe, bool reset);
	void setAngleX(qreal xAngle);
	void setAngleY(qreal yAngle);
	void setAngleZ(qreal zAngle);
	void resetAngles();

private:

	WireframeMainScene *m_wfmScene;

	bool m_rotating;
	QPointF m_lastPos;

	void resizeEvent(QResizeEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // WIREFRAMEMAINVIEW_H
