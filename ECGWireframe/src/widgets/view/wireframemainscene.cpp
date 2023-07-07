#include "wireframemainscene.h"
#include "src/common/constants.h"

// ##############
// #   PUBLIC   #
// ##############

WireframeMainScene::WireframeMainScene(QObject *parent) : QGraphicsScene(parent) {
	cubicCurve = [](qreal a, qreal b, qreal c, qreal d, qreal x) -> qreal {
		return a * x * x * x + b * x * x + c * x + d;
	};

	m_xAngle = 2.0 * M_PI * Constants::DEFAULT_X_ANGLE / 360.0;
	m_yAngle = 2.0 * M_PI * Constants::DEFAULT_Y_ANGLE / 360.0;
	m_zAngle = 2.0 * M_PI * Constants::DEFAULT_Z_ANGLE / 360.0;

	m_scale = 1;

	m_itemsFilled = false;
	m_figuresAdded = false;

	m_cameraPoint = Constants::DEFAULT_CAMERA_POINT;

	m_zn = Constants::MIN_ZN;
}

void WireframeMainScene::showWireframe(const Wireframe &wireframe, bool reset) {

	clear();

	m_itemsFilled = false;
	m_figuresAdded = false;

	const auto &controlPoints2D = wireframe.getControlPoints();

	if (controlPoints2D.count() < 4) {
		showBlank();
		return;
	}

	const auto N = wireframe.getRenderParameters().getN();
	const auto M1 = wireframe.getRenderParameters().getM1();
	const auto M2 = wireframe.getRenderParameters().getM2();
	const auto K = N * (controlPoints2D.count() - 3) + 1;

	const auto [forming, circlePoints2D] = makeForming(controlPoints2D, N);

	Q_ASSERT(forming.count() == K);

	qreal width = sceneRect().width();
	qreal height = sceneRect().height();

	m_scale = 1;

	// Allocate memory for formings
	m_formings.resize(M1);
	for (auto &f : m_formings) {
		f = new FormingItem(&m_xAngle, &m_yAngle, &m_zAngle, &m_cameraPoint, &m_zn, m_matrix);
		f->setPos(width / 2, height / 2);
	}

	// Fill formings
	for (qsizetype i = 0; i < K; ++i) {
		for (qsizetype j = 0; j < M1; ++j) {
			float x = static_cast<float>(-(forming[i].y() - wireframe.getHeight() / 2) * qCos(2 * M_PI * j / M1));
			float y = static_cast<float>(-(forming[i].y() - wireframe.getHeight() / 2) * qSin(2 * M_PI * j / M1));
			float z = static_cast<float>((forming[i].x() - wireframe.getWidth() / 2));
			float w = static_cast<float>(1);
			m_formings[j]->addPoint(QVector4D(x, y, z, w));

			m_scale = qMax(m_scale, qMax(qMax(qAbs(x), qAbs(y)), qMax(qAbs(z), qAbs(w))));
		}
	}

	// Allocate memory for circles
	m_circles.resize(circlePoints2D.count());
	for (auto &c : m_circles) {
		c = new CircleItem(&m_xAngle, &m_yAngle, &m_zAngle, &m_cameraPoint, &m_zn, m_matrix);
		c->setPos(width / 2, height / 2);
	}

	// Fill circles
	for (qsizetype i = 0; i < circlePoints2D.count(); ++i) {
		for (qsizetype j = 0; j < M2 * M1; ++j) {
			float x = static_cast<float>(-(circlePoints2D[i].y() - wireframe.getHeight() / 2) * qCos(2 * M_PI * j / (M1 * M2)));
			float y = static_cast<float>(-(circlePoints2D[i].y() - wireframe.getHeight() / 2) * qSin(2 * M_PI * j / (M1 * M2)));
			float z = static_cast<float>((circlePoints2D[i].x() - wireframe.getWidth() / 2));
			float w = static_cast<float>(1);
			m_circles[i]->addPoint(QVector4D(x, y, z, w));

			m_scale = qMax(m_scale, qMax(qMax(qAbs(x), qAbs(y)), qMax(qAbs(z), qAbs(w))));
		}
	}

	for (auto f : m_formings) {
		f->setScale(m_scale);
	}

	for (auto c : m_circles) {
		c->setScale(m_scale);
	}

	m_itemsFilled = true;
	if (reset) {
		m_zn = Constants::MIN_ZN;
		resetAngles();
	}
	else {
		repaint();
	}
}

void WireframeMainScene::setAngleX(qreal xAngle) {
	m_xAngle = xAngle;
	repaint();
}

void WireframeMainScene::setAngleY(qreal yAngle) {
	m_yAngle = yAngle;
	repaint();
}

void WireframeMainScene::setAngleZ(qreal zAngle) {
	m_zAngle = zAngle;
	repaint();
}

qreal WireframeMainScene::getAngleX() const {
	return m_xAngle;
}

qreal WireframeMainScene::getAngleY() const {
	return m_yAngle;
}

qreal WireframeMainScene::getAngleZ() const {
	return m_zAngle;
}

void WireframeMainScene::resetAngles() {
	m_xAngle = 2.0 * M_PI * Constants::DEFAULT_X_ANGLE / 360.0;
	m_yAngle = 2.0 * M_PI * Constants::DEFAULT_Y_ANGLE / 360.0;
	m_zAngle = 2.0 * M_PI * Constants::DEFAULT_Z_ANGLE / 360.0;
	repaint();
}

void WireframeMainScene::setCameraPoint(const QVector3D &cameraPoint) {
	m_cameraPoint = cameraPoint;
	repaint();
}

qreal WireframeMainScene::getZn() const {
	return m_zn;
}

void WireframeMainScene::setZn(qreal zn) {
	m_zn = zn;
	repaint();
}

void WireframeMainScene::repaint() {
	if (!m_itemsFilled) {
		showBlank();
	}
	else {
		qreal width = sceneRect().width();
		qreal height = sceneRect().height();

		calculateMatrix(width, height);

		for (auto &f : m_formings) {
			f->setPos(width / 2, height / 2);
		}
		for (auto &c : m_circles) {
			c->setPos(width / 2, height / 2);
		}

		showProjection();
	}
}

// ###############
// #   PRIVATE   #
// ###############

QPair<QVector<QPointF>, QVector<QPointF>> WireframeMainScene::makeForming(const QVector<QPointF> &controlPoints, qsizetype splinePartition) {
	QVector<QPointF> forming;
	QVector<QPointF> circlePoints2D;

	for (qsizetype i = 1; i < controlPoints.count() - 2; ++i) {
		QPointF p1 = controlPoints[i - 1];
		QPointF p2 = controlPoints[i];
		QPointF p3 = controlPoints[i + 1];
		QPointF p4 = controlPoints[i + 2];

		qreal ax = (-p1.x() + 3 * p2.x() - 3 * p3.x() + p4.x()) / 6;
		qreal bx = (3 * p1.x() - 6 * p2.x() + 3 * p3.x()) / 6;
		qreal cx = (-3 * p1.x() + 3 * p3.x()) / 6;
		qreal dx = (p1.x() + 4 * p2.x() + p3.x()) / 6;

		qreal ay = (-p1.y() + 3 * p2.y() - 3 * p3.y() + p4.y()) / 6;
		qreal by = (3 * p1.y() - 6 * p2.y() + 3 * p3.y()) / 6;
		qreal cy = (-3 * p1.y() + 3 * p3.y()) / 6;
		qreal dy = (p1.y() + 4 * p2.y() + p3.y()) / 6;

		QPointF firstPoint(cubicCurve(ax, bx, cx, dx, 0), cubicCurve(ay, by, cy, dy, 0));
		forming.push_back(firstPoint);
		circlePoints2D.push_back(firstPoint);

		for (qsizetype j = 1; j < splinePartition; ++j) {
			qreal t = 1.0 / splinePartition * j;
			QPointF currPoint(cubicCurve(ax, bx, cx, dx, t), cubicCurve(ay, by, cy, dy, t));
			forming.push_back(QPointF(currPoint.x(), currPoint.y()));
		}

		if (i == controlPoints.count() - 3) {
			QPointF currPoint(cubicCurve(ax, bx, cx, dx, 1), cubicCurve(ay, by, cy, dy, 1));
			forming.push_back(QPointF(currPoint.x(), currPoint.y()));
			circlePoints2D.push_back(currPoint);
		}
	}

	return {forming, circlePoints2D};
}

void WireframeMainScene::calculateMatrix(qreal w, qreal h) {
	qreal rotationMatrix[16];
	qreal cameraRotationMatrix[16];
	qreal cameraTranslationMatrix[16];
	qreal viewMatrix[16];
	qreal translationMatrix[16];
	qreal projectionMatrix[16];

	qreal tmpMatrix1[16];
	qreal tmpMatrix2[16];

	rotationMatrix[ 0] = qCos(m_yAngle) * qCos(m_zAngle);
	rotationMatrix[ 1] = -qSin(m_zAngle) * qCos(m_yAngle);
	rotationMatrix[ 2] = qSin(m_yAngle);
	rotationMatrix[ 3] = 0.0;
	rotationMatrix[ 4] = qSin(m_xAngle) * qSin(m_yAngle) * qCos(m_zAngle) + qSin(m_zAngle) * qCos(m_xAngle);
	rotationMatrix[ 5] = -qSin(m_xAngle) * qSin(m_yAngle) * qSin(m_zAngle) + qCos(m_xAngle) * qCos(m_zAngle);
	rotationMatrix[ 6] = -qSin(m_xAngle) * qCos(m_yAngle);
	rotationMatrix[ 7] = 0.0;
	rotationMatrix[ 8] = -qCos(m_xAngle) * qSin(m_yAngle) * qCos(m_zAngle) + qSin(m_xAngle) * qSin(m_zAngle);
	rotationMatrix[ 9] = qCos(m_xAngle) * qSin(m_yAngle) * qSin(m_zAngle) + qSin(m_xAngle) * qCos(m_zAngle);
	rotationMatrix[10] = qCos(m_xAngle) * qCos(m_yAngle);
	rotationMatrix[11] = 0.0;
	rotationMatrix[12] = 0.0;
	rotationMatrix[13] = 0.0;
	rotationMatrix[14] = 0.0;
	rotationMatrix[15] = 1.0;

	qreal xCameraAngle = 0.0;
	qreal yCameraAngle = -M_PI / 2;
	qreal zCameraAngle = 0.0;

	cameraRotationMatrix[ 0] = qCos(yCameraAngle) * qCos(zCameraAngle);
	cameraRotationMatrix[ 1] = -qSin(zCameraAngle) * qCos(yCameraAngle);
	cameraRotationMatrix[ 2] = qSin(yCameraAngle);
	cameraRotationMatrix[ 3] = 0.0;
	cameraRotationMatrix[ 4] = qSin(xCameraAngle) * qSin(yCameraAngle) * qCos(zCameraAngle) + qSin(zCameraAngle) * qCos(xCameraAngle);
	cameraRotationMatrix[ 5] = -qSin(xCameraAngle) * qSin(yCameraAngle) * qSin(zCameraAngle) + qCos(xCameraAngle) * qCos(zCameraAngle);
	cameraRotationMatrix[ 6] = -qSin(xCameraAngle) * qCos(yCameraAngle);
	cameraRotationMatrix[ 7] = 0.0;
	cameraRotationMatrix[ 8] = -qCos(xCameraAngle) * qSin(yCameraAngle) * qCos(zCameraAngle) + qSin(xCameraAngle) * qSin(zCameraAngle);
	cameraRotationMatrix[ 9] = qCos(xCameraAngle) * qSin(yCameraAngle) * qSin(zCameraAngle) + qSin(xCameraAngle) * qCos(zCameraAngle);
	cameraRotationMatrix[10] = qCos(xCameraAngle) * qCos(yCameraAngle);
	cameraRotationMatrix[11] = 0.0;
	cameraRotationMatrix[12] = 0.0;
	cameraRotationMatrix[13] = 0.0;
	cameraRotationMatrix[14] = 0.0;
	cameraRotationMatrix[15] = 1.0;

	cameraTranslationMatrix[ 0] = 1;
	cameraTranslationMatrix[ 1] = 0;
	cameraTranslationMatrix[ 2] = 0;
	cameraTranslationMatrix[ 3] = -m_cameraPoint.x();
	cameraTranslationMatrix[ 4] = 0;
	cameraTranslationMatrix[ 5] = 1;
	cameraTranslationMatrix[ 6] = 0;
	cameraTranslationMatrix[ 7] = -m_cameraPoint.y();
	cameraTranslationMatrix[ 8] = 0;
	cameraTranslationMatrix[ 9] = 0;
	cameraTranslationMatrix[10] = 1;
	cameraTranslationMatrix[11] = -m_cameraPoint.z();
	cameraTranslationMatrix[12] = 0;
	cameraTranslationMatrix[13] = 0;
	cameraTranslationMatrix[14] = 0;
	cameraTranslationMatrix[15] = 1;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			qreal s = 0;
			for (int k = 0; k < 4; ++k) {
				s += cameraRotationMatrix[i * 4 + k] * cameraTranslationMatrix[k * 4 + j];
			}
			viewMatrix[i * 4 + j] = s;
		}
	}

	projectionMatrix[ 0] = m_zn;
	projectionMatrix[ 1] = 0;
	projectionMatrix[ 2] = 0;
	projectionMatrix[ 3] = 0;
	projectionMatrix[ 4] = 0;
	projectionMatrix[ 5] = m_zn;
	projectionMatrix[ 6] = 0;
	projectionMatrix[ 7] = 0;
	projectionMatrix[ 8] = 0;
	projectionMatrix[ 9] = 0;
	projectionMatrix[10] = -1;
	projectionMatrix[11] = -2 * m_zn;
	projectionMatrix[12] = 0;
	projectionMatrix[13] = 0;
	projectionMatrix[14] = -1;
	projectionMatrix[15] = 0;

	translationMatrix[ 0] = 1;
	translationMatrix[ 1] = 0;
	translationMatrix[ 2] = 0;
	translationMatrix[ 3] = 0;
	translationMatrix[ 4] = 0;
	translationMatrix[ 5] = 1;
	translationMatrix[ 6] = 0;
	translationMatrix[ 7] = 0;
	translationMatrix[ 8] = 0;
	translationMatrix[ 9] = 0;
	translationMatrix[10] = 1;
	translationMatrix[11] = 0;
	translationMatrix[12] = 0;
	translationMatrix[13] = 0;
	translationMatrix[14] = 0;
	translationMatrix[15] = 1;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			qreal s = 0;
			for (int k = 0; k < 4; ++k) {
				s += viewMatrix[i * 4 + k] * rotationMatrix[k * 4 + j];
			}
			tmpMatrix1[i * 4 + j] = s;
		}
	}


	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			qreal s = 0;
			for (int k = 0; k < 4; ++k) {
				s += translationMatrix[i * 4 + k] * tmpMatrix1[k * 4 + j];
			}
			tmpMatrix2[i * 4 + j] = s;
		}
	}

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			qreal s = 0;
			for (int k = 0; k < 4; ++k) {
				s += projectionMatrix[i * 4 + k] * tmpMatrix2[k * 4 + j];
			}
			m_matrix[i * 4 + j] = s;
		}
	}
}

void WireframeMainScene::showBlank() {
	if (m_figuresAdded) {
		for (auto f: m_formings) {
			removeItem(f);
		}
		for (auto c: m_circles) {
			removeItem(c);
		}
	}
	m_figuresAdded = false;
}

void WireframeMainScene::showProjection() {
	for (auto f: m_formings) {
		f->updateForming(&m_xAngle, &m_yAngle, &m_zAngle, &m_cameraPoint, &m_zn, m_matrix);
		if (!m_figuresAdded) {
			addItem(f);
		}
	}
	for (auto c: m_circles) {
		c->updateCircle(&m_xAngle, &m_yAngle, &m_zAngle, &m_cameraPoint, &m_zn, m_matrix);
		if (!m_figuresAdded) {
			addItem(c);
		}
	}
	m_figuresAdded = true;
}





















