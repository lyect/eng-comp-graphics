#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QToolBar>
#include <QMenuBar>
#include <QAction>
#include <QSlider>
#include <QLabel>

#include "src/widgets/view/wireframemainview.h"

class MainWindow : public QMainWindow {

	Q_OBJECT

public:

	explicit MainWindow(QWidget *parent = nullptr);

	void showWireframe(const Wireframe &wireframe, bool reset);
	void showWarning(const QString &text);

private:

	QToolBar *m_toolbar;

	QAction *m_openSceneAction;
	QAction *m_saveSceneAction;

	QAction *m_resetAnglesAction;
	QAction *m_openEditorAction;

	QAction *m_aboutAction;

	QWidget *m_centralWidget;
	QVBoxLayout *m_mainLayout;

	WireframeMainView *m_wfmView;

	void initializeInterface();

signals:

	void openSceneActionTriggered(QString chosenFilePath);
	void saveSceneActionTriggered(QString chosenFilePath);

	void openEditorActionTriggered();
};

#endif // MAINWINDOW_H
