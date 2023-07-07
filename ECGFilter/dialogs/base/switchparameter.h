#ifndef SWITCHPARAMETER_H
#define SWITCHPARAMETER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QLabel>
#include <QRadioButton>
#include <QVector>

class SwitchParameter : public QWidget {

	Q_OBJECT

public:

	SwitchParameter(
			const QString &title,
			const qsizetype &choicesNumber,
			const QVector<QString> &choicesNames,
			const qsizetype &defaultChoice,
			QWidget *parent = nullptr
	);

	qsizetype getChoice() const;
	void setChoice(const qsizetype &choice);
private:

	QVBoxLayout *mainLayout;
	QLabel *label;
	QButtonGroup *choicesGroup;

	qsizetype lastChoice;

signals:

};

#endif // COLORPARAMETER_H

