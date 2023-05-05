#include "wireframeeditorscene.h"

#include "src/common/constants.h"

// ##############
// #   PUBLIC   #
// ##############

WireframeEditorScene::WireframeEditorScene(QObject *parent) : QGraphicsScene(parent) {
	m_xAxis = nullptr;
	m_yAxis = nullptr;
	m_lastSelection = nullptr;

	QObject::connect(this, &WireframeEditorScene::selectionChanged, this, &WireframeEditorScene::updateSelection);
}

WireframeEditorScene::~WireframeEditorScene() {
	blockSignals(true);
	QGraphicsScene::clear();
}

void WireframeEditorScene::repaintAxis(double zoomFactor) {

	qreal tickWidth = Constants::EDITOR_AXIS_TICK_WIDTH;
	qreal tickStep = Constants::EDITOR_AXIS_TICK_STEP;
	qreal arrowWidth = Constants::EDITOR_AXIS_ARROW_WIDTH;
	qreal arrowLength = Constants::EDITOR_AXIS_ARROW_LENGTH;

	if (zoomFactor > Constants::MIN_EDITOR_ZOOM_FACTOR) {
		tickWidth /= zoomFactor;
		arrowWidth /= zoomFactor;
		arrowLength /= zoomFactor;
	}

	qreal height = sceneRect().height();
	qreal width = sceneRect().width();

	if (m_xAxis != nullptr) {
		removeItem(m_xAxis);
		delete m_xAxis;
	}
	if (m_yAxis != nullptr) {
		removeItem(m_yAxis);
		delete m_yAxis;
	}

	m_xAxis = new TickArrowItem(
				QPointF(        0, height / 2),
				QPointF(width - 1, height / 2),
				tickWidth,
				tickStep,
				Constants::EDITOR_AXIS_TICK_START,
				false,
				arrowWidth,
				arrowLength,
				QPen(Constants::EDITOR_AXIS_COLOR),
				QBrush(Constants::EDITOR_AXIS_COLOR)
	);
	m_yAxis = new TickArrowItem(
				QPointF(width / 2, height - 1),
				QPointF(width / 2,          0),
				tickWidth,
				tickStep,
				Constants::EDITOR_AXIS_TICK_START,
				false,
				arrowWidth,
				arrowLength,
				QPen(Constants::EDITOR_AXIS_COLOR),
				QBrush(Constants::EDITOR_AXIS_COLOR)
	);

	addItem(m_xAxis);
	addItem(m_yAxis);
}

bool WireframeEditorScene::isControlPoint(const QPointF &scenePoint, const QTransform &transform) {
	QGraphicsItem *item = itemAt(scenePoint, transform);

	auto it = std::find_if(m_wrappers.begin(), m_wrappers.end(), [&](const PointWrapper &c) -> bool {
		return c.getPoint() == item;
	});

	if (item == nullptr || it == m_wrappers.end()) {
		return false;
	}

	return true;
}

void WireframeEditorScene::addControlPoint(const QPointF &scenePoint, const QTransform &transform) {
	QGraphicsItem *item = itemAt(scenePoint, transform);

	auto it = std::find_if(m_wrappers.begin(), m_wrappers.end(), [&](const PointWrapper &c) -> bool {
		return c.getPoint() == item;
	});

	if (item != nullptr && it != m_wrappers.end()) {
		return;
	}

	ControlPointItem *controlPoint = new ControlPointItem(
				Constants::EDITOR_CONTROL_POINT_RADIUS,
				Constants::EDITOR_CONTROL_OBJECT_COLOR, Constants::EDITOR_CONTROL_OBJECT_COLOR
	);
	controlPoint->setFlag(QGraphicsItem::ItemIsMovable, true);
	controlPoint->setFlag(QGraphicsItem::ItemIsSelectable, true);
	controlPoint->setPos(
				scenePoint.x() - Constants::EDITOR_CONTROL_POINT_RADIUS,
				scenePoint.y() - Constants::EDITOR_CONTROL_POINT_RADIUS
	);
	QObject::connect(controlPoint, &ControlPointItem::moved, this, &WireframeEditorScene::controlPointMoved);

	// add control point to the scene
	addItem(controlPoint);
	m_wrappers.push_back(PointWrapper(controlPoint));

	// connect control point to its neighbors
	addControlLines(m_wrappers.count() - 1);

	// draw new (or update) spline
	updateSplines(m_wrappers.count() - 3, m_wrappers.count() - 3);

	if (m_lastSelection != nullptr) {
		m_lastSelection->setSelected(false);
	}
	m_lastSelection = controlPoint;
	m_lastSelection->setSelected(true);
	emit infoUpdated(m_wrappers.count(), m_wrappers.count(), controlPoint->getCenter());
}

void WireframeEditorScene::removeControlPoint(const QPointF &scenePoint, const QTransform &transform) {
	QGraphicsItem *item = itemAt(scenePoint, transform);

	auto it = std::find_if(m_wrappers.begin(), m_wrappers.end(), [&](const PointWrapper &c) -> bool {
		return c.getPoint() == item;
	});

	if (item == nullptr || it == m_wrappers.end()) {
		return;
	}

	int controlPointIndex = std::distance(m_wrappers.begin(), it);

	// disconnect control point from another control points
	removeControlLines(controlPointIndex);

	// remove spline if exists
	if (m_wrappers[controlPointIndex].getSpline() != nullptr) {
		auto spline = m_wrappers[controlPointIndex].getSpline();

		// free allocated memory and delete spline from the scene
		delete spline;

		// set member value to nullptr (just for code cleanness)
		m_wrappers[controlPointIndex].setSpline(nullptr);
	}

	if (m_wrappers.count() > 1) {
		if (m_wrappers[controlPointIndex].getPoint() == m_lastSelection) {
			emit infoUpdated(m_wrappers.count() - 1, 0, QPointF());
		}
		else {
			if (m_lastSelection == nullptr) {
				emit infoUpdated(m_wrappers.count() - 1, 0, QPointF());
			}
			else {
				auto it = std::find_if(m_wrappers.begin(), m_wrappers.end(), [&](const PointWrapper &c) -> bool {
					return c.getPoint() == m_lastSelection;
				});
				int selectedPointIndex = std::distance(m_wrappers.begin(), it);

				if (selectedPointIndex > controlPointIndex) {
					emit infoUpdated(m_wrappers.count() - 1, selectedPointIndex, it->getPoint()->getCenter());
				}
				else {
					emit infoUpdated(m_wrappers.count() - 1, selectedPointIndex + 1, it->getPoint()->getCenter());
				}
			}

		}
	}
	else {
		emit infoUpdated(0, 0, QPointF());
	}

	// remove control point from scene
	m_wrappers[controlPointIndex].getPoint()->deleteLater();
	m_wrappers.removeAt(controlPointIndex);

	// connect point's neighbors
	addControlLines(controlPointIndex); // or controlPointIndex - 1, it does not matter

	// update splines which had been affected by point deletion
	updateSplines(controlPointIndex - 2, controlPointIndex);
}

void WireframeEditorScene::setSplinePartition(qsizetype splinePartition) {
	m_splinePartition = splinePartition;
	updateSplines(1, m_wrappers.count() - 2);
}

void WireframeEditorScene::clear() {
	m_wrappers.clear();
	QGraphicsScene::clear();
	m_xAxis = nullptr;
	m_yAxis = nullptr;
}

QVector<QPointF> WireframeEditorScene::getControlPoints() const {
	QVector<QPointF> res;
	for (auto p : m_wrappers) {
		res.push_back(p.getPoint()->getCenter());
	}
	return res;
}

void WireframeEditorScene::selectNextControlPoint() {
	if (m_lastSelection == nullptr) {
		emit infoUpdated(m_wrappers.count(), 0, QPointF());
	}
	else {
		auto it = std::find_if(m_wrappers.begin(), m_wrappers.end(), [&](const PointWrapper &c) -> bool {
			return c.getPoint() == m_lastSelection;
		});
		int controlPointIndex = std::distance(m_wrappers.begin(), it);

		if (controlPointIndex < m_wrappers.count() - 1) {
			m_lastSelection->setSelected(false);
			m_lastSelection = m_wrappers[controlPointIndex + 1].getPoint();
			m_lastSelection->setSelected(true);

			emit infoUpdated(m_wrappers.count(), controlPointIndex + 2, m_wrappers[controlPointIndex + 1].getPoint()->getCenter());
		}
		else {
			emit infoUpdated(m_wrappers.count(), controlPointIndex + 1, m_wrappers[controlPointIndex].getPoint()->getCenter());
		}
	}
}

void WireframeEditorScene::selectPrevControlPoint() {
	if (m_lastSelection == nullptr) {
		emit infoUpdated(m_wrappers.count(), 0, QPointF());
	}
	else {
		auto it = std::find_if(m_wrappers.begin(), m_wrappers.end(), [&](const PointWrapper &c) -> bool {
			return c.getPoint() == m_lastSelection;
		});
		int controlPointIndex = std::distance(m_wrappers.begin(), it);

		if (controlPointIndex > 0) {
			m_lastSelection->setSelected(false);
			m_lastSelection = m_wrappers[controlPointIndex - 1].getPoint();
			m_lastSelection->setSelected(true);

			emit infoUpdated(m_wrappers.count(), controlPointIndex, m_wrappers[controlPointIndex - 1].getPoint()->getCenter());
		}
		else {
			emit infoUpdated(m_wrappers.count(), controlPointIndex + 1, m_wrappers[controlPointIndex].getPoint()->getCenter());
		}
	}
}

void WireframeEditorScene::deleteSelectedControlPoint() {
	if (m_lastSelection == nullptr) {
		emit infoUpdated(m_wrappers.count(), 0, QPointF());
		return;
	}

	auto it = std::find_if(m_wrappers.begin(), m_wrappers.end(), [&](const PointWrapper &c) -> bool {
		return c.getPoint() == m_lastSelection;
	});
	int controlPointIndex = std::distance(m_wrappers.begin(), it);

	// disconnect control point from another control points
	removeControlLines(controlPointIndex);

	// remove spline if exists
	if (m_wrappers[controlPointIndex].getSpline() != nullptr) {
		auto spline = m_wrappers[controlPointIndex].getSpline();

		// free allocated memory and delete spline from the scene
		delete spline;

		// set member value to nullptr (just for code cleanness)
		m_wrappers[controlPointIndex].setSpline(nullptr);
	}

	if (m_wrappers.count() != 0) {
		if (m_wrappers[controlPointIndex].getPoint() == m_lastSelection) {
			emit infoUpdated(m_wrappers.count(), 0, QPointF());
		}
	}
	else {
		emit infoUpdated(0, 0, QPointF());
	}


	// remove control point from scene
	m_wrappers[controlPointIndex].getPoint()->deleteLater();
	m_wrappers.removeAt(controlPointIndex);

	// connect point's neighbors
	addControlLines(controlPointIndex); // or controlPointIndex - 1, it does not matter

	// update splines which had been affected by point deletion
	updateSplines(controlPointIndex - 2, controlPointIndex);
}

void WireframeEditorScene::updateSelectedControlPointCoordinates(qreal x, qreal y) {
	if (m_lastSelection == nullptr) {
		emit infoUpdated(m_wrappers.count(), 0, QPointF());
		return;
	}

	auto it = std::find_if(m_wrappers.begin(), m_wrappers.end(), [&](const PointWrapper &c) -> bool {
		return c.getPoint() == m_lastSelection;
	});
	int controlPointIndex = std::distance(m_wrappers.begin(), it);

	it->getPoint()->setPos(QPointF(x, y));

	if (it->getPrevLine() != nullptr) {
		QLineF precedingLine = it->getPrevLine()->line();
		it->getPrevLine()->setLine(QLineF(precedingLine.p1(), it->getPoint()->getCenter()));
	}

	if (it->getNextLine() != nullptr) {
		QLineF precedingLine = it->getNextLine()->line();
		it->getNextLine()->setLine(QLineF(it->getPoint()->getCenter(), precedingLine.p2()));
	}

	updateSplines(controlPointIndex - 2, controlPointIndex + 1);

	emit infoUpdated(m_wrappers.count(), controlPointIndex + 1, it->getPoint()->getCenter());
}


// ###############
// #   PRIVATE   #
// ###############

void WireframeEditorScene::addControlLines(int controlPointIndex) {
	if (controlPointIndex < 0 || controlPointIndex > m_wrappers.count() - 1) {
		return;
	}

	auto &cc = m_wrappers[controlPointIndex];    // current connection
	const QPointF &cp = cc.getPoint()->getCenter(); // current point

	if (0 <= controlPointIndex && controlPointIndex < m_wrappers.count() - 1 && cc.getNextLine() == nullptr) {
		auto &nc = m_wrappers[controlPointIndex + 1]; // next connection
		const QPointF &np = nc.getPoint()->getCenter();  // next point
		QGraphicsLineItem *lineItem = addLine(QLineF(cp, np), QPen(Constants::EDITOR_CONTROL_OBJECT_COLOR));
		cc.setNextLine(lineItem);
		nc.setPrevLine(lineItem);
	}

	if (0 < controlPointIndex && controlPointIndex <= m_wrappers.count() - 1 && cc.getPrevLine() == nullptr) {
		auto &pc = m_wrappers[controlPointIndex - 1]; // previous connection
		const QPointF &pp = pc.getPoint()->getCenter();  // previous point
		QGraphicsLineItem *lineItem = addLine(QLineF(pp, cp), QPen(Constants::EDITOR_CONTROL_OBJECT_COLOR));
		lineItem->setZValue(-2);
		pc.setNextLine(lineItem);
		cc.setPrevLine(lineItem);
	}
}

void WireframeEditorScene::removeControlLines(int controlPointIndex) {
	auto prevLine = m_wrappers[controlPointIndex].getPrevLine();
	auto nextLine = m_wrappers[controlPointIndex].getNextLine();

	if (prevLine != nullptr) {
		delete prevLine;
		m_wrappers[controlPointIndex].setPrevLine(nullptr);
	}

	if (nextLine != nullptr) {
		delete nextLine;
		m_wrappers[controlPointIndex].setNextLine(nullptr);
	}

	if (0 <= controlPointIndex && controlPointIndex < m_wrappers.count() - 1) {
		m_wrappers[controlPointIndex + 1].setPrevLine(nullptr);
	}
	if (0 < controlPointIndex && controlPointIndex <= m_wrappers.count() - 1) {
		m_wrappers[controlPointIndex - 1].setNextLine(nullptr);
	}
}

void WireframeEditorScene::updateSplines(int firstControlPointIndex, int secondControlPointIndex) {
	for (int controlPointIndex = firstControlPointIndex; controlPointIndex <= secondControlPointIndex; ++controlPointIndex) {

		if (controlPointIndex < 0 || controlPointIndex >= m_wrappers.count()) {
			continue;
		}

		auto spline = m_wrappers[controlPointIndex].getSpline();

		if (0 < controlPointIndex && controlPointIndex < m_wrappers.count() - 2) {
			const QPointF &p1 = m_wrappers[controlPointIndex - 1].getPoint()->getCenter();
			const QPointF &p2 = m_wrappers[controlPointIndex].getPoint()->getCenter();
			const QPointF &p3 = m_wrappers[controlPointIndex + 1].getPoint()->getCenter();
			const QPointF &p4 = m_wrappers[controlPointIndex + 2].getPoint()->getCenter();

			if (spline == nullptr) {
				spline = new SplineItem(p1, p2, p3, p4, m_splinePartition, Constants::EDITOR_SPLINE_COLOR);
				addItem(spline);
				m_wrappers[controlPointIndex].setSpline(spline);
			}
			else {
				spline->updateSpline(p1, p2, p3, p4, m_splinePartition);
			}
		}
		else {
			if (spline != nullptr) {
				delete spline;
				m_wrappers[controlPointIndex].setSpline(nullptr);
			}
		}
	}
}

// #####################
// #   PRIVATE SLOTS   #
// #####################

void WireframeEditorScene::controlPointMoved() {
	auto sender = QObject::sender();

	auto it = std::find_if(m_wrappers.begin(), m_wrappers.end(), [&](const PointWrapper &c) -> bool {
		return c.getPoint() == sender;
	});

	if (it == m_wrappers.end()) {
		return;
	}

	if (it->getPrevLine() != nullptr) {
		QLineF precedingLine = it->getPrevLine()->line();
		it->getPrevLine()->setLine(QLineF(precedingLine.p1(), it->getPoint()->getCenter()));
	}

	if (it->getNextLine() != nullptr) {
		QLineF precedingLine = it->getNextLine()->line();
		it->getNextLine()->setLine(QLineF(it->getPoint()->getCenter(), precedingLine.p2()));
	}

	int controlPointIndex = std::distance(m_wrappers.begin(), it);
	updateSplines(controlPointIndex - 2, controlPointIndex + 1);

	emit infoUpdated(m_wrappers.count(), controlPointIndex + 1, it->getPoint()->getCenter());
}

void WireframeEditorScene::updateSelection() {
	auto items = selectedItems();

	if (items.empty()) {
		m_lastSelection = nullptr;
	}
	else if (items.count() == 1) {
		if (items[0] != m_lastSelection) {
			if (m_lastSelection != nullptr) {
				m_lastSelection->setSelected(false);
			}
			m_lastSelection = items[0];
		}
	}
	else {
		if (items[0] == m_lastSelection) {
			m_lastSelection = items[1];
			items[0]->setSelected(false);
		}
		else {
			m_lastSelection = items[0];
			items[1]->setSelected(false);
		}
	}

	if (m_lastSelection != nullptr) {
		auto it = std::find_if(m_wrappers.begin(), m_wrappers.end(), [&](const PointWrapper &c) -> bool {
			return c.getPoint() == m_lastSelection;
		});
		int controlPointIndex = std::distance(m_wrappers.begin(), it);

		emit infoUpdated(m_wrappers.count(), controlPointIndex + 1, it->getPoint()->getCenter());
	}
	else {
		emit infoUpdated(m_wrappers.count(), 0, QPointF());
	}

}






























