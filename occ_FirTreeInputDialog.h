
#include "ui_occFirTreeInputDialog.h"
#include "FirTreeCreator.h"

#include <AIS_InteractiveContext.hxx>


#include "qdialog.h"
class occ_FirTreeInputDialog :
	public QDialog
{
	Q_OBJECT
public:
	occ_FirTreeInputDialog(QWidget *parent, Handle_AIS_InteractiveContext context);
	~occ_FirTreeInputDialog();

private:
	Ui::Dialog *ui;
	FirTreeCreator* myFirTreeCreator;

private slots:
	void setChangesOf_SetHubAnglesCheckbox(int state);
	void setChangesOf_SetDepthCuttingCheckBox(int state);
	void setChangesOf_SetFirTreeCheckBox(int state);

	void setChangesOf_FinishButton(void);

	void setChangesOf_a1_doubleSpinBox(void);
	void setChangesOf_a2_doubleSpinBox(void);
	void setChangesOf_a3_doubleSpinBox(void);
	void setChangesOf_a4_doubleSpinBox(void);
	void setChangesOf_fiTreeBottomFilletRadius_doubleSpinBox(void);
	void setChangesOf_fiTreeUpperFilletRadius_doubleSpinBox(void);
	void setChangesOf_neckHeight_doubleSpinBox(void);
	void setChangesOf_neckBottomWidth_doubleSpinBox(void);

	void setChangesOf_depthFromFrontFace_tableWidget(void);
	void depthFromNextToFrontFace_tableWidget(void);
	void setChangesOf_lobeProperties_tableWidget(void);

};

