#ifndef DIALOGPARAMETER_H
#define DIALOGPARAMETER_H

#include <QLabel>
#include <QSlider>
#include <QLineEdit>
#include <QString>
#include <QVBoxLayout>

class DialogParameter : public QWidget {

	Q_OBJECT

public:

	DialogParameter(
		const QString &title,
		const int &minValue,
		const int &defaultValue,
		const int &maxValue,
		QWidget *parent = nullptr
	);

	void setValue(const int &value);
	int getValue() const;

private:

	QVBoxLayout *layout;

	QLabel *parameterLabel;
	QSlider *parameterSlider;
	QLineEdit *parameterLineEdit;

	int minValue;
	int defaultValue;
	int maxValue;

	QString minValueText;
	QString defaultValueText;
	QString maxValueText;

	int lastCorrectValue;

	void initializeWidgets();
	void connectWidgets();
};

#endif // DIALOGPARAMETER_H
