#ifndef WIREFRAMESTATE_H
#define WIREFRAMESTATE_H

#include <QTransform>
#include <QVector>
#include <QPointF>
#include <QByteArray>

#include "src/common/renderparameters.h"

class Wireframe {
public:

	Wireframe() = default;

	Wireframe(
			const RenderParameters &renderParameters,
			const QVector<QPointF> &controlPoints,
			qreal width,
			qreal height
	);

	void setRenderParameters(const RenderParameters &renderParameters);
	void setControlPoints(const QVector<QPointF> &controlPoints);
	void setWidth(qreal width);
	void setHeight(qreal height);
	const RenderParameters getRenderParameters() const;
	const QVector<QPointF> &getControlPoints() const;
	qreal getWidth() const;
	qreal getHeight() const;

	int fromBinaryData(const QByteArray &ba);
	QByteArray getBinaryData() const;

private:

	QVector<QPointF> m_controlPoints;
	RenderParameters m_renderParameters;

	qreal m_width;
	qreal m_height;
};

#endif // WIREFRAMESTATE_H
