#include "wireframemainview.h"

#include "src/common/constants.h"

WireframeMainView::WireframeMainView(QWidget *parent) : QGraphicsView(parent) {
	m_wfmScene = new WireframeMainScene(this);
	setScene(m_wfmScene);
	setBackgroundBrush(Constants::MAIN_BACKGROUND_COLOR);
	m_rotating = false;
}

void WireframeMainView::showWireframe(const Wireframe &wireframe, bool reset) {
	m_wfmScene->showWireframe(wireframe, reset);
}

void WireframeMainView::setAngleX(qreal xAngle) {
	m_wfmScene->setAngleX(xAngle);
}

void WireframeMainView::setAngleY(qreal yAngle) {
	m_wfmScene->setAngleY(yAngle);
}

void WireframeMainView::setAngleZ(qreal zAngle) {
	m_wfmScene->setAngleZ(zAngle);
}

void WireframeMainView::resetAngles() {
	m_wfmScene->resetAngles();
}

void WireframeMainView::resizeEvent(QResizeEvent *event) {
	QGraphicsView::resizeEvent(event);

	setSceneRect(viewport()->rect());
	m_wfmScene->setSceneRect(viewport()->rect());

	m_wfmScene->repaint();
}

void WireframeMainView::wheelEvent(QWheelEvent *event) {
	if (event->modifiers() & Qt::ControlModifier) {
		if (event->angleDelta().y() > 0) {
			if (m_wfmScene->getZn() + Constants::ZN_STEP <= Constants::MAX_ZN) {
				m_wfmScene->setZn(m_wfmScene->getZn() + Constants::ZN_STEP);
			}
		}
		else {
			if (m_wfmScene->getZn() - Constants::ZN_STEP >= Constants::MIN_ZN) {
				m_wfmScene->setZn(m_wfmScene->getZn() - Constants::ZN_STEP);
			}
		}
	}
	else {
		QGraphicsView::wheelEvent(event);
	}
}

void WireframeMainView::mouseMoveEvent(QMouseEvent *event) {

	if (!m_rotating) {
		m_lastPos = event->pos();
		m_rotating = true;
		return;
	}

	int dx = event->position().x() - m_lastPos.x();
	int dy = event->position().y() - m_lastPos.y();

	if (event->buttons() & Qt::LeftButton) {
		//setAngleX(m_wfmScene->getAngleX() + 2 * M_PI * dy / 360.0 / 4);
		setAngleY(m_wfmScene->getAngleY() + 2 * M_PI * dx / 360.0 / 4);
	}
	else if (event->buttons() & Qt::RightButton) {
		setAngleX(m_wfmScene->getAngleX() + 2 * M_PI * dy / 360.0 / 4);
		//setAngleZ(m_wfmScene->getAngleZ() + 2 * M_PI * dx / 360.0 / 4);
	}
	m_lastPos = event->pos();
}

void WireframeMainView::mouseReleaseEvent(QMouseEvent *event) {
	Q_UNUSED(event);
	m_rotating = false;
}
