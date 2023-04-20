#include <QFileDialog>
#include <QMessageBox>

#include "mainwindow.h"
#include "src/common/constants.h"

// ##############
// #   PUBLIC   #
// ##############

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	setMinimumWidth(Constants::MIN_WINDOW_WIDTH);
	setMinimumHeight(Constants::MIN_WINDOW_HEIGHT);

	resize(Constants::DEFAULT_WINDOW_WIDTH, Constants::DEFAULT_WINDOW_HEIGHT);

	setWindowIcon(QIcon(":/icons/window_icon.png"));
	setWindowTitle("ECGWireframe");

	initializeInterface();
}

MainWindow::~MainWindow() {
	initializeInterface();
}

// ###############
// #   PRIVATE   #
// ###############

void MainWindow::initializeInterface() {

	// ========== ACTIONS ============================

	toolbar = addToolBar("Enable toolbar");

	openSceneAction = new QAction(QIcon(":/icons/open_scene_action_icon.png"), "Open scene", this);
	saveSceneAction = new QAction(QIcon(":/icons/save_scene_action_icon.png"), "Save scene", this);

	resetAnglesAction = new QAction(QIcon(":/icons/reset_angles_action_icon.png"), "Reset angles", this);
	openEditorAction = new QAction(QIcon(":/icons/open_editor_action_icon.png"), "Open editor", this);

	aboutAction = new QAction("&About", this);

	QObject::connect(openSceneAction, &QAction::triggered, this, [this]() -> void {
		QString filename = QFileDialog::getOpenFileName(this, "Open Scene", "/home", "WireFrame Scenes (*.wfsc)");

		if (filename.isNull()) {
			return;
		}

		emit openSceneActionTriggered(filename);
	});

	QObject::connect(saveSceneAction, &QAction::triggered, this, [this]() -> void {
		QString filename = QFileDialog::getSaveFileName(this, "Save Scene", "/home/untitled.wfsc", "WireFrame Scenes (*.wfsc)");

		if (filename.isNull()) {
			return;
		}

		emit saveSceneActionTriggered(filename);
	});

	QObject::connect(resetAnglesAction, &QAction::triggered, this, [this]() -> void {
		emit resetAnglesActionTriggered();
	});

	QObject::connect(openEditorAction, &QAction::triggered, this, [this]() -> void {
		emit openEditorActionTriggered();
	});

	QObject::connect(aboutAction, &QAction::triggered, this, [this]() -> void {
		QMessageBox::about(
						this, "About",
						"The third practical work on the subject \"Computer graphics\".\n"
						"Done by Timur Letyagin (FIT NSU).\n"
						"April 2023"
		);
	});

	toolbar->addAction(openSceneAction);
	toolbar->addAction(saveSceneAction);
	toolbar->addSeparator();
	toolbar->addAction(resetAnglesAction);
	toolbar->addAction(openEditorAction);
	toolbar->addSeparator();
	toolbar->addAction(aboutAction);

	QMenu *fileMenu = menuBar()->addMenu("&File");
	QMenu *editMenu = menuBar()->addMenu("&Edit");
	menuBar()->addAction(aboutAction);

	fileMenu->addAction(openSceneAction);
	fileMenu->addAction(saveSceneAction);

	editMenu->addAction(resetAnglesAction);
	editMenu->addAction(openEditorAction);


	// ========== LAYOUTS ============================

	centralWidget = new QWidget(this);
	mainLayout = new QVBoxLayout(centralWidget);

	centralWidget->setLayout(mainLayout);

	setCentralWidget(centralWidget);
}

