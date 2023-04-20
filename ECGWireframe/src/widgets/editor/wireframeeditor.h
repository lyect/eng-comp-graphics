#ifndef WIREFRAMEEDITOR_H
#define WIREFRAMEEDITOR_H

#include <QDialog>
#include <QDialogButtonBox>

#include "src/common/renderparameters.h"
#include "src/widgets/editor/renderparameterswidget.h"

class WireframeEditor : public QDialog {

	Q_OBJECT

public:

	explicit WireframeEditor(QWidget *parent = nullptr);

private:

	QVBoxLayout *m_mainLayout;

	QDialogButtonBox *m_buttonBox;

	RenderParametersWidget *m_renderParametersWidget;
	RenderParameters m_renderParametersOnOpen;

public slots:

	int exec() override;

public slots:

	void onAccepted();
	void onRejected();
};

#endif // WIREFRAMEEDITOR_H
