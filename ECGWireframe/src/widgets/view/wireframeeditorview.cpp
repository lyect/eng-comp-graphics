#include "wireframeeditorview.h"

#include "src/common/constants.h"

// ##############
// #   PUBLIC   #
// ##############

WireframeEditorView::WireframeEditorView(QWidget *parent) : QGraphicsView(parent) {
	m_wasOpened = false;
	m_wfeScene = new WireframeEditorScene(this);
	setScene(m_wfeScene);
	setBackgroundBrush(Constants::EDITOR_BACKGROUND_COLOR);
	setDragMode(QGraphicsView::ScrollHandDrag);

	for (qreal zf = Constants::MIN_EDITOR_ZOOM_FACTOR; zf < Constants::MAX_EDITOR_ZOOM_FACTOR; zf += Constants::EDITOR_ZOOM_FACTOR_STEP) {
		m_zoomFactors.append(zf);
	}
	m_currentZoomFactorIndex = 0;

	setSplinePartition(Constants::DEFAULT_N_RENDER_PARAMETER_VALUE);

	QObject::connect(m_wfeScene, &WireframeEditorScene::infoUpdated, this, &WireframeEditorView::sceneInfoUpdated);
}

void WireframeEditorView::clear() {
	m_wfeScene->clear();
}

void WireframeEditorView::setControlPoints(const QVector<QPointF> &points) {
	for (auto p : points) {
		m_wfeScene->addControlPoint(p, transform());
	}
}

QVector<QPointF> WireframeEditorView::getControlPoints() const {
	return m_wfeScene->getControlPoints();
}

// ###############
// #   PRIVATE   #
// ###############

void WireframeEditorView::resetZoom() {
	// It might have another implementation
	//	if initial zoom factor does not equal to MIN_ZOOM_FACTOR
	resetTransform();
	m_currentZoomFactorIndex = 0;
	m_wfeScene->repaintAxis(m_zoomFactors[m_currentZoomFactorIndex]);
}

void WireframeEditorView::zoomIn() {
	if (m_currentZoomFactorIndex == m_zoomFactors.count() - 1) {
		return;
	}
	++m_currentZoomFactorIndex;

	qreal previousZoomFactor = m_zoomFactors[m_currentZoomFactorIndex - 1];
	qreal currentZoomFactor = m_zoomFactors[m_currentZoomFactorIndex];
	qreal zoomFactor = currentZoomFactor / previousZoomFactor;
	scale(zoomFactor, zoomFactor);
	m_wfeScene->repaintAxis(currentZoomFactor / m_zoomFactors[0]);
}

void WireframeEditorView::zoomOut() {
	if (m_currentZoomFactorIndex == 0) {
		return;
	}
	--m_currentZoomFactorIndex;

	qreal previousZoomFactor = m_zoomFactors[m_currentZoomFactorIndex + 1];
	qreal currentZoomFactor = m_zoomFactors[m_currentZoomFactorIndex];
	qreal zoomFactor = currentZoomFactor / previousZoomFactor;
	scale(zoomFactor, zoomFactor);
	m_wfeScene->repaintAxis(currentZoomFactor / m_zoomFactors[0]);
}

void WireframeEditorView::showEvent(QShowEvent *event) {
	QGraphicsView::showEvent(event);

	resetZoom();

	if (m_wasOpened) {
		return;
	}

	m_wasOpened = true;

	qreal width = viewport()->rect().width();
	qreal height = viewport()->rect().height();

	emit sizeUpdated(width, height);

	setSceneRect(0, 0, width, height);
	m_wfeScene->setSceneRect(0, 0, width, height);

	m_wfeScene->repaintAxis(m_zoomFactors[m_currentZoomFactorIndex]);
}

void WireframeEditorView::wheelEvent(QWheelEvent *event) {
	if (event->modifiers() & Qt::ControlModifier) {

		const ViewportAnchor prevTransformationAnchor = transformationAnchor();
		setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

		if (event->angleDelta().y() > 0) {
			zoomIn();
		}
		else {
			zoomOut();
		}

		setTransformationAnchor(prevTransformationAnchor);
	}
	else {
		QGraphicsView::wheelEvent(event);
	}
}

void WireframeEditorView::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::RightButton) {
		auto scenePoint = mapToScene(event->pos());
		if (event->modifiers() == Qt::NoModifier) {
			m_wfeScene->addControlPoint(scenePoint, transform());
		}
		else if (event->modifiers() == Qt::ControlModifier) {
			m_wfeScene->removeControlPoint(scenePoint, transform());
		}
	}

	QGraphicsView::mousePressEvent(event);
}

// ####################
// #   PUBLIC SLOTS   #
// ####################

void WireframeEditorView::selectPrevControlPoint() {
	m_wfeScene->selectPrevControlPoint();
}

void WireframeEditorView::selectNextControlPoint() {
	m_wfeScene->selectNextControlPoint();
}

void WireframeEditorView::addControlPointToCenter() {
	auto r = sceneRect();
	qreal x = r.width() / 2;
	qreal y = r.height() / 2;
	m_wfeScene->addControlPoint(QPointF(x, y), transform());
}

void WireframeEditorView::deleteSelectedControlPoint() {
	m_wfeScene->deleteSelectedControlPoint();
}

void WireframeEditorView::updateSelectedControlPointCoordinates(qreal x, qreal y) {
	m_wfeScene->updateSelectedControlPointCoordinates(x, y);
}

void WireframeEditorView::sceneInfoUpdated(qsizetype controlPointsCount, qsizetype controlPointNumber, const QPointF &point) {
	emit infoUpdated(controlPointsCount, controlPointNumber, point);
}

void WireframeEditorView::setSplinePartition(qsizetype splinePartition) {
	m_wfeScene->setSplinePartition(splinePartition);
}
