#include "editorstate.h"

EditorState::EditorState(
		const RenderParameters &renderParameters,
		const QVector<QPointF> &controlPoints
) {
	m_renderParameters = renderParameters;
	m_controlPoints = controlPoints;
}

void EditorState::setRenderParameters(const RenderParameters &renderParameters) {
	m_renderParameters = renderParameters;
}

void EditorState::setControlPoints(const QVector<QPointF> &controlPoints) {
	m_controlPoints = controlPoints;
}

const RenderParameters EditorState::getRenderParameters() const {
	return m_renderParameters;
}

const QVector<QPointF> &EditorState::getControlPoints() const {
	return m_controlPoints;
}

