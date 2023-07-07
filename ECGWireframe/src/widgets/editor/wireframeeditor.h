#ifndef WIREFRAMEEDITOR_H
#define WIREFRAMEEDITOR_H

#include <QDialog>
#include <QDialogButtonBox>

#include "src/common/wireframe.h"
#include "src/widgets/editor/controlpointeditorwidget.h"
#include "src/widgets/editor/renderparameterswidget.h"
#include "src/widgets/view/wireframeeditorview.h"

class WireframeEditor : public QDialog {

	Q_OBJECT

public:

	explicit WireframeEditor(QWidget *parent = nullptr);

	Wireframe getWireframe() const;
	void setWireframe(const Wireframe &wireframe);

private:

	QVBoxLayout *m_mainLayout;
	QHBoxLayout *m_parametersLayout;

	WireframeEditorView *m_wfeView;
	ControlPointEditorWidget *m_controlPointEditorWidget;
	RenderParametersWidget *m_renderParametersWidget;
	QDialogButtonBox *m_buttonBox;

	Wireframe m_openWireframe;

signals:

	void editorAccepted(const Wireframe &wireframe);

public slots:

	int exec() override;

private slots:

	void onAccepted();
	void onRejected();
};

#endif // WIREFRAMEEDITOR_H
