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

	m_toolbar = addToolBar("Enable toolbar");

	m_openSceneAction = new QAction(QIcon(":/icons/open_scene_action_icon.png"), "Open scene", this);
	m_saveSceneAction = new QAction(QIcon(":/icons/save_scene_action_icon.png"), "Save scene", this);

	m_resetAnglesAction = new QAction(QIcon(":/icons/reset_angles_action_icon.png"), "Reset angles", this);
	m_openEditorAction = new QAction(QIcon(":/icons/open_editor_action_icon.png"), "Open editor", this);

	m_aboutAction = new QAction("&About", this);

	QObject::connect(m_openSceneAction, &QAction::triggered, this, [this]() -> void {
		QString filename = QFileDialog::getOpenFileName(this, "Open Scene", "/home", "WireFrame Scenes (*.wfsc)");

		if (filename.isNull()) {
			return;
		}

		emit openSceneActionTriggered(filename);
	});

	QObject::connect(m_saveSceneAction, &QAction::triggered, this, [this]() -> void {
		QString filename = QFileDialog::getSaveFileName(this, "Save Scene", "/home/untitled.wfsc", "WireFrame Scenes (*.wfsc)");

		if (filename.isNull()) {
			return;
		}

		emit saveSceneActionTriggered(filename);
	});

	QObject::connect(m_resetAnglesAction, &QAction::triggered, this, [this]() -> void {
		emit resetAnglesActionTriggered();
	});

	QObject::connect(m_openEditorAction, &QAction::triggered, this, [this]() -> void {
		emit openEditorActionTriggered();
	});

	QObject::connect(m_aboutAction, &QAction::triggered, this, [this]() -> void {
		QMessageBox::about(
						this, "About",
						"The third practical work on the subject \"Computer graphics\".\n"
						"Done by Timur Letyagin (FIT NSU).\n"
						"April 2023"
		);
	});

	m_toolbar->addAction(m_openSceneAction);
	m_toolbar->addAction(m_saveSceneAction);
	m_toolbar->addSeparator();
	m_toolbar->addAction(m_resetAnglesAction);
	m_toolbar->addAction(m_openEditorAction);
	m_toolbar->addSeparator();
	m_toolbar->addAction(m_aboutAction);

	QMenu *fileMenu = menuBar()->addMenu("&File");
	QMenu *editMenu = menuBar()->addMenu("&Edit");
	menuBar()->addAction(m_aboutAction);

	fileMenu->addAction(m_openSceneAction);
	fileMenu->addAction(m_saveSceneAction);

	editMenu->addAction(m_resetAnglesAction);
	editMenu->addAction(m_openEditorAction);


	// ========== LAYOUTS ============================

	m_centralWidget = new QWidget(this);
	m_mainLayout = new QVBoxLayout(m_centralWidget);

	m_centralWidget->setLayout(m_mainLayout);

	setCentralWidget(m_centralWidget);
}

