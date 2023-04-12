#ifndef TOOLDIALOG_H
#define TOOLDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>

#include "src/dialogs/dialogparameter.h"

class ToolDialog : public QDialog {

	Q_OBJECT

public:

	explicit ToolDialog(QWidget *parent = nullptr);

	void setParameterValue(const int &parameterIndex, const int &parameterValue);
	int getParameterValue(int parameterIndex) const;

	void open();

protected:

	[[nodiscard]] qsizetype addParameter(
			const QString &parameterName,
			const int &parameterMinValue,
			const int &parameterDefaultValue,
			const int &parameterMaxValue
	);

private:

	QVBoxLayout *layout;
	QDialogButtonBox *buttonBox;

	QVector<DialogParameter *> parameters;
	QVector<int> openParameterValues;

private slots:

	void onAccepted();
	void onRejected();
};

#endif // TOOLDIALOG_H
