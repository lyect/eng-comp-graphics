#ifndef PARAMETERDIALOG_H
#define PARAMETERDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>

#include "numericparameter.h"
#include "colorparameter.h"
#include "switchparameter.h"

class ParameterDialog : public QDialog {

	Q_OBJECT

public:
	explicit ParameterDialog(QWidget *parent = nullptr);

	void setNumericParameterValue(const int &numericParameterIndex, const float &numericParameterValue);
	float getNumericParameterValue(const int &numericParameterIndex) const;

	void setColorParameterValue(const int &colorParameterIndex, const QColor &colorParameterValue);
	QColor getColorParameterValue(const int &colorParameterIndex) const;

	void setSwitchParameterChoice(const int &switchParameterIndex, const qsizetype &choice);
	qsizetype getSwitchParameterChoice(const int &switchParameterIndex) const;

protected:

	[[nodiscard]] qsizetype addNumericParameter(
			const QString &parameterName,
			const float &parameterMinValue,
			const float &parameterMaxValue,
			const float &parameterDefaultValue,
			const float &step
	);

	[[nodiscard]] qsizetype addColorParameter(
			const QString &parameterName,
			const QColor &parameterDefaultColor
	);

	[[nodiscard]] qsizetype addSwitchParameter(
			const QString &title,
			const qsizetype &choicesNumber,
			const QVector<QString> &choicesNames,
			const qsizetype &defaultChoice
	);

private:

	QVBoxLayout *mainLayout;
	QDialogButtonBox *buttonBox;

	QVector<NumericParameter *> numericParameters;
	QVector<ColorParameter *> colorParameters;
	QVector<SwitchParameter *> switchParameters;

	QVector<float> openNumericParameterValues;
	QVector<QColor> openColorParameterValues;
	QVector<qsizetype> openSwitchParameterValues;

public slots:

	int exec() override;

private slots:

	void onAccepted();
	void onRejected();
};

#endif // PARAMETERDIALOG_H
