#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColorDialog>
#include <QToolBar>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QMouseEvent>
#include <QFileDialog>

#include "src/conf/settings.h"
#include "src/dialogs/resizedialog.h"
#include "src/dialogs/linedialog.h"
#include "src/dialogs/polygondialog.h"
#include "src/dialogs/stardialog.h"
#include "src/paintarea.h"

class MainWindow : public QMainWindow {

	Q_OBJECT

public:

	explicit MainWindow(QWidget *parent = nullptr);

private:

	enum ToolType {
		NO_TOOL,
		LINE_TOOL,
		POLYGON_TOOL,
		STAR_TOOL,
		FILL_TOOL
	};

	QToolBar *toolBar;

	ResizeDialog *resizeDialog;
	LineDialog *lineDialog;
	PolygonDialog *polygonDialog;
	StarDialog *starDialog;

	PaintArea *paintArea;

	ToolType selectedTool;
	QColor selectedColor;
	QPoint prevPoint;

	void initializeInterface();

	void mousePressEvent(QMouseEvent *e);

private slots:

	void onOpenFileAction();
	void onSaveFileAction();
};

#endif // MAINWINDOW_H
