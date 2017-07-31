#include "occ_FirTreeInputDialog.h"

#include <QtMath>





occ_FirTreeInputDialog::occ_FirTreeInputDialog(QWidget *parent, Handle_AIS_InteractiveContext context) :QDialog(parent)
{
	ui = new Ui::Dialog();
	ui->setupUi(this);

    myFirTreeCreator =  new FirTreeCreatorV2(context);

	connect(ui->setHubAngles_checkBox, SIGNAL(stateChanged(int)), this, SLOT(setChangesOf_SetHubAnglesCheckbox(int)));
	connect(ui->setDepthCutting_checkBox, SIGNAL(stateChanged(int)), this, SLOT(setChangesOf_SetDepthCuttingCheckBox(int)));
	connect(ui->setFirTree_checkBox, SIGNAL(stateChanged(int)), this, SLOT(setChangesOf_SetFirTreeCheckBox(int)));

	connect(ui->finishButton, SIGNAL(released()), this, SLOT(setChangesOf_FinishButton()));
}


occ_FirTreeInputDialog::~occ_FirTreeInputDialog()
{
	
}

void occ_FirTreeInputDialog::setChangesOf_a1_doubleSpinBox(void)
{
	double value = ui->a1_doubleSpinBox->value();

	myFirTreeCreator->setA1InDegrees(value);
}

void occ_FirTreeInputDialog::setChangesOf_a2_doubleSpinBox(void)
{
	double value = ui->a2_doubleSpinBox->value();

	myFirTreeCreator->setA2InDegrees(value);
}

void occ_FirTreeInputDialog::setChangesOf_a3_doubleSpinBox(void)
{
	double value = ui->a3_doubleSpinBox->value();

	myFirTreeCreator->setA3InDegrees(value);
}

void occ_FirTreeInputDialog::setChangesOf_a4_doubleSpinBox(void)
{
	double value = ui->a4_doubleSpinBox->value();

	myFirTreeCreator->setA4InDegrees(value);
}

void occ_FirTreeInputDialog::setChangesOf_fiTreeBottomFilletRadius_doubleSpinBox(void)
{
	double value = ui->fiTreeBottomFilletRadius_doubleSpinBox->value();
	myFirTreeCreator->setFirTreebottomFilletRadius(value);
}

void occ_FirTreeInputDialog::setChangesOf_fiTreeUpperFilletRadius_doubleSpinBox(void)
{
	double value = ui->fiTreeUpperFilletRadius_doubleSpinBox->value();
	myFirTreeCreator->setFirTreeupperFilletRadius(value);
}

void occ_FirTreeInputDialog::setChangesOf_neckHeight_doubleSpinBox(void)
{
	double value = ui->neckHeight_doubleSpinBox->value();
	myFirTreeCreator->setNeckHeight(value);
}

void occ_FirTreeInputDialog::setChangesOf_neckBottomWidth_doubleSpinBox(void)
{
	double value = ui->neckBottomWidth_doubleSpinBox->value();
	myFirTreeCreator->setBottomNeckWidth(value);
}

void occ_FirTreeInputDialog::setChangesOf_depthFromFrontFace_tableWidget(void)
{

}

void occ_FirTreeInputDialog::depthFromNextToFrontFace_tableWidget(void)
{

}

void occ_FirTreeInputDialog::setChangesOf_lobeProperties_tableWidget(void)
{

}

void occ_FirTreeInputDialog::setChangesOf_SetHubAnglesCheckbox(int state)
{
	if (state)
	{
		ui->a1_doubleSpinBox->setEnabled(true);
		ui->a2_doubleSpinBox->setEnabled(true);

		connect(ui->a1_doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setChangesOf_a1_doubleSpinBox()));
		connect(ui->a2_doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setChangesOf_a2_doubleSpinBox()));
	}
	else 
	{
		ui->a1_doubleSpinBox->setDisabled(true);
		ui->a2_doubleSpinBox->setDisabled(true);

		disconnect(ui->a1_doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setChangesOf_a1_doubleSpinBox()));
		disconnect(ui->a2_doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setChangesOf_a2_doubleSpinBox()));
	}
}

void occ_FirTreeInputDialog::setChangesOf_SetDepthCuttingCheckBox(int state)
{
	if (state)
	{
		ui->depthFromFrontFace_tableWidget->setEnabled(true);
		ui->addDepthFromFront_pushButton->setEnabled(true);
		ui->removeDepthFromFront_pushButton->setEnabled(true);
		ui->depthFromNextToFrontFace_tableWidget->setEnabled(true);
		ui->addDepthFromNextToFront_pushButton->setEnabled(true);
		ui->removeDepthFromNextToFront_pushButton->setEnabled(true);
	}
	else
	{
		ui->depthFromFrontFace_tableWidget->setDisabled(true);
		ui->addDepthFromFront_pushButton->setDisabled(true);
		ui->removeDepthFromFront_pushButton->setDisabled(true);
		ui->depthFromNextToFrontFace_tableWidget->setDisabled(true);
		ui->addDepthFromNextToFront_pushButton->setDisabled(true);
		ui->removeDepthFromNextToFront_pushButton->setDisabled(true);
	}
}

void occ_FirTreeInputDialog::setChangesOf_SetFirTreeCheckBox(int state)
{
	if (state)
	{
		ui->a3_doubleSpinBox->setEnabled(true);
		ui->a4_doubleSpinBox->setEnabled(true);
		ui->fiTreeBottomFilletRadius_doubleSpinBox->setEnabled(true);
		ui->fiTreeUpperFilletRadius_doubleSpinBox->setEnabled(true);
		ui->neckHeight_doubleSpinBox->setEnabled(true);
		ui->neckBottomWidth_doubleSpinBox->setEnabled(true);
		ui->lobeProperties_tableWidget->setEnabled(true);
		ui->addLobe_pushButton->setEnabled(true);
		ui->removeLobe_pushButton->setEnabled(true);

		connect(ui->a3_doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setChangesOf_a3_doubleSpinBox()));
		connect(ui->a4_doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setChangesOf_a4_doubleSpinBox()));
		connect(ui->fiTreeBottomFilletRadius_doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setChangesOf_fiTreeBottomFilletRadius_doubleSpinBox()));
		connect(ui->fiTreeUpperFilletRadius_doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setChangesOf_fiTreeUpperFilletRadius_doubleSpinBox()));
		connect(ui->neckHeight_doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setChangesOf_neckHeight_doubleSpinBox()));
		connect(ui->neckBottomWidth_doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setChangesOf_neckBottomWidth_doubleSpinBox()));
	}
	else
	{
		ui->a3_doubleSpinBox->setDisabled(true);
		ui->a4_doubleSpinBox->setDisabled(true);
		ui->fiTreeBottomFilletRadius_doubleSpinBox->setDisabled(true);
		ui->fiTreeUpperFilletRadius_doubleSpinBox->setDisabled(true);
		ui->neckHeight_doubleSpinBox->setDisabled(true);
		ui->neckBottomWidth_doubleSpinBox->setDisabled(true);
		ui->lobeProperties_tableWidget->setDisabled(true);
		ui->addLobe_pushButton->setDisabled(true);
		ui->removeLobe_pushButton->setDisabled(true);

		disconnect(ui->a3_doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setChangesOf_a3_doubleSpinBox()));
		disconnect(ui->a4_doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setChangesOf_a4_doubleSpinBox()));
		disconnect(ui->fiTreeBottomFilletRadius_doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setChangesOf_fiTreeBottomFilletRadius_doubleSpinBox()));
		disconnect(ui->fiTreeUpperFilletRadius_doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setChangesOf_fiTreeUpperFilletRadius_doubleSpinBox()));
		disconnect(ui->neckHeight_doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setChangesOf_neckHeight_doubleSpinBox()));
		disconnect(ui->neckBottomWidth_doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setChangesOf_neckBottomWidth_doubleSpinBox()));
	}
}

void occ_FirTreeInputDialog::setChangesOf_FinishButton()
{
	myFirTreeCreator->build();
	this->accept();
}
