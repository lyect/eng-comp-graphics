#include "ecgwireframe.h"

// ##############
// #   PUBLIC   #
// ##############

ECGWireframe::ECGWireframe(int &argc, char **argv) : QApplication(argc, argv) {
	QObject::connect(&mainWindow, &MainWindow::openSceneActionTriggered, this, &ECGWireframe::onOpenSceneActionTriggered);
	QObject::connect(&mainWindow, &MainWindow::saveSceneActionTriggered, this, &ECGWireframe::onSaveSceneActionTriggered);
	QObject::connect(&mainWindow, &MainWindow::resetAnglesActionTriggered, this, &ECGWireframe::resetAnglesActionTriggered);
	QObject::connect(&mainWindow, &MainWindow::openEditorActionTriggered, this, &ECGWireframe::onOpenEditorActionTriggered);
}

int ECGWireframe::exec() {

	mainWindow.show();

	return QApplication::exec();
}

// ###############
// #   PRIVATE   #
// ###############

void ECGWireframe::onOpenSceneActionTriggered() {
	// load .wfsc file
}

void ECGWireframe::onSaveSceneActionTriggered() {
	// save .wfsc file
}

void ECGWireframe::resetAnglesActionTriggered() {
	// reset angles
}

void ECGWireframe::onOpenEditorActionTriggered() {
	// open editor
}
