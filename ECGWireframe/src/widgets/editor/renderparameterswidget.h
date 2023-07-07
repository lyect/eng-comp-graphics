#ifndef RENDERPARAMETERSWIDGET_H
#define RENDERPARAMETERSWIDGET_H

#include <QFormLayout>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

#include "src/common/renderparameters.h"

class RenderParametersWidget : public QWidget {

	Q_OBJECT

public:

	explicit RenderParametersWidget(QWidget *parent = nullptr);

	RenderParameters getParameters() const;
	void setParameters(const RenderParameters &params);

private:

	QVBoxLayout *m_mainLayout;
	QFormLayout *m_formLayout;

	QLabel *m_titleLabel;

	QSpinBox *m_nSpinBox;
	QSpinBox *m_m1SpinBox;
	QSpinBox *m_m2SpinBox;

signals:

	void onNParameterChanged(qsizetype N);
};

#endif // RENDERPARAMETERSWIDGET_H
