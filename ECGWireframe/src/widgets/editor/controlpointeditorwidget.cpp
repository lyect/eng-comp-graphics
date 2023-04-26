#include "controlpointeditorwidget.h"
#include "src/common/constants.h"

ControlPointEditorWidget::ControlPointEditorWidget(QWidget *parent): QWidget(parent) {
	m_currentControlPointNumber = 0;
	m_controlPointsCount = 0;

	m_mainLayout = new QVBoxLayout(this);

	m_title = new QLabel("Edit control point:", this);

	m_buttonLayout = new QHBoxLayout();
	m_buttonGroup = new QButtonGroup(this);
	m_prevButton = new QPushButton("<", this);
	m_nextButton = new QPushButton(">", this);
	m_deleteButton = new QPushButton("-", this);
	m_addButton = new QPushButton("+", this);
	m_buttonGroup->addButton(m_prevButton, 1);
	m_buttonGroup->addButton(m_nextButton, 2);
	m_buttonGroup->addButton(m_deleteButton, 3);
	m_buttonGroup->addButton(m_addButton, 4);
	m_buttonLayout->addWidget(m_prevButton);
	m_buttonLayout->addWidget(m_nextButton);
	m_buttonLayout->addWidget(m_deleteButton);
	m_buttonLayout->addWidget(m_addButton);

	m_controlPointNumberInfo = new QLabel(this);

	m_xSpinBox = new QSpinBox(this);
	m_ySpinBox = new QSpinBox(this);

	m_mainLayout->addWidget(m_title);
	m_mainLayout->addLayout(m_buttonLayout);
	m_mainLayout->addWidget(m_controlPointNumberInfo);
	m_mainLayout->addWidget(m_xSpinBox);
	m_mainLayout->addWidget(m_ySpinBox);

	setLayout(m_mainLayout);

	QObject::connect(m_prevButton, &QPushButton::clicked, this, &ControlPointEditorWidget::onPrevButtonClicked);
	QObject::connect(m_nextButton, &QPushButton::clicked, this, &ControlPointEditorWidget::onNextButtonClicked);
	QObject::connect(m_deleteButton, &QPushButton::clicked, this, &ControlPointEditorWidget::onDeleteButtonClicked);
	QObject::connect(m_addButton, &QPushButton::clicked, this, &ControlPointEditorWidget::onAddButtonClicked);

	QObject::connect(m_xSpinBox, &QSpinBox::valueChanged, this, &ControlPointEditorWidget::onSpinBoxValuesChanged);
	QObject::connect(m_ySpinBox, &QSpinBox::valueChanged, this, &ControlPointEditorWidget::onSpinBoxValuesChanged);

	setNoPointsState();
}

// ###############
// #   PRIVATE   #
// ###############

void ControlPointEditorWidget::setNoPointsState() {
	m_controlPointNumberInfo->setText("No points on the scene");
	m_prevButton->setEnabled(false);
	m_nextButton->setEnabled(false);
	m_deleteButton->setEnabled(false);
	m_xSpinBox->setEnabled(false);
	m_ySpinBox->setEnabled(false);
}

void ControlPointEditorWidget::setUnchoosenState() {
	m_controlPointNumberInfo->setText("Point is not choosen");
	m_prevButton->setEnabled(false);
	m_nextButton->setEnabled(false);
	m_deleteButton->setEnabled(false);
	m_xSpinBox->setEnabled(false);
	m_ySpinBox->setEnabled(false);
}

void ControlPointEditorWidget::setChoosenState() {
	m_controlPointNumberInfo->setText(constructControlPointNumberInfo());
	if (m_currentControlPointNumber > 1) {
		m_prevButton->setEnabled(true);
	}
	else {
		m_prevButton->setEnabled(false);
	}
	if (m_currentControlPointNumber < m_controlPointsCount) {
		m_nextButton->setEnabled(true);
	}
	else {
		m_nextButton->setEnabled(false);
	}
	m_deleteButton->setEnabled(true);
	m_xSpinBox->setEnabled(true);
	m_ySpinBox->setEnabled(true);
}

QString ControlPointEditorWidget::constructControlPointNumberInfo() {
	return "Point #" + QString::number(m_currentControlPointNumber) + "/" + QString::number(m_controlPointsCount);
}

// ####################
// #   PUBLIC SLOTS   #
// ####################

void ControlPointEditorWidget::setXRange(int xMin, int xMax) {
	m_xSpinBox->setRange(xMin, xMax);
	m_xSpinBox->setRange(xMin, xMax);
}

void ControlPointEditorWidget::setYRange(int yMin, int yMax) {
	m_ySpinBox->setRange(yMin, yMax);
}

void ControlPointEditorWidget::updateInfo(qsizetype controlPointsCount, qsizetype controlPointNumber, const QPointF &point) {

	m_currentControlPointNumber = controlPointNumber;
	m_controlPointsCount = controlPointsCount;

	if (m_controlPointsCount == 0) {
		setNoPointsState();
		return;
	}

	if (m_currentControlPointNumber == 0) {
		setUnchoosenState();
		return;
	}

	setChoosenState();

	m_xSpinBox->blockSignals(true);
	m_ySpinBox->blockSignals(true);
	m_xSpinBox->setValue(static_cast<int>(point.x() - Constants::EDITOR_CONTROL_POINT_RADIUS - 1 + m_xSpinBox->minimum()));
	m_ySpinBox->setValue(static_cast<int>(-(point.y() - Constants::EDITOR_CONTROL_POINT_RADIUS - 1 + m_ySpinBox->minimum())));
	m_xSpinBox->blockSignals(false);
	m_ySpinBox->blockSignals(false);
}


// #####################
// #   PRIVATE SLOTS   #
// #####################

void ControlPointEditorWidget::onPrevButtonClicked() {
	emit prevControlPoint();
}

void ControlPointEditorWidget::onNextButtonClicked() {
	emit nextControlPoint();
}

void ControlPointEditorWidget::onDeleteButtonClicked() {
	emit deleteControlPoint();
}

void ControlPointEditorWidget::onAddButtonClicked() {
	emit addControlPoint();
}

void ControlPointEditorWidget::onSpinBoxValuesChanged() {
	qreal xCoord = static_cast<qreal>(m_xSpinBox->value() - m_xSpinBox->minimum());
	qreal yCoord = static_cast<qreal>(-m_ySpinBox->value() - m_ySpinBox->minimum());
	emit controlPointCoordinatesChanged(xCoord, yCoord);
}
