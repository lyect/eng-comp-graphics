#ifndef WIREFRAMEEDITOR_H
#define WIREFRAMEEDITOR_H

#include <QDialog>
#include <QDialogButtonBox>

#include "src/common/editorstate.h"
#include "src/widgets/editor/controlpointeditorwidget.h"
#include "src/widgets/editor/renderparameterswidget.h"
#include "src/widgets/view/wireframeeditorview.h"

class WireframeEditor : public QDialog {

	Q_OBJECT

public:

	explicit WireframeEditor(QWidget *parent = nullptr);

private:

	QVBoxLayout *m_mainLayout;
	QHBoxLayout *m_parametersLayout;

	WireframeEditorView *m_wfeView;
	ControlPointEditorWidget *m_controlPointEditorWidget;
	RenderParametersWidget *m_renderParametersWidget;
	QDialogButtonBox *m_buttonBox;

	EditorState m_openState;

signals:

	void editorAccepted(const EditorState &state);

public slots:

	int exec() override;

	void onAccepted();
	void onRejected();
};

#endif // WIREFRAMEEDITOR_H
