#ifndef ECGWIREFRAME_H
#define ECGWIREFRAME_H

#include <QApplication>

#include "src/widgets/mainwindow/mainwindow.h"

class ECGWireframe : public QApplication {
public:

	ECGWireframe(int &argc, char **argv);

	int exec();

private:

	MainWindow mainWindow;

private slots:

	void onOpenSceneActionTriggered();
	void onSaveSceneActionTriggered();
	void resetAnglesActionTriggered();
	void onOpenEditorActionTriggered();
};

#endif // ECGWIREFRAME_H
