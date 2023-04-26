#ifndef EDITORSTATE_H
#define EDITORSTATE_H

#include <QTransform>
#include <QVector>
#include <QPointF>

#include "src/common/renderparameters.h"

class EditorState {
public:

	EditorState() = default;

	EditorState(
			const RenderParameters &renderParameters,
			const QVector<QPointF> &controlPoints
	);

	void setRenderParameters(const RenderParameters &renderParameters);
	void setControlPoints(const QVector<QPointF> &controlPoints);
	const RenderParameters getRenderParameters() const;
	const QVector<QPointF> &getControlPoints() const;

private:

	QVector<QPointF> m_controlPoints;
	RenderParameters m_renderParameters;
};

#endif // EDITORSTATE_H
