#ifndef ECGWIREFRAME_H
#define ECGWIREFRAME_H

#include <QApplication>
#include <QFile>

#include "src/widgets/mainwindow/mainwindow.h"
#include "src/widgets/editor/wireframeeditor.h"

class ECGWireframe : public QApplication {
public:

	ECGWireframe(int &argc, char **argv);

	int exec();

private:

	MainWindow m_mainWindow;
	WireframeEditor m_wireframeEditor;

private slots:

	void onOpenSceneActionTriggered(QString chosenFilePath);
	void onSaveSceneActionTriggered(QString chosenFilePath);
	void onOpenEditorActionTriggered();

	void onEditorAccepted(const Wireframe &wireframe);
};

#endif // ECGWIREFRAME_H
