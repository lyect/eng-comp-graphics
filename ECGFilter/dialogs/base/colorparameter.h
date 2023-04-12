#ifndef COLORPARAMETER_H
#define COLORPARAMETER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QColorDialog>

class ColorParameter : public QWidget {

	Q_OBJECT

public:

	ColorParameter(
			const QString &title,
			const QColor &defaultColor,
			QWidget *parent = nullptr
	);

	QColor getColor() const;
	void setColor(const QColor &color);

private:

	QVBoxLayout *mainLayout;
	QLabel *label;
	QPushButton *button;

	QColor lastColor;

	void connectWidgets();

signals:

};

#endif // COLORPARAMETER_H
