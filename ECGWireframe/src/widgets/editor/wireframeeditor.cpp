#include "wireframeeditor.h"

#include "src/common/constants.h"

// ##############
// #   PUBLIC   #
// ##############

WireframeEditor::WireframeEditor(QWidget *parent) : QDialog(parent) {
	setMinimumWidth(Constants::MIN_EDITOR_WIDTH);
	setMinimumHeight(Constants::MIN_EDITOR_HEIGHT);

	resize(Constants::DEFAULT_EDITOR_WIDTH, Constants::DEFAULT_EDITOR_HEIGHT);

	m_mainLayout = new QVBoxLayout();

	m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

	m_renderParametersWidget = new RenderParametersWidget(this);

	// here must be adding of the WireframeView
	m_mainLayout->addWidget(m_renderParametersWidget);
	m_mainLayout->addWidget(m_buttonBox);

	setLayout(m_mainLayout);

	QObject::connect(m_buttonBox, &QDialogButtonBox::accepted, this, &WireframeEditor::accept);
	QObject::connect(m_buttonBox, &QDialogButtonBox::rejected, this, &WireframeEditor::reject);
	QObject::connect(this, &WireframeEditor::accepted, this, &WireframeEditor::onAccepted);
	QObject::connect(this, &WireframeEditor::rejected, this, &WireframeEditor::onRejected);
}

// ####################
// #   PUBLIC SLOTS   #
// ####################

int WireframeEditor::exec() {
	m_renderParametersOnOpen = m_renderParametersWidget->getParameters();
	return QDialog::exec();
}

// #####################
// #   PRIVATE SLOTS   #
// #####################

void WireframeEditor::onAccepted() {
	// emit spline object and render settings
}

void WireframeEditor::onRejected() {
	m_renderParametersWidget->setParameters(m_renderParametersOnOpen);
}
