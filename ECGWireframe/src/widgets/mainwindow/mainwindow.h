#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QToolBar>
#include <QMenuBar>
#include <QAction>

class MainWindow : public QMainWindow {

	Q_OBJECT

public:

	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private:

	QToolBar *m_toolbar;

	QAction *m_openSceneAction;
	QAction *m_saveSceneAction;

	QAction *m_resetAnglesAction;
	QAction *m_openEditorAction;

	QAction *m_aboutAction;

	QWidget *m_centralWidget;
	QVBoxLayout *m_mainLayout;

	void initializeInterface();

signals:

	void openSceneActionTriggered(QString chosenFilePath);
	void saveSceneActionTriggered(QString chosenFilePath);

	void resetAnglesActionTriggered();
	void openEditorActionTriggered();
};

#endif // MAINWINDOW_H
