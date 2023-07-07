#include "wireframeeditor.h"

#include "src/common/constants.h"

// ##############
// #   PUBLIC   #
// ##############

WireframeEditor::WireframeEditor(QWidget *parent) : QDialog(parent) {
	setMinimumWidth(Constants::MIN_EDITOR_WIDTH);
	setMinimumHeight(Constants::MIN_EDITOR_HEIGHT);

	setFixedSize(Constants::DEFAULT_EDITOR_WIDTH, Constants::DEFAULT_EDITOR_HEIGHT);

	m_mainLayout = new QVBoxLayout();
	m_parametersLayout = new QHBoxLayout();

	m_controlPointEditorWidget = new ControlPointEditorWidget(this);
	m_renderParametersWidget = new RenderParametersWidget(this);
	m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
	m_wfeView = new WireframeEditorView(this);

	m_mainLayout->addWidget(m_wfeView);
	m_parametersLayout->addWidget(m_renderParametersWidget);
	m_parametersLayout->addWidget(m_controlPointEditorWidget);
	m_mainLayout->addLayout(m_parametersLayout);
	m_mainLayout->addWidget(m_buttonBox);

	setLayout(m_mainLayout);

	QObject::connect(
				m_renderParametersWidget, &RenderParametersWidget::onNParameterChanged,
				m_wfeView, &WireframeEditorView::setSplinePartition
	);

	QObject::connect(m_wfeView, &WireframeEditorView::sizeUpdated, this, [this](int w, int h) -> void {
		m_controlPointEditorWidget->setXRange(
					-(w / 2 - Constants::EDITOR_CONTROL_POINT_RADIUS),
					  w / 2 - Constants::EDITOR_CONTROL_POINT_RADIUS
		);
		m_controlPointEditorWidget->setYRange(
					-(h / 2 - Constants::EDITOR_CONTROL_POINT_RADIUS),
					  h / 2 - Constants::EDITOR_CONTROL_POINT_RADIUS
		);
	});
	QObject::connect(m_wfeView, &WireframeEditorView::infoUpdated, m_controlPointEditorWidget, &ControlPointEditorWidget::updateInfo);

	QObject::connect(m_controlPointEditorWidget, &ControlPointEditorWidget::prevControlPoint, m_wfeView, &WireframeEditorView::selectPrevControlPoint);
	QObject::connect(m_controlPointEditorWidget, &ControlPointEditorWidget::nextControlPoint, m_wfeView, &WireframeEditorView::selectNextControlPoint);
	QObject::connect(m_controlPointEditorWidget, &ControlPointEditorWidget::addControlPoint, m_wfeView, &WireframeEditorView::addControlPointToCenter);
	QObject::connect(m_controlPointEditorWidget, &ControlPointEditorWidget::deleteControlPoint, m_wfeView, &WireframeEditorView::deleteSelectedControlPoint);
	QObject::connect(m_controlPointEditorWidget, &ControlPointEditorWidget::controlPointCoordinatesChanged, m_wfeView, &WireframeEditorView::updateSelectedControlPointCoordinates);

	QObject::connect(m_buttonBox, &QDialogButtonBox::accepted, this, &WireframeEditor::accept);
	QObject::connect(m_buttonBox, &QDialogButtonBox::rejected, this, &WireframeEditor::reject);
	QObject::connect(this, &WireframeEditor::accepted, this, &WireframeEditor::onAccepted);
	QObject::connect(this, &WireframeEditor::rejected, this, &WireframeEditor::onRejected);
}

Wireframe WireframeEditor::getWireframe() const {
	return Wireframe(
			m_renderParametersWidget->getParameters(),
			m_wfeView->getControlPoints(),
			m_wfeView->scene()->sceneRect().width(),
			m_wfeView->scene()->sceneRect().height()
	);
}

void WireframeEditor::setWireframe(const Wireframe &wireframe) {
	m_renderParametersWidget->setParameters(wireframe.getRenderParameters());
	m_wfeView->clear();
	m_wfeView->setControlPoints(wireframe.getControlPoints());
}

// ####################
// #   PUBLIC SLOTS   #
// ####################

int WireframeEditor::exec() {
	m_openWireframe = getWireframe();
	return QDialog::exec();
}

// #####################
// #   PRIVATE SLOTS   #
// #####################

void WireframeEditor::onAccepted() {
	emit editorAccepted(getWireframe());
}

void WireframeEditor::onRejected() {
	setWireframe(m_openWireframe);
}
