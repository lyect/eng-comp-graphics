#include "wireframe.h"

#include <QIODevice>

Wireframe::Wireframe(
		const RenderParameters &renderParameters,
		const QVector<QPointF> &controlPoints,
		qreal width,
		qreal height
) {
	m_renderParameters = renderParameters;
	m_controlPoints = controlPoints;
	m_width = width;
	m_height = height;
}

void Wireframe::setRenderParameters(const RenderParameters &renderParameters) {
	m_renderParameters = renderParameters;
}

void Wireframe::setControlPoints(const QVector<QPointF> &controlPoints) {
	m_controlPoints = controlPoints;
}

void Wireframe::setWidth(qreal width) {
	m_width = width;
}

void Wireframe::setHeight(qreal height) {
	m_height = height;
}

const RenderParameters Wireframe::getRenderParameters() const {
	return m_renderParameters;
}

const QVector<QPointF> &Wireframe::getControlPoints() const {
	return m_controlPoints;
}

qreal Wireframe::getWidth() const {
	return m_width;
}

qreal Wireframe::getHeight() const {
	return m_height;
}

int Wireframe::fromBinaryData(const QByteArray &ba) {
	QDataStream stream(ba);
	stream.setVersion(QDataStream::Qt_6_4);

	qsizetype n;
	unsigned long long size;
	int N, M1, M2;

	stream >> m_width >> m_height;
	stream >> N >> M1 >> M2;
	m_renderParameters.setN(N);
	m_renderParameters.setM1(M1);
	m_renderParameters.setM2(M2);
	stream >> n;
	m_controlPoints.resize(n);
	for (int i = 0; i < n; ++i) {
		qreal x, y;
		stream >> x >> y;
		m_controlPoints[i].setX(x);
		m_controlPoints[i].setY(y);
	}

	stream >> size;
	if (size != sizeof(m_width) + sizeof(m_height) + 3 * sizeof(int) + sizeof(n) + n * sizeof(QPointF)) {
		return -1;
	}

	if (!stream.atEnd()) {
		return -1;
	}

	return 0;
}

QByteArray Wireframe::getBinaryData() const {
	QByteArray byteArray;

	QDataStream stream(&byteArray, QIODevice::WriteOnly);
	stream.setVersion(QDataStream::Qt_6_4);

	stream << m_width << m_height;
	stream << m_renderParameters.getN() << m_renderParameters.getM1() << m_renderParameters.getM2();
	stream << m_controlPoints.count();
	for (const auto &p : m_controlPoints) {
		stream << p.x() << p.y();
	}
	stream << sizeof(m_width) + sizeof(m_height) + 3 * sizeof(int) + sizeof(m_controlPoints.count()) + m_controlPoints.count() * sizeof(QPointF);
	return byteArray;
}

