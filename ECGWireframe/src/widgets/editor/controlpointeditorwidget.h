#ifndef CONTROLPOINTEDITORWIDGET_H
#define CONTROLPOINTEDITORWIDGET_H

#include <QWidget>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QButtonGroup>
#include <QPushButton>

class ControlPointEditorWidget : public QWidget {

	Q_OBJECT

public:

	explicit ControlPointEditorWidget(QWidget *parent = nullptr);

private:

	qsizetype m_currentControlPointNumber;
	qsizetype m_controlPointsCount;

	QVBoxLayout *m_mainLayout;

	QLabel *m_title;

	QHBoxLayout *m_buttonLayout;
	QButtonGroup *m_buttonGroup;
	QPushButton *m_prevButton;
	QPushButton *m_nextButton;
	QPushButton *m_deleteButton;
	QPushButton *m_addButton;

	QLabel *m_controlPointNumberInfo;

	QSpinBox *m_xSpinBox;
	QSpinBox *m_ySpinBox;

	void setNoPointsState();
	void setUnchoosenState();
	void setChoosenState();
	QString constructControlPointNumberInfo();

signals:

	void prevControlPoint();
	void nextControlPoint();
	void deleteControlPoint();
	void addControlPoint();

	void controlPointCoordinatesChanged(qreal x, qreal y);

public slots:

	void setXRange(int xMin, int xMax);
	void setYRange(int yMin, int yMax);
	void updateInfo(qsizetype controlPointsCount, qsizetype controlPointNumber, const QPointF &point);

private slots:

	void onPrevButtonClicked();
	void onNextButtonClicked();
	void onDeleteButtonClicked();
	void onAddButtonClicked();

	void onSpinBoxValuesChanged();
};

#endif // CONTROLPOINTEDITORWIDGET_H
