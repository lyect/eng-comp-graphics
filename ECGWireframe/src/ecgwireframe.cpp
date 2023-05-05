#include "ecgwireframe.h"

// ##############
// #   PUBLIC   #
// ##############

ECGWireframe::ECGWireframe(int &argc, char **argv) : QApplication(argc, argv) {
	QObject::connect(&m_mainWindow, &MainWindow::openSceneActionTriggered, this, &ECGWireframe::onOpenSceneActionTriggered);
	QObject::connect(&m_mainWindow, &MainWindow::saveSceneActionTriggered, this, &ECGWireframe::onSaveSceneActionTriggered);
	QObject::connect(&m_mainWindow, &MainWindow::openEditorActionTriggered, this, &ECGWireframe::onOpenEditorActionTriggered);
	QObject::connect(&m_wireframeEditor, &WireframeEditor::editorAccepted, this, &ECGWireframe::onEditorAccepted);
}

int ECGWireframe::exec() {

	m_mainWindow.show();

	return QApplication::exec();
}

// ###############
// #   PRIVATE   #
// ###############

void ECGWireframe::onOpenSceneActionTriggered(QString chosenFilePath) {
	QFile saveFile(chosenFilePath);

	if (!saveFile.open(QFile::ReadOnly)) {
		m_mainWindow.showWarning("Failed to save file");
		return;
	}

	QDataStream loadStream(&saveFile);
	QByteArray binaryData;
	Wireframe wireframe;

	loadStream >> binaryData;

	if (wireframe.fromBinaryData(binaryData) == -1) {
		m_mainWindow.showWarning("Broken file");
	}
	else {
		m_mainWindow.showWireframe(wireframe, true);
		m_wireframeEditor.setWireframe(wireframe);
	}

	saveFile.close();
}

void ECGWireframe::onSaveSceneActionTriggered(QString chosenFilePath) {
	QFile saveFile(chosenFilePath);

	if (!saveFile.open(QFile::WriteOnly)) {
		m_mainWindow.showWarning("Failed to save file");
		return;
	}

	auto wireframe = m_wireframeEditor.getWireframe();

	QDataStream saveStream(&saveFile);
	saveStream << wireframe.getBinaryData();
	saveFile.close();
}

void ECGWireframe::onOpenEditorActionTriggered() {
	m_wireframeEditor.exec();
}

void ECGWireframe::onEditorAccepted(const Wireframe &wireframe) {
	m_mainWindow.showWireframe(wireframe, false);
}
