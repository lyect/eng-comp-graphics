#include "ecgwireframe.h"

// ##############
// #   PUBLIC   #
// ##############

ECGWireframe::ECGWireframe(int &argc, char **argv) : QApplication(argc, argv) {
	QObject::connect(&m_mainWindow, &MainWindow::openSceneActionTriggered, this, &ECGWireframe::onOpenSceneActionTriggered);
	QObject::connect(&m_mainWindow, &MainWindow::saveSceneActionTriggered, this, &ECGWireframe::onSaveSceneActionTriggered);
	QObject::connect(&m_mainWindow, &MainWindow::resetAnglesActionTriggered, this, &ECGWireframe::resetAnglesActionTriggered);
	QObject::connect(&m_mainWindow, &MainWindow::openEditorActionTriggered, this, &ECGWireframe::onOpenEditorActionTriggered);
}

int ECGWireframe::exec() {

	m_mainWindow.show();

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
	m_wireframeEditor.exec();
}
