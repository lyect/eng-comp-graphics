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

	QToolBar *toolbar;

	QAction *openSceneAction;
	QAction *saveSceneAction;

	QAction *resetAnglesAction;
	QAction *openEditorAction;

	QAction *aboutAction;

	QWidget *centralWidget;
	QVBoxLayout *mainLayout;

	void initializeInterface();

signals:

	void openSceneActionTriggered(QString chosenFilePath);
	void saveSceneActionTriggered(QString chosenFilePath);

	void resetAnglesActionTriggered();
	void openEditorActionTriggered();
};

#endif // MAINWINDOW_H
