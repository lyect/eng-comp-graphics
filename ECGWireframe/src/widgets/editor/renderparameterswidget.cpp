#include "renderparameterswidget.h"

#include "src/common/constants.h"

RenderParametersWidget::RenderParametersWidget(QWidget *parent) : QWidget(parent) {
	m_mainLayout = new QVBoxLayout();
	m_formLayout = new QFormLayout();

	m_titleLabel = new QLabel("Render parameters", this);

	m_nSpinBox = new QSpinBox(this);
	m_m1SpinBox = new QSpinBox(this);
	m_m2SpinBox = new QSpinBox(this);

	m_nSpinBox->setRange(
				Constants::MIN_N_RENDER_PARAMETER_VALUE,
				Constants::MAX_N_RENDER_PARAMETER_VALUE
	);
	m_m1SpinBox->setRange(
				Constants::MIN_M1_RENDER_PARAMETER_VALUE,
				Constants::MAX_M1_RENDER_PARAMETER_VALUE
	);
	m_m2SpinBox->setRange(
				Constants::MIN_M2_RENDER_PARAMETER_VALUE,
				Constants::MAX_M2_RENDER_PARAMETER_VALUE
	);

	m_nSpinBox->setValue(Constants::DEFAULT_N_RENDER_PARAMETER_VALUE);
	m_m1SpinBox->setValue(Constants::DEFAULT_M1_RENDER_PARAMETER_VALUE);
	m_m2SpinBox->setValue(Constants::DEFAULT_M2_RENDER_PARAMETER_VALUE);

	m_formLayout->addRow("N:", m_nSpinBox);
	m_formLayout->addRow("M1:", m_m1SpinBox);
	m_formLayout->addRow("M2:", m_m2SpinBox);

	m_mainLayout->addWidget(m_titleLabel);
	m_mainLayout->addLayout(m_formLayout);

	setLayout(m_mainLayout);
}

RenderParameters RenderParametersWidget::getParameters() const {
	return RenderParameters(
				m_nSpinBox->value(),
				m_m1SpinBox->value(),
				m_m2SpinBox->value()
	);
}

void RenderParametersWidget::setParameters(const RenderParameters &params) {
	m_nSpinBox->setValue(params.getN());
	m_m1SpinBox->setValue(params.getM1());
	m_m2SpinBox->setValue(params.getM2());
}
