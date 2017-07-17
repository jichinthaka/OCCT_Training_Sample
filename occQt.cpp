/*
*    Copyright (c) 2014 eryar All Rights Reserved.
*
*           File : occQt.cpp
*         Author : eryar@163.com
*           Date : 2014-07-15 21:00
*        Version : OpenCASCADE6.8.0 & Qt5.4
*
*    Description : Qt main window for OpenCASCADE.
*/

#include "occQt.h"
#include "occView.h"
#include "FirTreeCreator.h"

//#include <qinputdialog.h>

#include <QToolBar>
#include <QTreeView>
#include <QMessageBox>
#include <QDockWidget>

#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <gp_Pln.hxx>

#include <gp_Lin2d.hxx>

#include <Geom_ConicalSurface.hxx>
#include <Geom_ToroidalSurface.hxx>
#include <Geom_CylindricalSurface.hxx>

#include <GCE2d_MakeSegment.hxx>

#include <TopoDS.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TColgp_Array1OfPnt2d.hxx>

#include <BRepLib.hxx>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>

#include <BRepBuilderAPI_Transform.hxx>


#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>

#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>
#include <BRepOffsetAPI_MakeThickSolid.hxx>

#include <BRepOffsetAPI_MakePipe.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>

#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Common.hxx>

#include <BRepTools.hxx>


#include <AIS_TextLabel.hxx>
#include <AIS_InteractiveObject.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <GC_MakeSegment.hxx>
#include <GC_MakeArcOfCircle.hxx>

#include <Standard_Type.hxx>
#include <Geom_Plane.hxx>




occQt::occQt(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    myOccView = new OccView(this);

	setCentralWidget(myOccView);

    createActions();
    createMenus();
    createToolBars();

	isFilletedBox = false;
	isDrilledBox = false;
}

occQt::~occQt()
{

}

void occQt::createActions( void )
{
	importBrepAction = new QAction(tr("Import Brep"), this);
	//importBrepAction->setShortcut(tr("Ctrl+Q"));
	importBrepAction->setStatusTip(tr("Import Brep File"));
	connect(importBrepAction, SIGNAL(triggered()), this, SLOT(importBrep()));

    mExitAction = new QAction(tr("Exit"), this);
    mExitAction->setShortcut(tr("Ctrl+Q"));
    mExitAction->setIcon(QIcon(":/Resources/close.png"));
    mExitAction->setStatusTip(tr("Exit the application"));
    connect(mExitAction, SIGNAL(triggered()), this, SLOT(close()));

    mViewZoomAction = new QAction(tr("Zoom"), this);
    mViewZoomAction->setIcon(QIcon(":/Resources/Zoom.png"));
    mViewZoomAction->setStatusTip(tr("Zoom the view"));
    connect(mViewZoomAction, SIGNAL(triggered()), myOccView, SLOT(zoom()));

    mViewPanAction = new QAction(tr("Pan"), this);
    mViewPanAction->setIcon(QIcon(":/Resources/Pan.png"));
    mViewPanAction->setStatusTip(tr("Pan the view"));
    connect(mViewPanAction, SIGNAL(triggered()), myOccView, SLOT(pan()));

    mViewRotateAction = new QAction(tr("Rotate"), this);
    mViewRotateAction->setIcon(QIcon(":/Resources/Rotate.png"));
    mViewRotateAction->setStatusTip(tr("Rotate the view"));
    connect(mViewRotateAction, SIGNAL(triggered()), myOccView, SLOT(rotate()));

    mViewResetAction = new QAction(tr("Reset"), this);
    mViewResetAction->setIcon(QIcon(":/Resources/Home.png"));
    mViewResetAction->setStatusTip(tr("Reset the view"));
    connect(mViewResetAction, SIGNAL(triggered()), myOccView, SLOT(reset()));

    mViewFitallAction = new QAction(tr("Fit All"), this);
    mViewFitallAction->setIcon(QIcon(":/Resources/FitAll.png"));
    mViewFitallAction->setStatusTip(tr("Fit all "));
    connect(mViewFitallAction, SIGNAL(triggered()), myOccView, SLOT(fitAll()));

	mViewAutoModeAction = new QAction(tr("Auto"), this);
	//mViewFitallAction->setIcon(QIcon(":/Resources/FitAll.png"));
	mViewAutoModeAction->setStatusTip(tr("Auto mode "));
	connect(mViewAutoModeAction, SIGNAL(triggered()), myOccView, SLOT(autoMode()));

	removeAllDisplaiedAction = new QAction(tr("Rewmove All"), this);
	removeAllDisplaiedAction->setStatusTip(tr("Rewmove All Displaied"));
	connect(removeAllDisplaiedAction, SIGNAL(triggered()), this, SLOT(removeDisplaiedAISShape()));

    mMakeBoxAction = new QAction(tr("Box"), this);
    mMakeBoxAction->setIcon(QIcon(":/Resources/box.png"));
    mMakeBoxAction->setStatusTip(tr("Make a box"));
    connect(mMakeBoxAction, SIGNAL(triggered()), this, SLOT(makeBox()));

    mMakeConeAction = new QAction(tr("Cone"), this);
    mMakeConeAction->setIcon(QIcon(":/Resources/cone.png"));
    mMakeConeAction->setStatusTip(tr("Make a cone"));
    connect(mMakeConeAction, SIGNAL(triggered()), this, SLOT(makeCone()));

    mMakeSphereAction = new QAction(tr("Sphere"), this);
    mMakeSphereAction->setStatusTip(tr("Make a sphere"));
    mMakeSphereAction->setIcon(QIcon(":/Resources/sphere.png"));
    connect(mMakeSphereAction, SIGNAL(triggered()), this, SLOT(makeSphere()));

    mMakeCylinderAction = new QAction(tr("Cylinder"), this);
    mMakeCylinderAction->setStatusTip(tr("Make a cylinder"));
    mMakeCylinderAction->setIcon(QIcon(":/Resources/cylinder.png"));
    connect(mMakeCylinderAction, SIGNAL(triggered()), this, SLOT(makeCylinder()));

	camber = new QAction(tr("Dialog Input camber"), this);
	camber->setStatusTip(tr("Input camber"));
	camber->setIcon(QIcon(":/Resources/cylinder.png"));
	connect(camber, SIGNAL(triggered()), this, SLOT(ShowCamberInputDialog()));

	mMakeBottle = new QAction(tr("Bottle"), this);
	mMakeBottle->setStatusTip(tr("Make a bottle"));
	connect(mMakeBottle, SIGNAL(triggered()), this, SLOT(makeBottle()));

	mMakeBoxWithInput = new QAction(tr("Box"), this);
	mMakeBoxWithInput->setStatusTip(tr("Make a Box with inputs"));
	connect(mMakeBoxWithInput, SIGNAL(triggered()), this, SLOT(displayBoxWithInputs()));

	BoxAddFillet = new QAction(tr("Add Fillet"), this);
	//mViewFitallAction->setIcon(QIcon(":/Resources/FitAll.png"));
	BoxAddFillet->setStatusTip(tr("Add Fillet"));
	connect(BoxAddFillet, SIGNAL(triggered()), this, SLOT(addBoxFillet()));

	BoxDrillHole = new QAction(tr("Drill Hole"), this);
	//mViewFitallAction->setIcon(QIcon(":/Resources/FitAll.png"));
	BoxDrillHole->setStatusTip(tr("Drill Hole"));
	connect(BoxDrillHole, SIGNAL(triggered()), this, SLOT(drillBoxHole()));

    mMakeTorusAction = new QAction(tr("Torus"), this);
    mMakeTorusAction->setStatusTip(tr("Make a torus"));
    mMakeTorusAction->setIcon(QIcon(":/Resources/torus.png"));
    connect(mMakeTorusAction, SIGNAL(triggered()), this, SLOT(makeTorus()));

    mFilletAction = new QAction(tr("Fillet"), this);
    mFilletAction->setIcon(QIcon(":/Resources/fillet.png"));
    mFilletAction->setStatusTip(tr("Test Fillet algorithm"));
    connect(mFilletAction, SIGNAL(triggered()), this, SLOT(makeFillet()));

    mChamferAction = new QAction(tr("Chamfer"), this);
    mChamferAction->setIcon(QIcon(":/Resources/chamfer.png"));
    mChamferAction->setStatusTip(tr("Test chamfer algorithm"));
    connect(mChamferAction, SIGNAL(triggered()), this, SLOT(makeChamfer()));

    mExtrudeAction = new QAction(tr("Extrude"), this);
    mExtrudeAction->setIcon(QIcon(":/Resources/extrude.png"));
    mExtrudeAction->setStatusTip(tr("Test extrude algorithm"));
    connect(mExtrudeAction, SIGNAL(triggered()), this, SLOT(makeExtrude()));

    mRevolveAction = new QAction(tr("Revolve"), this);
    mRevolveAction->setIcon(QIcon(":/Resources/revolve.png"));
    mRevolveAction->setStatusTip(tr("Test revol algorithm"));
    connect(mRevolveAction, SIGNAL(triggered()), this, SLOT(makeRevol()));

    mLoftAction = new QAction(tr("Loft"), this);
    mLoftAction->setIcon(QIcon(":/Resources/loft.png"));
    mLoftAction->setStatusTip(tr("Test loft algorithm"));
    connect(mLoftAction, SIGNAL(triggered()), this, SLOT(makeLoft()));

    mCutAction = new QAction(tr("Cut"), this);
    mCutAction->setIcon(QIcon(":/Resources/cut.png"));
    mCutAction->setStatusTip(tr("Boolean operation cut"));
    connect(mCutAction, SIGNAL(triggered()), this, SLOT(testCut()));

    mFuseAction = new QAction(tr("Fuse"), this);
    mFuseAction->setIcon(QIcon(":/Resources/fuse.png"));
    mFuseAction->setStatusTip(tr("Boolean operation fuse"));
    connect(mFuseAction, SIGNAL(triggered()), this, SLOT(testFuse()));

    mCommonAction = new QAction(tr("Common"), this);
    mCommonAction->setIcon(QIcon(":/Resources/common.png"));
    mCommonAction->setStatusTip(tr("Boolean operation common"));
    connect(mCommonAction, SIGNAL(triggered()), this, SLOT(testCommon()));

    myHelixAction = new QAction(tr("Helix"), this);
    myHelixAction->setIcon(QIcon(":/Resources/helix.png"));
    myHelixAction->setStatusTip(tr("Make helix shapes"));
    connect(myHelixAction, SIGNAL(triggered()), this, SLOT(testHelix()));

	firTreeAction = new QAction(tr("Fir Tree"), this);
	firTreeAction->setStatusTip(tr("Set Fir Tree"));
	connect(firTreeAction, SIGNAL(triggered()), this, SLOT(setFirTree3()));

	//setFirTreeAction = new QAction(tr("SetFirTree"), this);
	//setFirTreeAction->setStatusTip(tr("Set Fir Tree using seperate class"));
	//connect(setFirTreeAction, SIGNAL(triggered()), myFirTreeCreator, SLOT(build()));

	setFilletModeAction = new QAction(tr("set Fillet Mode"), this);
	setFilletModeAction->setStatusTip(tr("set Fillet Mode"));
	connect(setFilletModeAction, SIGNAL(triggered()), this, SLOT(setFilletMode()));

	setFilletAction = new QAction(tr("set Fillet"), this);
	setFilletAction->setStatusTip(tr("set Fillet"));
	connect(setFilletAction, SIGNAL(triggered()), this, SLOT(setFillet()));

	checkerAction = new QAction(tr("checker"), this);
	checkerAction->setStatusTip(tr("checker"));
	connect(checkerAction, SIGNAL(triggered()), this, SLOT(Checker()));
	

    mAboutAction = new QAction(tr("About"), this);
    mAboutAction->setStatusTip(tr("About the application"));
    mAboutAction->setIcon(QIcon(":/Resources/lamp.png"));
    connect(mAboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void occQt::createMenus( void )
{
    mFileMenu = menuBar()->addMenu(tr("&File"));
	QMenu* ImportMenu = mFileMenu->addMenu("Import");
	QMenu* ExportMenu = mFileMenu->addMenu("Export");
	mFileMenu->addSeparator();
    mFileMenu->addAction(mExitAction);

	ImportMenu->addAction(importBrepAction);

    mViewMenu = menuBar()->addMenu(tr("&View"));
    mViewMenu->addAction(mViewZoomAction);
    mViewMenu->addAction(mViewPanAction);
    mViewMenu->addAction(mViewRotateAction);
    mViewMenu->addSeparator();
    mViewMenu->addAction(mViewResetAction);
    mViewMenu->addAction(mViewFitallAction);
	mViewMenu->addAction(mViewAutoModeAction);
	//mViewMenu->addAction(removeDisplaiedAISShape);

    mPrimitiveMenu = menuBar()->addMenu(tr("&Primitive"));
    mPrimitiveMenu->addAction(mMakeBoxAction);
    mPrimitiveMenu->addAction(mMakeConeAction);
    mPrimitiveMenu->addAction(mMakeSphereAction);
    mPrimitiveMenu->addAction(mMakeCylinderAction);
    mPrimitiveMenu->addAction(mMakeTorusAction);
	mPrimitiveMenu->addAction(camber);
	mPrimitiveMenu->addAction(mMakeBottle);
	mPrimitiveMenu->addAction(mMakeBoxWithInput);



    mModelingMenu = menuBar()->addMenu(tr("&Modeling"));
    mModelingMenu->addAction(mFilletAction);
    mModelingMenu->addAction(mChamferAction);
    mModelingMenu->addAction(mExtrudeAction);
    mModelingMenu->addAction(mRevolveAction);
    mModelingMenu->addAction(mLoftAction);
    mModelingMenu->addSeparator();
    mModelingMenu->addAction(mCutAction);
    mModelingMenu->addAction(mFuseAction);
    mModelingMenu->addAction(mCommonAction);
    mModelingMenu->addSeparator();
    mModelingMenu->addAction(myHelixAction);

    mHelpMenu = menuBar()->addMenu(tr("&Help"));
    mHelpMenu->addAction(mAboutAction);
}

void occQt::createToolBars( void )
{
    mNavigateToolBar = addToolBar(tr("&Navigate"));
    mNavigateToolBar->addAction(mViewZoomAction);
    mNavigateToolBar->addAction(mViewPanAction);
    mNavigateToolBar->addAction(mViewRotateAction);

    mViewToolBar = addToolBar(tr("&View"));
    mViewToolBar->addAction(mViewResetAction);
    mViewToolBar->addAction(mViewFitallAction);
	mViewToolBar->addAction(mViewAutoModeAction);
	mViewToolBar->addAction(removeAllDisplaiedAction);

    mPrimitiveToolBar = addToolBar(tr("&Primitive"));
    mPrimitiveToolBar->addAction(mMakeBoxAction);
    mPrimitiveToolBar->addAction(mMakeConeAction);
    mPrimitiveToolBar->addAction(mMakeSphereAction);
    mPrimitiveToolBar->addAction(mMakeCylinderAction);
    mPrimitiveToolBar->addAction(mMakeTorusAction);
	mPrimitiveToolBar->addAction(camber);
	mPrimitiveToolBar->addAction(mMakeBottle);
	mPrimitiveToolBar->addAction(mMakeBoxWithInput);
	mPrimitiveToolBar->addAction(checkerAction);

    mModelingToolBar = addToolBar(tr("&Modeling"));
    mModelingToolBar->addAction(mFilletAction);
    mModelingToolBar->addAction(mChamferAction);
    mModelingToolBar->addAction(mExtrudeAction);
    mModelingToolBar->addAction(mRevolveAction);
    mModelingToolBar->addAction(mLoftAction);
    mModelingToolBar->addSeparator();
    mModelingToolBar->addAction(mCutAction);
    mModelingToolBar->addAction(mFuseAction);
    mModelingToolBar->addAction(mCommonAction);
    mModelingToolBar->addSeparator();
    mModelingToolBar->addAction(myHelixAction);

	bladeModifyToolBar = addToolBar(tr("&Blade_Modify"));
	bladeModifyToolBar->addAction(firTreeAction);
	bladeModifyToolBar->addAction(setFilletModeAction);
	bladeModifyToolBar->addAction(setFilletAction);

    mHelpToolBar = addToolBar(tr("Help"));
    mHelpToolBar->addAction(mAboutAction);
}

#include <TopTools_HSequenceOfShape.hxx>
#include <QFileDialog>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
void occQt::importBrep(void)
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "C:/", tr("BREP Files (*.brep)"));
	std::string nameString= fileName.toStdString();
	Standard_CString pathName = nameString.c_str();

	TopoDS_Shape shape;

	std::filebuf aFileBuf;
	std::istream aStream(&aFileBuf);
	if (!aFileBuf.open(pathName, ios::in))
	{
		return;
	}

	BRep_Builder aBuilder;
	BRepTools::Read(shape, pathName, aBuilder);

	displayASape(shape);
}

void occQt::about()
{
    QMessageBox::about(this, tr("About occQt"),
        tr("<h2>occQt 2.0</h2>"
        "<p>Copyright &copy; 2014 eryar@163.com"
        "<p>occQt is a demo applicaton about Qt and OpenCASCADE."));
}

void occQt::makeBox()
{
    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(3.0, 4.0, 5.0).Shape();

	BRepTools::Write(aTopoBox, "C:/QTSample/Box.brep");

    Handle(AIS_Shape) anAisBox = new AIS_Shape(aTopoBox);

    anAisBox->SetColor(Quantity_NOC_AZURE);

    myOccView->getContext()->Display(anAisBox);
}

void occQt::makeCone()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 10.0, 0.0));

    TopoDS_Shape aTopoReducer = BRepPrimAPI_MakeCone(anAxis, 3.0, 1.5, 5.0).Shape();
    Handle(AIS_Shape) anAisReducer = new AIS_Shape(aTopoReducer);

    anAisReducer->SetColor(Quantity_NOC_BISQUE);

    anAxis.SetLocation(gp_Pnt(8.0, 10.0, 0.0));
    TopoDS_Shape aTopoCone = BRepPrimAPI_MakeCone(anAxis, 3.0, 0.0, 5.0).Shape();
    Handle(AIS_Shape) anAisCone = new AIS_Shape(aTopoCone);

    anAisCone->SetColor(Quantity_NOC_CHOCOLATE);

    myOccView->getContext()->Display(anAisReducer);
    myOccView->getContext()->Display(anAisCone);
}

void occQt::makeSphere()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 20.0, 0.0));

    TopoDS_Shape aTopoSphere = BRepPrimAPI_MakeSphere(anAxis, 3.0).Shape();
    Handle(AIS_Shape) anAisSphere = new AIS_Shape(aTopoSphere);

    anAisSphere->SetColor(Quantity_NOC_BLUE1);

    myOccView->getContext()->Display(anAisSphere);
}

void occQt::makeCylinder()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 30.0, 0.0));

    TopoDS_Shape aTopoCylinder = BRepPrimAPI_MakeCylinder(anAxis, 3.0, 5.0).Shape();
    Handle(AIS_Shape) anAisCylinder = new AIS_Shape(aTopoCylinder);

    anAisCylinder->SetColor(Quantity_NOC_RED);

    anAxis.SetLocation(gp_Pnt(8.0, 30.0, 0.0));
    TopoDS_Shape aTopoPie = BRepPrimAPI_MakeCylinder(anAxis, 3.0, 5.0, M_PI_2 * 3.0).Shape();
    Handle(AIS_Shape) anAisPie = new AIS_Shape(aTopoPie);

    anAisPie->SetColor(Quantity_NOC_TAN);

    myOccView->getContext()->Display(anAisCylinder);
    myOccView->getContext()->Display(anAisPie);
}

void occQt::makeTorus()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 40.0, 0.0));

    TopoDS_Shape aTopoTorus = BRepPrimAPI_MakeTorus(anAxis, 3.0, 1.0).Shape();
    Handle(AIS_Shape) anAisTorus = new AIS_Shape(aTopoTorus);

    anAisTorus->SetColor(Quantity_NOC_YELLOW);

    anAxis.SetLocation(gp_Pnt(8.0, 40.0, 0.0));
    TopoDS_Shape aTopoElbow = BRepPrimAPI_MakeTorus(anAxis, 3.0, 1.0, M_PI_2).Shape();
    Handle(AIS_Shape) anAisElbow = new AIS_Shape(aTopoElbow);

    anAisElbow->SetColor(Quantity_NOC_THISTLE);

    myOccView->getContext()->Display(anAisTorus);
    myOccView->getContext()->Display(anAisElbow);
}

void occQt::makeFillet()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 50.0, 0.0));

    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(anAxis, 3.0, 4.0, 5.0).Shape();
    BRepFilletAPI_MakeFillet MF(aTopoBox);

    // Add all the edges to fillet.
    for (TopExp_Explorer ex(aTopoBox, TopAbs_EDGE); ex.More(); ex.Next())
    {
        MF.Add(1.0, TopoDS::Edge(ex.Current()));
    }

    Handle(AIS_Shape) anAisShape = new AIS_Shape(MF.Shape());
    anAisShape->SetColor(Quantity_NOC_VIOLET);

    myOccView->getContext()->Display(anAisShape);
}

void occQt::makeChamfer()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(8.0, 50.0, 0.0));

    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(anAxis, 3.0, 4.0, 5.0).Shape();
    BRepFilletAPI_MakeChamfer MC(aTopoBox);
    TopTools_IndexedDataMapOfShapeListOfShape aEdgeFaceMap;

    TopExp::MapShapesAndAncestors(aTopoBox, TopAbs_EDGE, TopAbs_FACE, aEdgeFaceMap);

    for (Standard_Integer i = 1; i <= aEdgeFaceMap.Extent(); ++i)
    {
        TopoDS_Edge anEdge = TopoDS::Edge(aEdgeFaceMap.FindKey(i));
        TopoDS_Face aFace = TopoDS::Face(aEdgeFaceMap.FindFromIndex(i).First());

        MC.Add(0.6, 0.6, anEdge, aFace);
    }

    Handle(AIS_Shape) anAisShape = new AIS_Shape(MC.Shape());
    anAisShape->SetColor(Quantity_NOC_TOMATO);

    myOccView->getContext()->Display(anAisShape);
}

void occQt::makeExtrude()
{
    // prism a vertex result is an edge.
    TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex(gp_Pnt(0.0, 60.0, 0.0));
    TopoDS_Shape aPrismVertex = BRepPrimAPI_MakePrism(aVertex, gp_Vec(0.0, 0.0, 5.0));
    Handle(AIS_Shape) anAisPrismVertex = new AIS_Shape(aPrismVertex);

    // prism an edge result is a face.
    TopoDS_Edge anEdge = BRepBuilderAPI_MakeEdge(gp_Pnt(5.0, 60.0, 0.0), gp_Pnt(10.0, 60.0, 0.0));
    TopoDS_Shape aPrismEdge = BRepPrimAPI_MakePrism(anEdge, gp_Vec(0.0, 0.0, 5.0));
    Handle(AIS_Shape) anAisPrismEdge = new AIS_Shape(aPrismEdge);

    // prism a wire result is a shell.
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(16.0, 60.0, 0.0));

    TopoDS_Edge aCircleEdge = BRepBuilderAPI_MakeEdge(gp_Circ(anAxis, 3.0));
    TopoDS_Wire aCircleWire = BRepBuilderAPI_MakeWire(aCircleEdge);
    TopoDS_Shape aPrismCircle = BRepPrimAPI_MakePrism(aCircleWire, gp_Vec(0.0, 0.0, 5.0));
    Handle(AIS_Shape) anAisPrismCircle = new AIS_Shape(aPrismCircle);

    // prism a face or a shell result is a solid.
    anAxis.SetLocation(gp_Pnt(24.0, 60.0, 0.0));
    TopoDS_Edge aEllipseEdge = BRepBuilderAPI_MakeEdge(gp_Elips(anAxis, 3.0, 2.0));
    TopoDS_Wire aEllipseWire = BRepBuilderAPI_MakeWire(aEllipseEdge);
    TopoDS_Face aEllipseFace = BRepBuilderAPI_MakeFace(gp_Pln(gp::XOY()), aEllipseWire);
    TopoDS_Shape aPrismEllipse = BRepPrimAPI_MakePrism(aEllipseFace, gp_Vec(0.0, 0.0, 5.0));
    Handle(AIS_Shape) anAisPrismEllipse = new AIS_Shape(aPrismEllipse);

    anAisPrismVertex->SetColor(Quantity_NOC_PAPAYAWHIP);
    anAisPrismEdge->SetColor(Quantity_NOC_PEACHPUFF);
    anAisPrismCircle->SetColor(Quantity_NOC_PERU);
    anAisPrismEllipse->SetColor(Quantity_NOC_PINK);

    myOccView->getContext()->Display(anAisPrismVertex);
    myOccView->getContext()->Display(anAisPrismEdge);
    myOccView->getContext()->Display(anAisPrismCircle);
    myOccView->getContext()->Display(anAisPrismEllipse);
}

void occQt::makeRevol()
{
    gp_Ax1 anAxis;

    // revol a vertex result is an edge.
    anAxis.SetLocation(gp_Pnt(0.0, 70.0, 0.0));
    TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex(gp_Pnt(2.0, 70.0, 0.0));
    TopoDS_Shape aRevolVertex = BRepPrimAPI_MakeRevol(aVertex, anAxis);
    Handle(AIS_Shape) anAisRevolVertex = new AIS_Shape(aRevolVertex);

    // revol an edge result is a face.
    anAxis.SetLocation(gp_Pnt(8.0, 70.0, 0.0));
    TopoDS_Edge anEdge = BRepBuilderAPI_MakeEdge(gp_Pnt(6.0, 70.0, 0.0), gp_Pnt(6.0, 70.0, 5.0));
    TopoDS_Shape aRevolEdge = BRepPrimAPI_MakeRevol(anEdge, anAxis);
    Handle(AIS_Shape) anAisRevolEdge = new AIS_Shape(aRevolEdge);

    // revol a wire result is a shell.
    anAxis.SetLocation(gp_Pnt(20.0, 70.0, 0.0));
    anAxis.SetDirection(gp::DY());

    TopoDS_Edge aCircleEdge = BRepBuilderAPI_MakeEdge(gp_Circ(gp_Ax2(gp_Pnt(15.0, 70.0, 0.0), gp::DZ()), 1.5));
    TopoDS_Wire aCircleWire = BRepBuilderAPI_MakeWire(aCircleEdge);
    TopoDS_Shape aRevolCircle = BRepPrimAPI_MakeRevol(aCircleWire, anAxis, M_PI_2);
    Handle(AIS_Shape) anAisRevolCircle = new AIS_Shape(aRevolCircle);

    // revol a face result is a solid.
    anAxis.SetLocation(gp_Pnt(30.0, 70.0, 0.0));
    anAxis.SetDirection(gp::DY());

    TopoDS_Edge aEllipseEdge = BRepBuilderAPI_MakeEdge(gp_Elips(gp_Ax2(gp_Pnt(25.0, 70.0, 0.0), gp::DZ()), 3.0, 2.0));
    TopoDS_Wire aEllipseWire = BRepBuilderAPI_MakeWire(aEllipseEdge);
    TopoDS_Face aEllipseFace = BRepBuilderAPI_MakeFace(gp_Pln(gp::XOY()), aEllipseWire);
    TopoDS_Shape aRevolEllipse = BRepPrimAPI_MakeRevol(aEllipseFace, anAxis, M_PI_4);
    Handle(AIS_Shape) anAisRevolEllipse = new AIS_Shape(aRevolEllipse);

    anAisRevolVertex->SetColor(Quantity_NOC_LIMEGREEN);
    anAisRevolEdge->SetColor(Quantity_NOC_LINEN);
    anAisRevolCircle->SetColor(Quantity_NOC_MAGENTA1);
    anAisRevolEllipse->SetColor(Quantity_NOC_MAROON);

    myOccView->getContext()->Display(anAisRevolVertex);
    myOccView->getContext()->Display(anAisRevolEdge);
    myOccView->getContext()->Display(anAisRevolCircle);
    myOccView->getContext()->Display(anAisRevolEllipse);
}

void occQt::makeLoft()
{
    // bottom wire.
    TopoDS_Edge aCircleEdge = BRepBuilderAPI_MakeEdge(gp_Circ(gp_Ax2(gp_Pnt(0.0, 80.0, 0.0), gp::DZ()), 1.5));
    TopoDS_Wire aCircleWire = BRepBuilderAPI_MakeWire(aCircleEdge);

    // top wire.
    BRepBuilderAPI_MakePolygon aPolygon;
    aPolygon.Add(gp_Pnt(-3.0, 77.0, 6.0));
    aPolygon.Add(gp_Pnt(3.0, 77.0, 6.0));
    aPolygon.Add(gp_Pnt(3.0, 83.0, 6.0));
    aPolygon.Add(gp_Pnt(-3.0, 83.0, 6.0));
    aPolygon.Close();

    BRepOffsetAPI_ThruSections aShellGenerator;
    BRepOffsetAPI_ThruSections aSolidGenerator(true);

    aShellGenerator.AddWire(aCircleWire);
    aShellGenerator.AddWire(aPolygon.Wire());

    aSolidGenerator.AddWire(aCircleWire);
    aSolidGenerator.AddWire(aPolygon.Wire());

    // translate the solid.
    gp_Trsf aTrsf;
    aTrsf.SetTranslation(gp_Vec(18.0, 0.0, 0.0));
    BRepBuilderAPI_Transform aTransform(aSolidGenerator.Shape(), aTrsf);

    Handle(AIS_Shape) anAisShell = new AIS_Shape(aShellGenerator.Shape());
    Handle(AIS_Shape) anAisSolid = new AIS_Shape(aTransform.Shape());

    anAisShell->SetColor(Quantity_NOC_OLIVEDRAB);
    anAisSolid->SetColor(Quantity_NOC_PEACHPUFF);

    myOccView->getContext()->Display(anAisShell);
    myOccView->getContext()->Display(anAisSolid);
}

void occQt::testCut()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 90.0, 0.0));

    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(anAxis, 3.0, 4.0, 5.0).Shape();
    TopoDS_Shape aTopoSphere = BRepPrimAPI_MakeSphere(anAxis, 2.5).Shape();
    TopoDS_Shape aCuttedShape1 = BRepAlgoAPI_Cut(aTopoBox, aTopoSphere);
    TopoDS_Shape aCuttedShape2 = BRepAlgoAPI_Cut(aTopoSphere, aTopoBox);

    gp_Trsf aTrsf;
    aTrsf.SetTranslation(gp_Vec(8.0, 0.0, 0.0));
    BRepBuilderAPI_Transform aTransform1(aCuttedShape1, aTrsf);

    aTrsf.SetTranslation(gp_Vec(16.0, 0.0, 0.0));
    BRepBuilderAPI_Transform aTransform2(aCuttedShape2, aTrsf);

    Handle(AIS_Shape) anAisBox = new AIS_Shape(aTopoBox);
    Handle(AIS_Shape) anAisSphere = new AIS_Shape(aTopoSphere);
    Handle(AIS_Shape) anAisCuttedShape1 = new AIS_Shape(aTransform1.Shape());
    Handle(AIS_Shape) anAisCuttedShape2 = new AIS_Shape(aTransform2.Shape());

    anAisBox->SetColor(Quantity_NOC_SPRINGGREEN);
    anAisSphere->SetColor(Quantity_NOC_STEELBLUE);
    anAisCuttedShape1->SetColor(Quantity_NOC_TAN);
    anAisCuttedShape2->SetColor(Quantity_NOC_SALMON);

    myOccView->getContext()->Display(anAisBox);
    myOccView->getContext()->Display(anAisSphere);
    myOccView->getContext()->Display(anAisCuttedShape1);
    myOccView->getContext()->Display(anAisCuttedShape2);
}

void occQt::testFuse()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 100.0, 0.0));

    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(anAxis, 3.0, 4.0, 5.0).Shape();
    TopoDS_Shape aTopoSphere = BRepPrimAPI_MakeSphere(anAxis, 2.5).Shape();
    TopoDS_Shape aFusedShape = BRepAlgoAPI_Fuse(aTopoBox, aTopoSphere);

    gp_Trsf aTrsf;
    aTrsf.SetTranslation(gp_Vec(8.0, 0.0, 0.0));
    BRepBuilderAPI_Transform aTransform(aFusedShape, aTrsf);

    Handle(AIS_Shape) anAisBox = new AIS_Shape(aTopoBox);
    Handle(AIS_Shape) anAisSphere = new AIS_Shape(aTopoSphere);
    Handle(AIS_Shape) anAisFusedShape = new AIS_Shape(aTransform.Shape());

    anAisBox->SetColor(Quantity_NOC_SPRINGGREEN);
    anAisSphere->SetColor(Quantity_NOC_STEELBLUE);
    anAisFusedShape->SetColor(Quantity_NOC_ROSYBROWN);

    myOccView->getContext()->Display(anAisBox);
    myOccView->getContext()->Display(anAisSphere);
    myOccView->getContext()->Display(anAisFusedShape);
}

void occQt::testCommon()
{
    gp_Ax2 anAxis;
    anAxis.SetLocation(gp_Pnt(0.0, 110.0, 0.0));

    TopoDS_Shape aTopoBox = BRepPrimAPI_MakeBox(anAxis, 3.0, 4.0, 5.0).Shape();
    TopoDS_Shape aTopoSphere = BRepPrimAPI_MakeSphere(anAxis, 2.5).Shape();
    TopoDS_Shape aCommonShape = BRepAlgoAPI_Common(aTopoBox, aTopoSphere);

    gp_Trsf aTrsf;
    aTrsf.SetTranslation(gp_Vec(8.0, 0.0, 0.0));
    BRepBuilderAPI_Transform aTransform(aCommonShape, aTrsf);

    Handle(AIS_Shape) anAisBox = new AIS_Shape(aTopoBox);
    Handle(AIS_Shape) anAisSphere = new AIS_Shape(aTopoSphere);
    Handle(AIS_Shape) anAisCommonShape = new AIS_Shape(aTransform.Shape());

    anAisBox->SetColor(Quantity_NOC_SPRINGGREEN);
    anAisSphere->SetColor(Quantity_NOC_STEELBLUE);
    anAisCommonShape->SetColor(Quantity_NOC_ROYALBLUE);

    myOccView->getContext()->Display(anAisBox);
    myOccView->getContext()->Display(anAisSphere);
    myOccView->getContext()->Display(anAisCommonShape);
}

void occQt::testHelix()
{
    makeCylindricalHelix();

    makeConicalHelix();

    makeToroidalHelix();
}

void occQt::makeCylindricalHelix()
{
    Standard_Real aRadius = 3.0;
    Standard_Real aPitch = 1.0;

    // the pcurve is a 2d line in the parametric space.
    gp_Lin2d aLine2d(gp_Pnt2d(0.0, 0.0), gp_Dir2d(aRadius, aPitch));

    Handle(Geom2d_TrimmedCurve) aSegment = GCE2d_MakeSegment(aLine2d, 0.0, M_PI * 2.0).Value();

    Handle(Geom_CylindricalSurface) aCylinder = new Geom_CylindricalSurface(gp::XOY(), aRadius);

    TopoDS_Edge aHelixEdge = BRepBuilderAPI_MakeEdge(aSegment, aCylinder, 0.0, 6.0 * M_PI).Edge();

    gp_Trsf aTrsf;
    aTrsf.SetTranslation(gp_Vec(0.0, 120.0, 0.0));
    BRepBuilderAPI_Transform aTransform(aHelixEdge, aTrsf);

    Handle(AIS_Shape) anAisHelixCurve = new AIS_Shape(aTransform.Shape());

    myOccView->getContext()->Display(anAisHelixCurve);

    // sweep a circle profile along the helix curve.
    // there is no curve3d in the pcurve edge, so approx one.
    BRepLib::BuildCurve3d(aHelixEdge);

    gp_Ax2 anAxis;
    anAxis.SetDirection(gp_Dir(0.0, 4.0, 1.0));
    anAxis.SetLocation(gp_Pnt(aRadius, 0.0, 0.0));

    gp_Circ aProfileCircle(anAxis, 0.3);

    TopoDS_Edge aProfileEdge = BRepBuilderAPI_MakeEdge(aProfileCircle).Edge();
    TopoDS_Wire aProfileWire = BRepBuilderAPI_MakeWire(aProfileEdge).Wire();
    TopoDS_Face aProfileFace = BRepBuilderAPI_MakeFace(aProfileWire).Face();

    TopoDS_Wire aHelixWire = BRepBuilderAPI_MakeWire(aHelixEdge).Wire();

    BRepOffsetAPI_MakePipe aPipeMaker(aHelixWire, aProfileFace);

    if (aPipeMaker.IsDone())
    {
        aTrsf.SetTranslation(gp_Vec(8.0, 120.0, 0.0));
        BRepBuilderAPI_Transform aPipeTransform(aPipeMaker.Shape(), aTrsf);

        Handle(AIS_Shape) anAisPipe = new AIS_Shape(aPipeTransform.Shape());
        anAisPipe->SetColor(Quantity_NOC_CORAL);
        myOccView->getContext()->Display(anAisPipe);
    }
}

/**
 * make conical helix, it is the same as the cylindrical helix,
 * the only different is the surface.
 */
void occQt::makeConicalHelix()
{
    Standard_Real aRadius = 3.0;
    Standard_Real aPitch = 1.0;

    // the pcurve is a 2d line in the parametric space.
    gp_Lin2d aLine2d(gp_Pnt2d(0.0, 0.0), gp_Dir2d(aRadius, aPitch));

    Handle(Geom2d_TrimmedCurve) aSegment = GCE2d_MakeSegment(aLine2d, 0.0, M_PI * 2.0).Value();

    Handle(Geom_ConicalSurface) aCylinder = new Geom_ConicalSurface(gp::XOY(), M_PI / 6.0, aRadius);

    TopoDS_Edge aHelixEdge = BRepBuilderAPI_MakeEdge(aSegment, aCylinder, 0.0, 6.0 * M_PI).Edge();

    gp_Trsf aTrsf;
    aTrsf.SetTranslation(gp_Vec(18.0, 120.0, 0.0));
    BRepBuilderAPI_Transform aTransform(aHelixEdge, aTrsf);

    Handle(AIS_Shape) anAisHelixCurve = new AIS_Shape(aTransform.Shape());

    myOccView->getContext()->Display(anAisHelixCurve);

    // sweep a circle profile along the helix curve.
    // there is no curve3d in the pcurve edge, so approx one.
    BRepLib::BuildCurve3d(aHelixEdge);

    gp_Ax2 anAxis;
    anAxis.SetDirection(gp_Dir(0.0, 4.0, 1.0));
    anAxis.SetLocation(gp_Pnt(aRadius, 0.0, 0.0));

    gp_Circ aProfileCircle(anAxis, 0.3);

    TopoDS_Edge aProfileEdge = BRepBuilderAPI_MakeEdge(aProfileCircle).Edge();
    TopoDS_Wire aProfileWire = BRepBuilderAPI_MakeWire(aProfileEdge).Wire();
    TopoDS_Face aProfileFace = BRepBuilderAPI_MakeFace(aProfileWire).Face();

    TopoDS_Wire aHelixWire = BRepBuilderAPI_MakeWire(aHelixEdge).Wire();

    BRepOffsetAPI_MakePipe aPipeMaker(aHelixWire, aProfileFace);

    if (aPipeMaker.IsDone())
    {
        aTrsf.SetTranslation(gp_Vec(28.0, 120.0, 0.0));
        BRepBuilderAPI_Transform aPipeTransform(aPipeMaker.Shape(), aTrsf);

        Handle(AIS_Shape) anAisPipe = new AIS_Shape(aPipeTransform.Shape());
        anAisPipe->SetColor(Quantity_NOC_DARKGOLDENROD);
        myOccView->getContext()->Display(anAisPipe);
    }
}

void occQt::makeToroidalHelix()
{
    Standard_Real aRadius = 1.0;
    Standard_Real aSlope = 0.05;

    // the pcurve is a 2d line in the parametric space.
    gp_Lin2d aLine2d(gp_Pnt2d(0.0, 0.0), gp_Dir2d(aSlope, 1.0));

    Handle(Geom2d_TrimmedCurve) aSegment = GCE2d_MakeSegment(aLine2d, 0.0, M_PI * 2.0).Value();

    Handle(Geom_ToroidalSurface) aCylinder = new Geom_ToroidalSurface(gp::XOY(), aRadius * 5.0, aRadius);

    TopoDS_Edge aHelixEdge = BRepBuilderAPI_MakeEdge(aSegment, aCylinder, 0.0, 2.0 * M_PI / aSlope).Edge();

    gp_Trsf aTrsf;
    aTrsf.SetTranslation(gp_Vec(45.0, 120.0, 0.0));
    BRepBuilderAPI_Transform aTransform(aHelixEdge, aTrsf);

    Handle(AIS_Shape) anAisHelixCurve = new AIS_Shape(aTransform.Shape());

    myOccView->getContext()->Display(anAisHelixCurve);

    // sweep a circle profile along the helix curve.
    // there is no curve3d in the pcurve edge, so approx one.
    BRepLib::BuildCurve3d(aHelixEdge);

    gp_Ax2 anAxis;
    anAxis.SetDirection(gp_Dir(0.0, 0.0, 1.0));
    anAxis.SetLocation(gp_Pnt(aRadius * 6.0, 0.0, 0.0));

    gp_Circ aProfileCircle(anAxis, 0.3);

    TopoDS_Edge aProfileEdge = BRepBuilderAPI_MakeEdge(aProfileCircle).Edge();
    TopoDS_Wire aProfileWire = BRepBuilderAPI_MakeWire(aProfileEdge).Wire();
    TopoDS_Face aProfileFace = BRepBuilderAPI_MakeFace(aProfileWire).Face();

    TopoDS_Wire aHelixWire = BRepBuilderAPI_MakeWire(aHelixEdge).Wire();

    BRepOffsetAPI_MakePipe aPipeMaker(aHelixWire, aProfileFace);

    if (aPipeMaker.IsDone())
    {
        aTrsf.SetTranslation(gp_Vec(60.0, 120.0, 0.0));
        BRepBuilderAPI_Transform aPipeTransform(aPipeMaker.Shape(), aTrsf);

        Handle(AIS_Shape) anAisPipe = new AIS_Shape(aPipeTransform.Shape());
        anAisPipe->SetColor(Quantity_NOC_CORNSILK1);
        myOccView->getContext()->Display(anAisPipe);
    }
}

#include <Geom2d_Ellipse.hxx>
void occQt::makeBottle()
{
	Standard_Real BottleHight = 70;
	Standard_Real BottleWidth = 50;
	Standard_Real BottleThickness = 30;

	Standard_Real BottleNeckHight = BottleHight / 10;
	Standard_Real BottleNeckRadius = BottleThickness / 4;

	gp_Pnt point1(-BottleWidth/2, 0, 0);
	gp_Pnt point2(-BottleWidth / 2, -BottleThickness / 4, 0);
	gp_Pnt point3(0, -BottleThickness/2,0);
	gp_Pnt point4(BottleWidth / 2, -BottleThickness / 4, 0);
	gp_Pnt point5(BottleWidth / 2, 0, 0);

	TopoDS_Shape topoDSVertexShape1 = BRepBuilderAPI_MakeVertex(point1);
	displayASape(topoDSVertexShape1);
	makeTextLabel("Vertex 1", point1, -1, 1, 0);
	TopoDS_Shape topoDSVertexShape2 = BRepBuilderAPI_MakeVertex(point2);
	displayASape(topoDSVertexShape2);
	makeTextLabel("Vertex 2", point2, -1, 1, 0);
	TopoDS_Shape topoDSVertexShape3 = BRepBuilderAPI_MakeVertex(point3);
	displayASape(topoDSVertexShape3);
	makeTextLabel("Vertex 3", point3, -1, 1, 0);
	TopoDS_Shape topoDSVertexShape4 = BRepBuilderAPI_MakeVertex(point4);
	displayASape(topoDSVertexShape4);
	makeTextLabel("Vertex 4", point4, -1, 1, 0);
	TopoDS_Shape topoDSVertexShape5 = BRepBuilderAPI_MakeVertex(point5);
	displayASape(topoDSVertexShape5);
	makeTextLabel("Vertex 5", point5, -1, 1, 0);

	Sleep(1000);

	Handle(Geom_TrimmedCurve) line1 = GC_MakeSegment(point1, point2);
	Handle(Geom_TrimmedCurve) line2 = GC_MakeSegment(point4, point5);
	Handle(Geom_TrimmedCurve) arc = GC_MakeArcOfCircle(point2, point3, point4);

	removeDisplaiedAISLables();

	TopoDS_Edge topoDSEdge1 = BRepBuilderAPI_MakeEdge(line1);
	displayASape(topoDSEdge1);
	makeTextLabel("Edge 1", point2, -1, 4, 0);
	TopoDS_Edge topoDSEdge2 = BRepBuilderAPI_MakeEdge(line2);
	displayASape(topoDSEdge2);
	makeTextLabel("Edge 2", point4, -1, 4, 0);
	TopoDS_Edge topoDSEdge3 = BRepBuilderAPI_MakeEdge(arc);
	displayASape(topoDSEdge3);
	makeTextLabel("Edge 3", point4, -25, -10, 0);

	TopoDS_Wire topoDSWire = BRepBuilderAPI_MakeWire(topoDSEdge1,  topoDSEdge3, topoDSEdge2);

	removeDisplaiedAISShape();
	removeDisplaiedAISLables();

	makeTextLabel("Wire", point3, -25, -10, 0);
	displayASape(topoDSWire);
	Sleep(1000);

	gp_Pnt center(0, 0, 0);
	gp_Dir xDirection(1, 0, 0);
	gp_Ax1 xAxis(center, xDirection);

	gp_Trsf transformLine;
	transformLine.SetMirror(xAxis);

	BRepBuilderAPI_Transform transformedWire(topoDSWire,transformLine);
	TopoDS_Shape transformedWireShape = transformedWire.Shape();

	
	removeDisplaiedAISShape();
	displayASape(transformedWireShape);
	Sleep(1000);

	TopoDS_Wire topoDSMirroredWire = TopoDS::Wire(transformedWireShape);

	BRepBuilderAPI_MakeWire wireMaker;
	wireMaker.Add(topoDSWire);
	wireMaker.Add(topoDSMirroredWire);
	TopoDS_Wire combinedWire = wireMaker.Wire();

	displayASape(combinedWire);
	Sleep(1000);

	TopoDS_Face bottomFace = BRepBuilderAPI_MakeFace(combinedWire);

	removeDisplaiedAISLables();
	removeDisplaiedAISShape();

	makeTextLabel("Wire", point3, -25, -10, 0);
	displayASape(bottomFace);
	Sleep(1000);
	
	gp_Vec bottleHightVector(0, 0, BottleHight);

	TopoDS_Shape bottleBody = BRepPrimAPI_MakePrism(bottomFace, bottleHightVector);

	removeDisplaiedAISLables();
	removeDisplaiedAISShape();
	displayASape(bottleBody);
	Sleep(1000);

	BRepFilletAPI_MakeFillet filletMaker(bottleBody);

	TopExp_Explorer edgeExplorer(bottleBody, TopAbs_EDGE);

	while (edgeExplorer.More())
	{
		TopoDS_Edge edge = TopoDS::Edge(edgeExplorer.Current());
		filletMaker.Add(BottleThickness / 12, edge);
		edgeExplorer.Next();
	}

	bottleBody = filletMaker.Shape();

	removeDisplaiedAISShape();
	displayASape(bottleBody);
	Sleep(1000);

	gp_Pnt neckStartPoint(0, 0, BottleHight);
	//gp_Dir neckAxisDirection(0, 0, 1);
	gp_Dir neckAxisDirection = gp::DZ();
	gp_Ax2 neckAxis(neckStartPoint, neckAxisDirection);

	TopoDS_Shape neckCylinder = BRepPrimAPI_MakeCylinder(neckAxis, BottleNeckRadius, BottleNeckHight).Shape();

	//BRepTools::Write(neckCylinder, "C:/Users/DELL/Desktop/neck.brep");
	//BRepTools::Write(bottleBody, "C:/Users/DELL/Desktop/body.brep");

	displayASape(neckCylinder);
	Sleep(1000);

	TopoDS_Shape bottle = BRepAlgoAPI_Fuse(bottleBody, neckCylinder);

	TopoDS_Face   removingface;
	Standard_Real zMax = -1;
	
	TopExp_Explorer faceExplorer(bottle, TopAbs_FACE);

	while (faceExplorer.More()) {
		TopoDS_Face aFace = TopoDS::Face(faceExplorer.Current());
		Handle(Geom_Surface) aSurface = BRep_Tool::Surface(aFace);
		if (aSurface->DynamicType() == STANDARD_TYPE(Geom_Plane)) {
			Handle(Geom_Plane) aPlane = Handle(Geom_Plane)::DownCast(aSurface);
			gp_Pnt aPoint = aPlane->Location();
			Standard_Real aZ = aPoint.Z();
			if (aZ > zMax) {
				zMax = aZ;
				removingface = aFace;
			}
		}
		faceExplorer.Next();
	}

	TopTools_ListOfShape topFacesRemoving;
	topFacesRemoving.Append(removingface);

	TopoDS_Shape hollowedBottle = BRepOffsetAPI_MakeThickSolid(bottle, topFacesRemoving, BottleThickness/50, 1.e-3);

	removeDisplaiedAISShape();

	displayASape(hollowedBottle);
	Sleep(1000);

	removeDisplaiedAISShape();


	Handle(Geom_CylindricalSurface) aCyl1 = new Geom_CylindricalSurface(neckAxis, BottleNeckRadius * 1.08);
	Handle(Geom_CylindricalSurface) aCyl2 = new Geom_CylindricalSurface(neckAxis, BottleNeckRadius * 1.1);


	gp_Pnt2d aPnt(2. * M_PI, BottleNeckHight / 2.);
	gp_Dir2d aDir(2. * M_PI, BottleNeckHight / 4.);
	gp_Ax2d anAx2d(aPnt, aDir);

	Standard_Real aMajor = 2. * M_PI;
	Standard_Real aMinor = BottleNeckHight / 10;
	Handle(Geom2d_Ellipse) anEllipse1 = new Geom2d_Ellipse(anAx2d, aMajor, aMinor);
	Handle(Geom2d_Ellipse) anEllipse2 = new Geom2d_Ellipse(anAx2d, aMajor, aMinor / 4);

	Handle(Geom2d_TrimmedCurve) anArc1 = new Geom2d_TrimmedCurve(anEllipse1, 0, M_PI);
	Handle(Geom2d_TrimmedCurve) anArc2 = new Geom2d_TrimmedCurve(anEllipse2, 0, M_PI);


	gp_Pnt2d anEllipsePnt1 = anEllipse1->Value(0);
	gp_Pnt2d anEllipsePnt2;
	anEllipse1->D0(M_PI, anEllipsePnt2);

	Handle(Geom2d_TrimmedCurve) aSegment = GCE2d_MakeSegment(anEllipsePnt1, anEllipsePnt2);

	TopoDS_Edge anEdge1OnSurf1 = BRepBuilderAPI_MakeEdge(anArc1, aCyl1);
	TopoDS_Edge anEdge2OnSurf1 = BRepBuilderAPI_MakeEdge(aSegment, aCyl1);
	TopoDS_Edge anEdge1OnSurf2 = BRepBuilderAPI_MakeEdge(anArc2, aCyl2);
	TopoDS_Edge anEdge2OnSurf2 = BRepBuilderAPI_MakeEdge(aSegment, aCyl2);
	
	TopoDS_Wire threadingWire1 = BRepBuilderAPI_MakeWire(anEdge1OnSurf1, anEdge2OnSurf1);
	TopoDS_Wire threadingWire2 = BRepBuilderAPI_MakeWire(anEdge1OnSurf2, anEdge2OnSurf2);

	BRepLib::BuildCurves3d(threadingWire1);
	BRepLib::BuildCurves3d(threadingWire2);

	BRepOffsetAPI_ThruSections aTool(Standard_True);
	aTool.AddWire(threadingWire1); 
	aTool.AddWire(threadingWire2);
	aTool.CheckCompatibility(Standard_False);
	TopoDS_Shape myThreading = aTool.Shape();

	//displayASape(myThreading);
	Sleep(1000);

	TopoDS_Compound aRes;
	BRep_Builder aBuilder;
	aBuilder.MakeCompound(aRes);
	aBuilder.Add(aRes, hollowedBottle);
	aBuilder.Add(aRes, myThreading);

	//TopoDS_Shape last = BRepBuilderAPI_MakeShape(aRes);

	Handle(AIS_Shape) anAis = new AIS_Shape(aRes);
	anAis->SetColor(Quantity_NOC_CORAL);
	myOccView->getContext()->Display(anAis);




	//TopoDS_Shape aa = line.Shape();
	

	//Handle(AIS_Shape) anAisBottle = new AIS_Shape(topoDSPoint1);
	//anAisBottle->SetColor(Quantity_NOC_DEEPPINK2);
	//myOccView->getContext()->Display(anAisBottle);
}

void occQt::makeTextLabel(const char* lableText, const gp_Pnt gpPnt, Standard_Real Xoffset,
																	Standard_Real Yoffset,
																	Standard_Real Zoffset)
{
	Handle(AIS_TextLabel) label = new AIS_TextLabel();
	label->SetText(lableText);
	label->SetPosition(gp_Pnt(gpPnt.X() + Xoffset, gpPnt.Y() + Yoffset, gpPnt.Z() + Zoffset));
	myOccView->getContext()->Display(label);
	handleAISLablesVector.append(label);
}

void occQt::displayASape(TopoDS_Shape topoDSShape)
{
	Handle(AIS_Shape) anAis = new AIS_Shape(topoDSShape);
	anAis->SetColor(Quantity_NOC_CORAL);
	myOccView->getContext()->Display(anAis);
	handleAISShapesVector.append(anAis);
}

void occQt::removeDisplaiedAISShape(void)
{
	while (handleAISShapesVector.isEmpty() != TRUE) {
		Handle(AIS_Shape) anAis = handleAISShapesVector.takeLast();
		myOccView->getContext()->Remove(anAis);
	}
	myOccView->getContext()->UpdateCurrentViewer();
	
}

void occQt::removeDisplaiedAISLables()
{
	while (handleAISLablesVector.isEmpty() != TRUE) {
		Handle(AIS_TextLabel) anAis = handleAISLablesVector.takeLast();
		myOccView->getContext()->Remove(anAis);

	}

}

#include <QVBoxLayout>
//#include <QSpinBox>
#include <QDialogButtonBox>
#include <QDebug>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QMessageBox>

TopoDS_Shape occQt::makeBoxWithInput(QString dialogBoxName)
{
	double boxHeight;
	double boxLength;
	double boxWidth;
	//QString LableText;


	QDialog * d = new QDialog();
	d->setWindowTitle(dialogBoxName);
	QVBoxLayout * vbox = new QVBoxLayout();

	QLabel * lableLength = new QLabel("Length (x-Axis)  ");
	QDoubleSpinBox * spinBoxLength = new QDoubleSpinBox();
	spinBoxLength->setDecimals(5);
	spinBoxLength->setSingleStep(0.00001);
	spinBoxLength->setMaximum(std::numeric_limits<double>::infinity());
	QLabel * lableWidth = new QLabel("Width (y-Axis)  ");
	QDoubleSpinBox * spinBoxWidth = new QDoubleSpinBox();
	spinBoxWidth->setDecimals(5);
	spinBoxWidth->setSingleStep(0.00001);
	spinBoxWidth->setMaximum(std::numeric_limits<double>::infinity());
	QLabel * lableHeight = new QLabel("Heigth (z-Axis)  ");
	QDoubleSpinBox * spinBoxHeight = new QDoubleSpinBox();
	spinBoxHeight->setDecimals(5);
	spinBoxHeight->setSingleStep(0.00001);
	spinBoxHeight->setMaximum(std::numeric_limits<double>::infinity());
	//QLineEdit * lineEditA = new QLineEdit();

	QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	QObject::connect(buttonBox, SIGNAL(accepted()), d, SLOT(accept()));
	QObject::connect(buttonBox, SIGNAL(rejected()), d, SLOT(reject()));

	vbox->addWidget(lableLength);
	vbox->addWidget(spinBoxLength);
	vbox->addWidget(lableWidth);
	vbox->addWidget(spinBoxWidth);
	vbox->addWidget(lableHeight);
	vbox->addWidget(spinBoxHeight);
	//vbox->addWidget(lineEditA);
	vbox->addWidget(buttonBox);

	d->setLayout(vbox);

	int result = d->exec();

	while (true)
	{
		if (result == QDialog::Accepted)
		{
			boxLength = spinBoxLength->value();
			boxWidth = spinBoxWidth->value();
			boxHeight = spinBoxHeight->value();

		}
		else 
		{
			return box;
		}
	
		if (boxHeight == 0 | boxLength == 0 | boxWidth == 0)
		{
			QMessageBox msgBox;
			msgBox.setText("Fill All Values");
			msgBox.setIcon(QMessageBox::Warning);
			msgBox.exec();
			result = d->exec();

		}
		else
		{
			break;
		}
	}

	gp_Pnt point1(-boxLength, -boxWidth,0);
	gp_Pnt point2(0, -boxWidth, 0);
	gp_Pnt point3(-boxLength, 0, 0);
	gp_Pnt point4(0, 0, 0);

	Handle(Geom_TrimmedCurve) line1 = GC_MakeSegment(point1, point2);
	Handle(Geom_TrimmedCurve) line4 = GC_MakeSegment(point1, point3);
	Handle(Geom_TrimmedCurve) line2 = GC_MakeSegment(point4, point2);
	Handle(Geom_TrimmedCurve) line3 = GC_MakeSegment(point4, point3);

	TopoDS_Edge edge1 = BRepBuilderAPI_MakeEdge(line1);
	TopoDS_Edge edge2 = BRepBuilderAPI_MakeEdge(line2);
	TopoDS_Edge edge3 = BRepBuilderAPI_MakeEdge(line3);
	TopoDS_Edge edge4 = BRepBuilderAPI_MakeEdge(line4);

	TopoDS_Wire bottomWire = BRepBuilderAPI_MakeWire(edge1,edge2,edge3,edge4);

	TopoDS_Face bottomFace = BRepBuilderAPI_MakeFace(bottomWire);

	gp_Vec prismVec(0, 0, boxHeight);

	TopoDS_Shape Box = BRepPrimAPI_MakePrism(bottomFace, prismVec);

	return Box;
}

void occQt::displayBoxWithInputs()
{
	box = makeBoxWithInput("Create A Box");
	displayASape(box);

	//Sleep(1000);
	//removeDisplaiedAISShape();


	if (box.IsNull())
		return;

	mNavigateToolBar = addToolBar(tr("&Box"));
	mNavigateToolBar->addAction(BoxAddFillet);
	mNavigateToolBar->addAction(BoxDrillHole);
}


#include <QInputDialog>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>
#include <GCPnts_AbscissaPoint.hxx>
#include <GeomAdaptor_Curve.hxx>

void occQt::addBoxFillet()
{
	TopoDS_Shape filletingBox = box;

	if (isDrilledBox & !isFilletedBox)
	{
		//filletedBox = drilledBox;
		filletingBox = drilledBox;
	}
	else if (isFilletedBox & !isDrilledBox)
	{
		filletedBox = box;
		filletingBox = box;
	}
	else if (isDrilledBox & isFilletedBox)
	{
		filletingBox = drilledBox;
	}

	TopExp_Explorer edgeExplorer1(filletingBox, TopAbs_EDGE);

	GProp_GProps lengthProps;

	double minimumLength = std::numeric_limits<double>::infinity();

	while (edgeExplorer1.More())
	{
		TopoDS_Edge edge = TopoDS::Edge(edgeExplorer1.Current());
		BRepGProp::LinearProperties(edge, lengthProps);
		double length = lengthProps.Mass();
		if (length < minimumLength)
		{
			minimumLength = length;
		}
		edgeExplorer1.Next();
	}

	bool ok;
	double filletRadius = QInputDialog::getDouble(this, tr("Add Fillet Radius"),
		tr("Fillet Radius :  "), 0, 0, minimumLength/2 - 0.00001, 5, &ok);

	if (filletRadius == 0)
	{
		removeDisplaiedAISShape();
		displayASape(box);
		return;
	}
		

	BRepFilletAPI_MakeFillet filletMaker(filletingBox);
	removeDisplaiedAISShape();
	displayASape(filletingBox);

	TopExp_Explorer edgeExplorer(filletingBox, TopAbs_EDGE);

	while (edgeExplorer.More())
	{
		TopoDS_Edge edge = TopoDS::Edge(edgeExplorer.Current());
		filletMaker.Add(filletRadius, edge);
		edgeExplorer.Next();
	}

	filletingBox = filletMaker.Shape();

	if(!isFilletedBox & !isDrilledBox)
	{
		filletedBox = filletingBox;
	}

	isFilletedBox = true;

	removeDisplaiedAISShape();
	displayASape(filletingBox);
}

#include <QComboBox>
#include <BRepAlgoAPI_Cut.hxx>

void occQt::drillBoxHole()
{
	TopoDS_Shape drillingBox = box;

	if (isDrilledBox & !isFilletedBox)
	{
		drilledBox = box;
		drillingBox = box;
	}
	else if (isFilletedBox & !isDrilledBox)
	{
		drillingBox = filletedBox;
	}
	else if (isDrilledBox & isFilletedBox)
	{
		QMessageBox msgBox;
		msgBox.setText("If you want to change the drilling of filleted & drilled one before, you have to set fillets again after changinging the fillets");
		msgBox.setIcon(QMessageBox::Information);
		msgBox.exec();
		drillingBox = filletedBox;
	}

	QString selectedAxisText;
	QString selectedHoleShape;
	double BoxHoleRadius;
	double BoxHoleLength;

	QDialog * drillingAxisDialog = new QDialog();
	drillingAxisDialog->setWindowTitle("Drilling Axis");
	QVBoxLayout * vbox = new QVBoxLayout();

	QLabel * lable1 = new QLabel("Select the Drilling Axis");
	QComboBox * Axis = new QComboBox();
	Axis->addItem("x-Axis");
	Axis->addItem("y-Axis");
	Axis->addItem("z-Axis");

	QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	QObject::connect(buttonBox, SIGNAL(accepted()), drillingAxisDialog, SLOT(accept()));
	QObject::connect(buttonBox, SIGNAL(rejected()), drillingAxisDialog, SLOT(reject()));

	vbox->addWidget(lable1);
	vbox->addWidget(Axis);
	vbox->addWidget(buttonBox);

	drillingAxisDialog->setLayout(vbox);

	int result = drillingAxisDialog->exec();

	if (result == QDialog::Accepted)
	{
		selectedAxisText = Axis->currentText();
	}
	else
	{
		return;
	}

	TopoDS_Face drillingFace;
	gp_Ax1 drillingAxis;
	gp_Pnt drillingFaceCenter1;
	gp_Pnt drillingFaceCenter2;
	bool isSelectedOneFace = false;

	TopExp_Explorer faceExplorer(drillingBox, TopAbs_FACE);

	GProp_GProps faceCenterGprop;

	while (faceExplorer.More()) {
		TopoDS_Face aFace = TopoDS::Face(faceExplorer.Current());

		Handle(Geom_Surface) aSurface = BRep_Tool::Surface(aFace);
		if (aSurface->DynamicType() == STANDARD_TYPE(Geom_Plane)) {
			Handle(Geom_Plane) aPlane = Handle(Geom_Plane)::DownCast(aSurface);
			gp_Ax1 surfaceAxis = aPlane->Axis();


			if ((selectedAxisText == "x-Axis" & surfaceAxis.IsParallel(gp::OX(), 0))| 
				(selectedAxisText == "y-Axis" & surfaceAxis.IsParallel(gp::OY(), 0))|
				(selectedAxisText == "z-Axis" & surfaceAxis.IsParallel(gp::OZ(), 0)))
			{
				BRepGProp::SurfaceProperties(aFace, faceCenterGprop);
				gp_Pnt faceCenter = faceCenterGprop.CentreOfMass();

				drillingFace = aFace;
				drillingAxis = surfaceAxis;
				if (!isSelectedOneFace)
				{
					drillingFaceCenter1 = faceCenter;
					isSelectedOneFace = true;
				}
				else
				{
					drillingFaceCenter2 = faceCenter;
					break;
				}
			}
		}
		faceExplorer.Next();
	}

	gp_Vec holeVector(drillingFaceCenter1, drillingFaceCenter2);

	//double maximumHoleLength = holeVector.Magnitude();

	QDialog * holeRadiusDialog = new QDialog();
	holeRadiusDialog->setWindowTitle("Hole Radius");
	QVBoxLayout * holeRadiusVbox = new QVBoxLayout();

	QLabel * holeRadiusLable = new QLabel("Hole Radius");
	QDoubleSpinBox * spinBoxHoleRadius = new QDoubleSpinBox();
	spinBoxHoleRadius->setDecimals(5);
	spinBoxHoleRadius->setSingleStep(0.00001);
	spinBoxHoleRadius->setMaximum(std::numeric_limits<double>::infinity());
	QLabel * holelengthLabel = new QLabel("Hole Length");
	QDoubleSpinBox * spinBoxHolelength = new QDoubleSpinBox();
	spinBoxHolelength->setDecimals(5);
	spinBoxHolelength->setSingleStep(0.00001);
	spinBoxHolelength->setMaximum(std::numeric_limits<double>::infinity());
	//spinBoxHolelength->setMaximum(maximumHoleLength);

	QDialogButtonBox * holeRadiusButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	QObject::connect(holeRadiusButtonBox, SIGNAL(accepted()), holeRadiusDialog, SLOT(accept()));
	QObject::connect(holeRadiusButtonBox, SIGNAL(rejected()), holeRadiusDialog, SLOT(reject()));

	holeRadiusVbox->addWidget(holeRadiusLable);
	holeRadiusVbox->addWidget(spinBoxHoleRadius);
	holeRadiusVbox->addWidget(holelengthLabel);
	holeRadiusVbox->addWidget(spinBoxHolelength);
	holeRadiusVbox->addWidget(holeRadiusButtonBox);

	holeRadiusDialog->setLayout(holeRadiusVbox);


	int result2 = holeRadiusDialog->exec();

	if (result2 == QDialog::Accepted)
	{
		BoxHoleRadius = spinBoxHoleRadius->value();
		BoxHoleLength = spinBoxHolelength->value();
	}
	else
	{
		return;
	}

	if (BoxHoleRadius == 0 | BoxHoleLength == 0)
	{
		removeDisplaiedAISShape();
		displayASape(drillingBox);
		return;
	}
		
	gp_Dir haleDirection(holeVector);

	gp_Ax2 dDrillingAxis(drillingFaceCenter1, haleDirection);

	//dDrillingAxis.SetAxis(drillingAxis);
	//dDrillingAxis.SetLocation(drillingFaceCenter1);
	TopoDS_Shape topoCylinder = BRepPrimAPI_MakeCylinder(dDrillingAxis, BoxHoleRadius, BoxHoleLength).Shape();

	removeDisplaiedAISShape();

	//displayASape(topoCylinder);

	drillingBox = BRepAlgoAPI_Cut(drillingBox, topoCylinder);

	if (!isFilletedBox & !isDrilledBox)
	{
		drilledBox = drillingBox;
	}

	isDrilledBox = true;

	
	displayASape(drillingBox);
}

#include <Geom_Curve.hxx>
#include <GeomLProp_CLProps.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>

void occQt::setFirTree()
{

	Standard_Real A1;	//angle from blade center to one hub side
	Standard_Real A2;	//angle from blade center to next hub side
	Standard_Real A3;	//angle from blade center to fir tree center


	/* get the imported blade shape to the parameter shape*/
	TopoDS_Shape shape;
	AIS_ListOfInteractive objList;
	myOccView->getContext()->DisplayedObjects(objList);
	AIS_ListIteratorOfListOfInteractive iter;
	for (iter.Initialize(objList); iter.More(); iter.Next())
	{
		Handle(AIS_InteractiveObject) aisShp = iter.Value();
		if (aisShp->IsKind("AIS_Shape"))
		{
			shape = Handle(AIS_Shape)::DownCast(aisShp)->Shape();
		}
	}


	/* get the selected face to the parameter selected face*/
	TopoDS_Shape selectedShape;
	myOccView->getContext()->InitSelected();
	selectedShape = myOccView->getContext()->SelectedShape();
	TopoDS_Face selectedFace = TopoDS::Face(selectedShape);


	/* find what the curved edges and straight edges*/

	TopExp_Explorer edgeExplorer(selectedFace, TopAbs_EDGE);

	QVector<TopoDS_Edge> curvedTopoDSEdges;
	QVector<TopoDS_Edge> straightTopoDSEdges;
	QVector<Handle(Geom_Curve)> curvedEdges;
	QVector<Handle(Geom_Curve)> straightEdges;

	while (edgeExplorer.More())
	{
		TopoDS_Edge edge = TopoDS::Edge(edgeExplorer.Current());
		bool w = edge.Convex();
		Standard_Real first;
		Standard_Real last;
		Standard_Real Resolution;
		Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
		GeomLProp_CLProps aCurrentCurve_Props(curve, 2, Resolution);
		aCurrentCurve_Props.SetParameter(first);
		Standard_Real aCurrentCurve_Curvature = aCurrentCurve_Props.Curvature();

		if (aCurrentCurve_Curvature != 0)
		{
			curvedTopoDSEdges.append(edge);
			displayASape(edge);
		}
		else
		{
			straightTopoDSEdges.append(edge);
		}
		edgeExplorer.Next();
	}

	/* find what the longer curve edge*/

	TopoDS_Edge longerCurveEdge;
	TopoDS_Edge shoterCurveEdge;

	GProp_GProps lengthProps1;
	GProp_GProps lengthProps2;

	TopoDS_Edge edge1 = curvedTopoDSEdges.at(0);
	TopoDS_Edge edge2 = curvedTopoDSEdges.at(1);

	BRepGProp::LinearProperties(edge1, lengthProps1);
	double lengthOfEdge1 = lengthProps1.Mass();
	BRepGProp::LinearProperties(edge2, lengthProps2);
	double lengthOfEdge2 = lengthProps2.Mass();

	double longerCurveLength;

	if(lengthOfEdge1<lengthOfEdge2)
	{
		longerCurveEdge = edge2;
		shoterCurveEdge = edge1;
		longerCurveLength = lengthOfEdge2;
	}
	else
	{
		longerCurveEdge = edge1;
		shoterCurveEdge = edge2;
		longerCurveLength = lengthOfEdge1;
	}

	/* find the center of the longer curve*/
	/* we get this center point as the point on both the center axis of blade
		and  the long curve*/

	gp_Pnt longerCurveCenterPoint;
	gp_Pnt shoterCurveCenterPoint;

	Standard_Real firstParameterOfLongerCurve;
	Standard_Real lastParameterOfLongerCurve;
	Handle(Geom_Curve) longerCurve = BRep_Tool::Curve(longerCurveEdge, firstParameterOfLongerCurve, lastParameterOfLongerCurve);
	Standard_Real midOflonger = (firstParameterOfLongerCurve + lastParameterOfLongerCurve) / 2;
	longerCurveCenterPoint = longerCurve->Value(midOflonger);

	gp_Pnt  firstParameterPointOfLongerCurve = longerCurve->Value(firstParameterOfLongerCurve);

	Standard_Real firstParameterOfShoterCurve;
	Standard_Real lastParameterOfShoterCurve;
	Handle(Geom_Curve) shoterCurve = BRep_Tool::Curve(shoterCurveEdge, firstParameterOfShoterCurve, lastParameterOfShoterCurve);
	Standard_Real midOfShoter = (firstParameterOfShoterCurve + lastParameterOfShoterCurve) / 2;
	shoterCurveCenterPoint = shoterCurve->Value(midOfShoter);

	/* use this center line length to get dimention of fire tree as a frantion (for hard coddings)*/
	Standard_Real CenterLineLength = longerCurveCenterPoint.Distance(shoterCurveCenterPoint);

	/* get the rotating axis as the faceCenterAxis*/
	gp_Pnt rotatingAxisPoint(0, 0, longerCurveCenterPoint.Z());

	gp_Vec faceCenterVector(longerCurveCenterPoint, rotatingAxisPoint);

	gp_Dir faceCenterDirection(faceCenterVector);

	gp_Ax1 faceCenterAxis(longerCurveCenterPoint, faceCenterDirection);


	gp_Dir zDir(0, 0, 1);
	gp_Ax1 rotatingAxis(rotatingAxisPoint, zDir);
	gp_Trsf A1Transformation;
	A1Transformation.SetRotation(rotatingAxis, A1);


	gp_Vec faceCenterUnitVector = faceCenterVector.Divided(CenterLineLength);

	TopoDS_Edge hubBottomCurveEdge;

	Standard_Real hubThickness = CenterLineLength * 0.2;

	TopoDS_Shape trnsFormingEdge = longerCurveEdge;

	gp_Trsf theTransformation;
	gp_Vec centerUnitVector(faceCenterDirection);
	theTransformation.SetTranslation(centerUnitVector.Multiplied(hubThickness));
	BRepBuilderAPI_Transform myBRepTransformation(trnsFormingEdge, theTransformation, true);

	TopoDS_Shape hubBottomCurveShape = myBRepTransformation.Shape();

	hubBottomCurveEdge = TopoDS::Edge(hubBottomCurveShape);

	Standard_Real firstParameterOfHabBottemCurve;
	Standard_Real lastParameterOfHabBottemCurve;
	Handle(Geom_Curve) hubBottomCurve = BRep_Tool::Curve(hubBottomCurveEdge, firstParameterOfHabBottemCurve, lastParameterOfHabBottemCurve);
	
	Standard_Real midParameterOfHabBottemCurve = (firstParameterOfHabBottemCurve + lastParameterOfHabBottemCurve) / 2;

	Standard_Real diference =( midParameterOfHabBottemCurve - firstParameterOfHabBottemCurve);

	Standard_Real U3 = midParameterOfHabBottemCurve - (lastParameterOfHabBottemCurve * 0.1);

	Handle(Geom_Curve) toolUperHalfLineCurve =new Geom_TrimmedCurve (hubBottomCurve, firstParameterOfHabBottemCurve, U3);

	TopoDS_Edge aEdge1 = BRepBuilderAPI_MakeEdge(toolUperHalfLineCurve);

	gp_Pnt neckStartPoint = toolUperHalfLineCurve->Value(U3);

	Standard_Real neckHeight = CenterLineLength * 0.2;

	gp_Trsf theTransformationNeckPoint;
	theTransformationNeckPoint.SetTranslation(centerUnitVector.Multiplied(neckHeight));

	gp_Pnt neckEndPoint = neckStartPoint.Transformed(theTransformationNeckPoint);
	
	Handle(Geom_TrimmedCurve) neckLineCurve = GC_MakeSegment(neckStartPoint, neckEndPoint);

	TopoDS_Edge aEdge2 = BRepBuilderAPI_MakeEdge(neckLineCurve);

	QVector<TopoDS_Edge> firstHalfEdges;
	firstHalfEdges.append(aEdge1);
	firstHalfEdges.append(aEdge2);


	gp_Pnt lastAngle_2_LastPoint;

	gp_Pnt Angle_1_FirstPoint = neckEndPoint;

	for (int i = 0; i < 3; i++)
	{
		Standard_Real angleLength = CenterLineLength * (0.9 - (0.15*i));

		gp_Trsf theTransformationAngle_1_Point;
		theTransformationAngle_1_Point.SetTranslation(centerUnitVector.Multiplied(angleLength));

		gp_Pnt angleLastPointBefore = Angle_1_FirstPoint.Transformed(theTransformationAngle_1_Point);

		gp_Dir zDir(0, 0, 1);
		gp_Ax1 neckEndPointAxis(Angle_1_FirstPoint, zDir);

		gp_Pnt angle_1_LastPoint = angleLastPointBefore.Rotated(neckEndPointAxis, ((3.141 * 2.5) / 6));

		Handle(Geom_TrimmedCurve) angle_1_Curve = GC_MakeSegment(Angle_1_FirstPoint, angle_1_LastPoint);

		TopoDS_Edge aEdge3 = BRepBuilderAPI_MakeEdge(angle_1_Curve);

		firstHalfEdges.append(aEdge3);

		Standard_Real angleThickness = CenterLineLength * 0.2;

		gp_Trsf theTransformationAngle_2_Point;
		theTransformationAngle_2_Point.SetTranslation(centerUnitVector.Multiplied(angleThickness));

		gp_Pnt angle_2_LastPoint = Angle_1_FirstPoint.Transformed(theTransformationAngle_2_Point);

		Handle(Geom_TrimmedCurve) angle_2_Curve = GC_MakeSegment(angle_1_LastPoint, angle_2_LastPoint);

		TopoDS_Edge aEdge4 = BRepBuilderAPI_MakeEdge(angle_2_Curve);

		firstHalfEdges.append(aEdge4);

		lastAngle_2_LastPoint = angle_2_LastPoint;
		Angle_1_FirstPoint = lastAngle_2_LastPoint;

	}
	Standard_Real firTreeHeight = CenterLineLength * 0.9;

	gp_Trsf theTransformationMidBottomPoint;
	theTransformationMidBottomPoint.SetTranslation(centerUnitVector.Multiplied(firTreeHeight));

	gp_Pnt MidBottomPointPoint(longerCurveCenterPoint.X(), lastAngle_2_LastPoint.Y(), longerCurveCenterPoint.Z());

	Handle(Geom_TrimmedCurve) centerConnectCurve = GC_MakeSegment(lastAngle_2_LastPoint, MidBottomPointPoint);

	TopoDS_Edge aEdge5 = BRepBuilderAPI_MakeEdge(centerConnectCurve);
	firstHalfEdges.append(aEdge5);

	gp_Trsf theTransformationToolMidBottomPoint;
	theTransformationToolMidBottomPoint.SetTranslation(centerUnitVector.Multiplied(CenterLineLength*2));

	gp_Pnt toolMiddBottomPoint = longerCurveCenterPoint.Transformed(theTransformationToolMidBottomPoint);

	gp_Pnt toolUpperHalfLineOuterCurve = toolUperHalfLineCurve->Value(firstParameterOfHabBottemCurve);

	gp_Pnt toolLastPoint(toolUpperHalfLineOuterCurve.X(), toolMiddBottomPoint.Y(), toolUpperHalfLineOuterCurve.Z());

	Handle(Geom_TrimmedCurve) toolbottomCurve = GC_MakeSegment(toolMiddBottomPoint, toolLastPoint);

	TopoDS_Edge aEdge6 = BRepBuilderAPI_MakeEdge(toolbottomCurve);

	Handle(Geom_TrimmedCurve) toolSideCurve = GC_MakeSegment(toolLastPoint, toolUpperHalfLineOuterCurve);

	TopoDS_Edge aEdge7 = BRepBuilderAPI_MakeEdge(toolSideCurve);

	firstHalfEdges.insert(0, aEdge7);
	firstHalfEdges.insert(0, aEdge6);


	BRepBuilderAPI_MakeWire wireMaker1;

	for (int i = 0; i < firstHalfEdges.length();i++)
	{
		wireMaker1.Add(firstHalfEdges.at(i));
	}


	TopoDS_Wire halfWireProfile = wireMaker1.Wire();

	
	gp_Trsf aTrsf;
	aTrsf.SetMirror(faceCenterAxis);
	BRepBuilderAPI_Transform aBRepTrsf(halfWireProfile, aTrsf);
	TopoDS_Shape aMirroredShape = aBRepTrsf.Shape();
	TopoDS_Wire aMirroredWire = TopoDS::Wire(aMirroredShape);


	BRepBuilderAPI_MakeWire mkWire(aMirroredWire);
	mkWire.Add(halfWireProfile);
	TopoDS_Wire WireProfile = mkWire.Wire();

	TopoDS_Face myFaceProfile = BRepBuilderAPI_MakeFace(WireProfile);

	gp_Vec aPrismVec(0, 0, -50);

	TopoDS_Shape cuttingToolBody = BRepPrimAPI_MakePrism(myFaceProfile, aPrismVec);

	TopoDS_Shell shell = TopoDS::Shell(shape);

	shape = BRepBuilderAPI_MakeSolid(shell);

	shape = BRepAlgoAPI_Cut(shape, cuttingToolBody);

	removeDisplaiedAISShape();
	displayASape(shape);

}

#include <GeomLib_Tool.hxx>
#include <ChFi2d_FilletAPI.hxx>
#include <QTime>
#include <GeomConvert.hxx>
void occQt::setFirTree2()
{
	QTime myTimer;
	myTimer.start();


	/* these are my inputs. hard coded just for now*/
	Standard_Real A1 = (3.141 * 1.5) / 6;	//angle from blade center to one hub side
	Standard_Real A2 = (3.141 * 1.5) / 6;		//angle from blade center to next hub side
	Standard_Real A3 = (3.141 * (0.7)) / 6;	//angle from blade center to fir tree center

	Standard_Real A4 = (3.141 * 0.5) / 15;	//just for now, assume Neck Thickness by an angle

	gp_Pnt bladeCenterPointOnHubSerface;	//asume it is the selected face longer curve center 
	Standard_Real hubThickness;
	Standard_Real neckHeight;	//just hard coded as a fraction of selected face center line length
	Standard_Real LobeLoadLength;	//this is the first firtree upper angle length
							//get it as a fraction of selected face center line length
							//other other angle lengths are redusing fraction of this
	Standard_Real LobeThickness;	//get it as a fraction of selected face center line length
							 //other other angle lengths are redusing fraction of this

	Standard_Real lobeLodeAngle = (3.141 * 2.5) / 6;

	Standard_Real concaveRadius = 0.5;
	Standard_Real convexRadius = 0.5;
	Standard_Real firTreebottomFilletRadius = 0.8;
	Standard_Real firTreeupperFilletRadius = 0.9;

	Standard_Real firTreeDepthFrom_SelectedFace = 5;
	Standard_Real firTreeDepthFrom_NextToSelectedFace = 5;




	/* get the imported blade shape to the parameter shape*/
	TopoDS_Shape shape;
	AIS_ListOfInteractive objList;
	myOccView->getContext()->DisplayedObjects(objList);
	AIS_ListIteratorOfListOfInteractive iter;
	for (iter.Initialize(objList); iter.More(); iter.Next())
	{
		Handle(AIS_InteractiveObject) aisShp = iter.Value();
		if (aisShp->IsKind("AIS_Shape"))
		{
			shape = Handle(AIS_Shape)::DownCast(aisShp)->Shape();
		}
	}


	/* convert given shell to a solid*/
	TopoDS_Shell shell = TopoDS::Shell(shape);
	shape = BRepBuilderAPI_MakeSolid(shell);


	/* get the selected face to the parameter selectedFace*/
	TopoDS_Shape selectedShape;
	myOccView->getContext()->InitSelected();
	selectedShape = myOccView->getContext()->SelectedShape();
	TopoDS_Face selectedFace = TopoDS::Face(selectedShape);


	/* find what the curved edges and straight edges*/
	TopExp_Explorer edgeExplorer(selectedFace, TopAbs_EDGE);

	QVector<TopoDS_Edge> curvedTopoDSEdges;
	QVector<TopoDS_Edge> straightTopoDSEdges;
	//QVector<Handle(Geom_Curve)> curvedEdges;
	//QVector<Handle(Geom_Curve)> straightEdges;

	while (edgeExplorer.More())
	{
		TopoDS_Edge edge = TopoDS::Edge(edgeExplorer.Current());
		bool w = edge.Convex();
		Standard_Real first;
		Standard_Real last;
		Standard_Real Resolution;
		Handle(Geom_Curve) curve = BRep_Tool::Curve(edge, first, last);
		GeomLProp_CLProps aCurrentCurve_Props(curve, 2, Resolution);
		aCurrentCurve_Props.SetParameter(first);
		Standard_Real aCurrentCurve_Curvature = aCurrentCurve_Props.Curvature();

		if (aCurrentCurve_Curvature != 0)
		{
			curvedTopoDSEdges.append(edge);
		}
		else
		{
			straightTopoDSEdges.append(edge);
		}
		edgeExplorer.Next();
	}

	/* find what the longer curve edge*/
	TopoDS_Edge longerCurveEdge;
	TopoDS_Edge shoterCurveEdge;

	GProp_GProps lengthProps1;
	GProp_GProps lengthProps2;

	TopoDS_Edge edge1 = curvedTopoDSEdges.at(0);
	TopoDS_Edge edge2 = curvedTopoDSEdges.at(1);

	BRepGProp::LinearProperties(edge1, lengthProps1);
	double lengthOfEdge1 = lengthProps1.Mass();
	BRepGProp::LinearProperties(edge2, lengthProps2);
	double lengthOfEdge2 = lengthProps2.Mass();

	double longerCurveLength;

	if (lengthOfEdge1<lengthOfEdge2)
	{
		longerCurveEdge = edge2;
		shoterCurveEdge = edge1;
		longerCurveLength = lengthOfEdge2;
	}
	else
	{
		longerCurveEdge = edge1;
		shoterCurveEdge = edge2;
		longerCurveLength = lengthOfEdge1;
	}

	/* find the center of the longer curve*/
	/* we get this center point as the point on both the center axis of blade
	and  the long curve*/

	gp_Pnt longerCurveCenterPoint;
	gp_Pnt shoterCurveCenterPoint;

	Standard_Real firstParameterOfLongerCurve;
	Standard_Real lastParameterOfLongerCurve;
	Handle(Geom_Curve) longerCurve = BRep_Tool::Curve(longerCurveEdge, firstParameterOfLongerCurve, lastParameterOfLongerCurve);
	Standard_Real midParameterOflongercurve = (firstParameterOfLongerCurve + lastParameterOfLongerCurve) / 2;
	longerCurveCenterPoint = longerCurve->Value(midParameterOflongercurve);

	gp_Pnt firstParameterPointOfLongerCurve = longerCurve->Value(firstParameterOfLongerCurve);
	gp_Pnt lastParameterPointOfLongerCurve = longerCurve->Value(lastParameterOfLongerCurve);


	bladeCenterPointOnHubSerface = longerCurveCenterPoint;  // just for hard code because don't know the blade center

	/* find the center of the shoter curve*/

	Standard_Real firstParameterOfShoterCurve;
	Standard_Real lastParameterOfShoterCurve;
	Handle(Geom_Curve) shoterCurve = BRep_Tool::Curve(shoterCurveEdge, firstParameterOfShoterCurve, lastParameterOfShoterCurve);
	Standard_Real midOfShoter = (firstParameterOfShoterCurve + lastParameterOfShoterCurve) / 2;
	shoterCurveCenterPoint = shoterCurve->Value(midOfShoter);


	/* use this center line length to get dimention of fire tree as a frantion (for hard coddings) and 
		to get it as the side line length of given shape to creat the cutting tool face side line*/
	Standard_Real CenterLineLength = longerCurveCenterPoint.Distance(shoterCurveCenterPoint);

	
	gp_Pnt rotatingAxisPointForSelectedPlane(0, 0, bladeCenterPointOnHubSerface.Z());

	gp_Vec firstParameterOfLongerCurveToZAxisVector(firstParameterPointOfLongerCurve, rotatingAxisPointForSelectedPlane);
	gp_Dir firstParameterOfLongerCurveToZAxis_Direction(firstParameterOfLongerCurveToZAxisVector);
	gp_Vec firstParameterOfLongerCurveToZAxis_UnitVector(firstParameterOfLongerCurveToZAxis_Direction);

	gp_Vec lastParameterOfLongerCurveToZAxisVector(lastParameterPointOfLongerCurve, rotatingAxisPointForSelectedPlane);
	gp_Dir lastParameterOfLongerCurveToZAxis_Direction(lastParameterOfLongerCurveToZAxisVector);
	gp_Vec lastParameterOfLongerCurveToZAxis_UnitVector(lastParameterOfLongerCurveToZAxis_Direction);

	/* get the blade center line vector from longerCurve to rotating point as bladeCenterLineVector*/
	gp_Vec bladeCenterLineVector(bladeCenterPointOnHubSerface, rotatingAxisPointForSelectedPlane);

	gp_Ax1 rotatingAxis(rotatingAxisPointForSelectedPlane, gp::DZ());
	gp_Ax2 rotatingAxis2(rotatingAxisPointForSelectedPlane, gp::DZ());
	gp_Ax3 rotatingAxis3(rotatingAxis2);

	/////////////////

	Standard_Real bladeCenterToFirstParameterPointAngle = qAbs(bladeCenterLineVector.Angle(firstParameterOfLongerCurveToZAxisVector));

	/* get the cutting edge point on longercuve by A1 angle*/
	gp_Pnt pointOnLongerCurveByA1_AngleFromBladeCenterLine = bladeCenterPointOnHubSerface.Rotated(rotatingAxis, A1);
	Standard_Real parameterByA1_AngleFromBladeCenterLine;
	GeomLib_Tool::Parameter(longerCurve, pointOnLongerCurveByA1_AngleFromBladeCenterLine, 1e-7, parameterByA1_AngleFromBladeCenterLine);
	gp_Vec pointOnLongerCurveByA1ToZAxisVector(pointOnLongerCurveByA1_AngleFromBladeCenterLine, rotatingAxisPointForSelectedPlane);


	/* get the cutting edge point on longercuve by A2 angle for next side*/
	gp_Pnt pointOnLongerCurveByA2_AngleFromBladeCenterLine = bladeCenterPointOnHubSerface.Rotated(rotatingAxis, -A2);
	Standard_Real parameterByA2_AngleFromBladeCenterLine;
	GeomLib_Tool::Parameter(longerCurve, pointOnLongerCurveByA2_AngleFromBladeCenterLine, 1e-7, parameterByA2_AngleFromBladeCenterLine);
	gp_Vec pointOnLongerCurveByA2ToZAxisVector(pointOnLongerCurveByA2_AngleFromBladeCenterLine, rotatingAxisPointForSelectedPlane);


	/* what is near to the firstParameterOfLongerCurve from parameterByA1_AngleFromBladeCenterLine 
		and parameterByA2_AngleFromBladeCenterLine*/
	gp_Pnt firstParameterSideLongCurveCuttingEgdgePoint;
	Standard_Real firstParameterSideLongCurveCuttingEgdgeParameter;
	gp_Pnt lastParameterSideLongCurveCuttingEgdgePoint;
	Standard_Real lastParameterSideLongCurveCuttingEgdgeParameter;

	Standard_Real fromFirstParameterPointToPointOnLongerCurveByA1Angle = qAbs(firstParameterOfLongerCurveToZAxisVector.Angle(pointOnLongerCurveByA1ToZAxisVector));
	
	if (bladeCenterToFirstParameterPointAngle < fromFirstParameterPointToPointOnLongerCurveByA1Angle)
	{
		firstParameterSideLongCurveCuttingEgdgePoint = pointOnLongerCurveByA2_AngleFromBladeCenterLine;
		firstParameterSideLongCurveCuttingEgdgeParameter = parameterByA2_AngleFromBladeCenterLine;
		lastParameterSideLongCurveCuttingEgdgePoint = pointOnLongerCurveByA1_AngleFromBladeCenterLine;
		lastParameterSideLongCurveCuttingEgdgeParameter = parameterByA1_AngleFromBladeCenterLine;
	}
	else
	{
		firstParameterSideLongCurveCuttingEgdgePoint = pointOnLongerCurveByA1_AngleFromBladeCenterLine;
		firstParameterSideLongCurveCuttingEgdgeParameter = parameterByA1_AngleFromBladeCenterLine;
		lastParameterSideLongCurveCuttingEgdgePoint = pointOnLongerCurveByA2_AngleFromBladeCenterLine;
		lastParameterSideLongCurveCuttingEgdgeParameter = parameterByA2_AngleFromBladeCenterLine;
	}


	hubThickness = CenterLineLength * 0.2;

	/* find 3 points to to creat the inner hub face circle*/
	gp_Vec firstCuttingEdgePointToZAxis_Vector(firstParameterSideLongCurveCuttingEgdgePoint, rotatingAxisPointForSelectedPlane);
	gp_Dir firstCuttingEdgePointToZAxis_Direction(firstCuttingEdgePointToZAxis_Vector);
	gp_Vec firstCuttingEdgePointToZAxis_UnitVector(firstCuttingEdgePointToZAxis_Direction);

	gp_Trsf theTransformation_FirstCuttingEdgePoint;
	theTransformation_FirstCuttingEdgePoint.SetTranslation(firstCuttingEdgePointToZAxis_UnitVector.Multiplied(hubThickness));

	gp_Pnt point1 = firstParameterSideLongCurveCuttingEgdgePoint.Transformed(theTransformation_FirstCuttingEdgePoint);

	gp_Vec lastCuttingEdgePointToZAxis_Vector(lastParameterSideLongCurveCuttingEgdgePoint, rotatingAxisPointForSelectedPlane);
	gp_Dir lastCuttingEdgePointToZAxis_Direction(lastCuttingEdgePointToZAxis_Vector);
	gp_Vec lastCuttingEdgePointToZAxis_UnitVector(lastCuttingEdgePointToZAxis_Direction);

	gp_Trsf theTransformation_LastCuttingEdgePoint;
	theTransformation_LastCuttingEdgePoint.SetTranslation(lastCuttingEdgePointToZAxis_UnitVector.Multiplied(hubThickness));

	gp_Pnt point2 = lastParameterSideLongCurveCuttingEgdgePoint.Transformed(theTransformation_LastCuttingEdgePoint);

	gp_Dir bladeCenterLine_Direction(bladeCenterLineVector);
	gp_Vec bladeCenterLine_UnitVector(bladeCenterLine_Direction);

	gp_Trsf theTransformation_bladeCenterPoint;
	theTransformation_bladeCenterPoint.SetTranslation(bladeCenterLine_UnitVector.Multiplied(hubThickness));

	gp_Pnt point3 = bladeCenterPointOnHubSerface.Transformed(theTransformation_bladeCenterPoint);

	/* find cutting tool's hub face curves*/
	gp_Trsf Transformation_firstParameterSideLongCurveCuttingEgdgePointCuttingTool;
	Transformation_firstParameterSideLongCurveCuttingEgdgePointCuttingTool.SetTranslation(-((firstCuttingEdgePointToZAxis_UnitVector.Multiplied(5))));
	gp_Pnt firstParameterSideLongCurveCuttingEgdgePoint_CuttingTool = 
		firstParameterSideLongCurveCuttingEgdgePoint.Transformed(Transformation_firstParameterSideLongCurveCuttingEgdgePointCuttingTool);

	gp_Trsf Transformation_firstParameterPointCuttingTool;
	Transformation_firstParameterPointCuttingTool.SetTranslation(-((firstParameterOfLongerCurveToZAxis_UnitVector.Multiplied(5))));
	gp_Pnt firstParameterPoint_CuttingTool =
		firstParameterPointOfLongerCurve.Transformed(Transformation_firstParameterPointCuttingTool);

	gp_Trsf Transformation_lastParameterSideLongCurveCuttingEgdgePointCuttingTool;
	Transformation_lastParameterSideLongCurveCuttingEgdgePointCuttingTool.SetTranslation(-((lastCuttingEdgePointToZAxis_UnitVector.Multiplied(5))));
	gp_Pnt lastParameterSideLongCurveCuttingEgdgePoint_CuttingTool =
		lastParameterSideLongCurveCuttingEgdgePoint.Transformed(Transformation_lastParameterSideLongCurveCuttingEgdgePointCuttingTool);

	gp_Trsf Transformation_lastParameterPointCuttingTool;
	Transformation_lastParameterPointCuttingTool.SetTranslation(-((lastParameterOfLongerCurveToZAxis_UnitVector.Multiplied(5))));
	gp_Pnt lastParameterPoint_CuttingTool =
		lastParameterPointOfLongerCurve.Transformed(Transformation_lastParameterPointCuttingTool);

	gp_Circ hubfaceCuttingToolCircle(rotatingAxis2, firstParameterSideLongCurveCuttingEgdgePoint_CuttingTool.Distance(rotatingAxisPointForSelectedPlane));

	Handle(Geom_TrimmedCurve) hubFaceCurve1_CuttingTool = GC_MakeArcOfCircle(hubfaceCuttingToolCircle,
																	firstParameterPoint_CuttingTool,
																	firstParameterSideLongCurveCuttingEgdgePoint_CuttingTool, true);
	Handle(Geom_TrimmedCurve) hubFaceCurve2_CuttingTool = GC_MakeArcOfCircle(hubfaceCuttingToolCircle,
																	lastParameterSideLongCurveCuttingEgdgePoint_CuttingTool,
																	lastParameterPoint_CuttingTool, true);

	TopoDS_Edge hubFaceEdge1_CuttingTool = BRepBuilderAPI_MakeEdge(hubFaceCurve1_CuttingTool);
	TopoDS_Edge hubFaceEdge2_CuttingTool = BRepBuilderAPI_MakeEdge(hubFaceCurve2_CuttingTool);

	/////////
	gp_Pnt firstParameterSide_CuttingToolPoint = longerCurve->Value(firstParameterOfLongerCurve- (midParameterOflongercurve/5));
	gp_Pnt lastParameterSide_CuttingToolPoint = longerCurve->Value(lastParameterOfLongerCurve + (midParameterOflongercurve / 5));

	gp_Vec firstParameterSide_CuttingToolPointToZaxis_Vector(firstParameterSide_CuttingToolPoint, rotatingAxisPointForSelectedPlane);
	gp_Dir firstParameterSide_CuttingToolPointToZaxis_VectorDirection(firstParameterSide_CuttingToolPointToZaxis_Vector);
	gp_Vec firstParameterSide_CuttingToolPointToZaxis_UnitVector(firstParameterSide_CuttingToolPointToZaxis_VectorDirection);

	gp_Vec lastParameterSide_CuttingToolPointToZaxis_Vector(lastParameterSide_CuttingToolPoint, rotatingAxisPointForSelectedPlane);
	gp_Dir lastParameterSide_CuttingToolPointToZaxis_VectorDirection(lastParameterSide_CuttingToolPointToZaxis_Vector);
	gp_Vec lastParameterSide_CuttingToolPointToZaxis_UnitVector(lastParameterSide_CuttingToolPointToZaxis_VectorDirection);

	gp_Trsf Transformation_firstParameterSide_CuttingToolPoint;
	Transformation_firstParameterSide_CuttingToolPoint.SetTranslation(firstParameterSide_CuttingToolPointToZaxis_UnitVector * (CenterLineLength + 6));
	gp_Pnt firstParameterSide_CuttingTool_BottomPoint =
		firstParameterSide_CuttingToolPoint.Transformed(Transformation_firstParameterSide_CuttingToolPoint);

	gp_Trsf Transformation_lastParameterSide_CuttingToolPoint;
	Transformation_lastParameterSide_CuttingToolPoint.SetTranslation(lastParameterSide_CuttingToolPointToZaxis_UnitVector * (CenterLineLength + 6));
	gp_Pnt lastParameterSide_CuttingTool_BottomPoint =
		lastParameterSide_CuttingToolPoint.Transformed(Transformation_lastParameterSide_CuttingToolPoint);

	Handle(Geom_TrimmedCurve) firstParameterSide_cuttingToolCurve = GC_MakeSegment(firstParameterSide_CuttingToolPoint, 
																			firstParameterSide_CuttingTool_BottomPoint);
	Handle(Geom_TrimmedCurve) lastParameterSide_cuttingToolCurve = GC_MakeSegment(lastParameterSide_CuttingToolPoint,
																			lastParameterSide_CuttingTool_BottomPoint);

	TopoDS_Edge firstParameterSide_cuttingToolEdge = BRepBuilderAPI_MakeEdge(firstParameterSide_cuttingToolCurve);
	TopoDS_Edge lastParameterSide_cuttingToolEdge = BRepBuilderAPI_MakeEdge(lastParameterSide_cuttingToolCurve);


	Handle(Geom_TrimmedCurve) upperConnecter1_Curve = GC_MakeSegment(firstParameterPoint_CuttingTool,
																firstParameterSide_CuttingToolPoint);
	Handle(Geom_TrimmedCurve) upperConnecter2_Curve = GC_MakeSegment(lastParameterPoint_CuttingTool,
																lastParameterSide_CuttingToolPoint);

	TopoDS_Edge upperConnecter1_Edge = BRepBuilderAPI_MakeEdge(upperConnecter1_Curve);
	TopoDS_Edge upperConnecter2_Edge = BRepBuilderAPI_MakeEdge(upperConnecter2_Curve);

	Handle(Geom_TrimmedCurve) cuttingTool_BottomCurve = GC_MakeSegment(firstParameterSide_CuttingTool_BottomPoint,
																	lastParameterSide_CuttingTool_BottomPoint);

	TopoDS_Edge cuttingTool_BottomEdge = BRepBuilderAPI_MakeEdge(cuttingTool_BottomCurve);


	/* create innner hub serface*/
	Handle(Geom_TrimmedCurve) hubBottomFaceCurve = GC_MakeArcOfCircle(point1, point3, point2);
	

	Handle(Geom_TrimmedCurve) hubSide1Curve = GC_MakeSegment(firstParameterSideLongCurveCuttingEgdgePoint_CuttingTool, point1);
	Handle(Geom_TrimmedCurve) hubSide2Curve = GC_MakeSegment(lastParameterSideLongCurveCuttingEgdgePoint_CuttingTool, point2);

	TopoDS_Edge hubSide1Edge = BRepBuilderAPI_MakeEdge(hubSide1Curve);
	TopoDS_Edge hubSide2Edge = BRepBuilderAPI_MakeEdge(hubSide2Curve);


	/* find the parameter values of first, last and blade center and firTree Center on bottom hubface curve*/
	Standard_Real parameterHubBottomCurveOnBladeCenter;
	GeomLib_Tool::Parameter(hubBottomFaceCurve, point3, 1e-7, parameterHubBottomCurveOnBladeCenter);

	Standard_Real hubSide1ParameterOnHubBottomFaceCurve;
	GeomLib_Tool::Parameter(hubBottomFaceCurve, point1, 1e-7, hubSide1ParameterOnHubBottomFaceCurve);

	Standard_Real hubSide2ParameterOnHubBottomFaceCurve;
	GeomLib_Tool::Parameter(hubBottomFaceCurve, point2, 1e-7, hubSide2ParameterOnHubBottomFaceCurve);

	gp_Pnt firTreeCenterLinePointOnHubBottomCurve = point3.Rotated(rotatingAxis, A3);
	Standard_Real parameterfirTreeCenterLinePointOnHubBottomCurve;
	GeomLib_Tool::Parameter(hubBottomFaceCurve, firTreeCenterLinePointOnHubBottomCurve, 1e-7, parameterfirTreeCenterLinePointOnHubBottomCurve);
	

	gp_Vec firTreeCenterToZAxisLine_Vector(firTreeCenterLinePointOnHubBottomCurve, rotatingAxisPointForSelectedPlane);
	gp_Dir firTreeCenterToZAxisLine_VectorDirection(firTreeCenterToZAxisLine_Vector);
	gp_Vec firTreeCenterToZAxisLine_UnitVector(firTreeCenterToZAxisLine_VectorDirection);
	gp_Ax1 firTreeCenterAxis(firTreeCenterLinePointOnHubBottomCurve, firTreeCenterToZAxisLine_Vector);


	/* find the neck start points on botton neck surface*/
	gp_Pnt nearToSide1_neckStartPoint = point3.Rotated(rotatingAxis, A3 - (A4/2));
	Standard_Real parameterNearToSide1_neckStartPoint;
	GeomLib_Tool::Parameter(hubBottomFaceCurve, nearToSide1_neckStartPoint, 1e-7, parameterNearToSide1_neckStartPoint);

	gp_Pnt farToside1_neckStartPoint = point3.Rotated(rotatingAxis, A3 + (A4 / 2));
	Standard_Real parameterFarToSide1_neckStartPoint;
	GeomLib_Tool::Parameter(hubBottomFaceCurve, farToside1_neckStartPoint, 1e-7, parameterFarToSide1_neckStartPoint);

	gp_Pnt neckStartPoint = nearToSide1_neckStartPoint;

	Handle(Geom_Curve) hubSide1HubBottomCurve = new Geom_TrimmedCurve(hubBottomFaceCurve, hubSide1ParameterOnHubBottomFaceCurve, parameterNearToSide1_neckStartPoint);
	Handle(Geom_Curve) hubSide2HubBottomCurve = new Geom_TrimmedCurve(hubBottomFaceCurve, parameterFarToSide1_neckStartPoint, hubSide2ParameterOnHubBottomFaceCurve);
	
	hubSide1HubBottomCurve = GeomConvert::CurveToBSplineCurve(hubSide1HubBottomCurve);
	hubSide2HubBottomCurve = GeomConvert::CurveToBSplineCurve(hubSide2HubBottomCurve);

	TopoDS_Edge hubSide1HubBottomEdge = BRepBuilderAPI_MakeEdge(hubSide1HubBottomCurve);
	TopoDS_Edge hubSide2HubBottomEdge = BRepBuilderAPI_MakeEdge(hubSide2HubBottomCurve);


	neckHeight = CenterLineLength * 0.3;

	gp_Trsf theTransformationNeckPoint;
	theTransformationNeckPoint.SetTranslation(firTreeCenterToZAxisLine_UnitVector.Multiplied(neckHeight));

	gp_Pnt neckEndPoint = neckStartPoint.Transformed(theTransformationNeckPoint);

	Handle(Geom_TrimmedCurve) neckLineCurve = GC_MakeSegment(neckStartPoint, neckEndPoint);

	TopoDS_Edge neckLineEdge = BRepBuilderAPI_MakeEdge(neckLineCurve);

	QVector<TopoDS_Edge> firstHalfFirTreeEdges;


	gp_Pnt lastLobeReleaf_LastPoint;
	gp_Pnt lobeLoadEdge_1_FirstPoint = neckEndPoint;
	TopoDS_Edge edgeBeforeLobeLoad = neckLineEdge;
	TopoDS_Edge lastLobeReleaf;

	removeDisplaiedAISShape();

	for (int i = 0; i < 2; i++)
	{
		LobeLoadLength = CenterLineLength * (0.6 - (0.15*i));

		gp_Trsf theTransformationLobeLoadEdge_1_Point;
		theTransformationLobeLoadEdge_1_Point.SetTranslation(firTreeCenterToZAxisLine_UnitVector.Multiplied(LobeLoadLength));

		gp_Pnt angleLastPointBefore = lobeLoadEdge_1_FirstPoint.Transformed(theTransformationLobeLoadEdge_1_Point);

		gp_Dir zDir(0, 0, 1);
		gp_Ax1 neckEndPointAxis(lobeLoadEdge_1_FirstPoint, zDir);

		gp_Pnt LobeLoad_LastPoint = angleLastPointBefore.Rotated(neckEndPointAxis, lobeLodeAngle);

		Handle(Geom_TrimmedCurve) LobeLoad_Curve = GC_MakeSegment(lobeLoadEdge_1_FirstPoint, LobeLoad_LastPoint);

		TopoDS_Edge lobeLoad_Edge = BRepBuilderAPI_MakeEdge(LobeLoad_Curve);

		TopoDS_Edge convexFilletEdge = make2dFillet(edgeBeforeLobeLoad, lobeLoad_Edge, lobeLoadEdge_1_FirstPoint, convexRadius, rotatingAxis3);
		

		if (i == 0)
		{
			neckLineEdge = edgeBeforeLobeLoad;
		}
		else
		{
			firstHalfFirTreeEdges.insert(0, edgeBeforeLobeLoad);
		}

		LobeThickness = CenterLineLength * 0.1;

		gp_Trsf theTransformationLobeReleaf_Point;
		theTransformationLobeReleaf_Point.SetTranslation(firTreeCenterToZAxisLine_UnitVector.Multiplied(LobeThickness));

		gp_Pnt lobeReleaf_LastPoint = lobeLoadEdge_1_FirstPoint.Transformed(theTransformationLobeReleaf_Point);

		Handle(Geom_TrimmedCurve) lobeReleaf_Curve = GC_MakeSegment(LobeLoad_LastPoint, lobeReleaf_LastPoint);

		TopoDS_Edge lobeReleaf_Edge = BRepBuilderAPI_MakeEdge(lobeReleaf_Curve);


		TopoDS_Edge concaveFilletEdge = make2dFillet(lobeLoad_Edge, lobeReleaf_Edge, LobeLoad_LastPoint, concaveRadius, rotatingAxis3);

		if (!convexFilletEdge.IsNull())
		{
			firstHalfFirTreeEdges.insert(0, convexFilletEdge);
		}
		
		firstHalfFirTreeEdges.insert(0, lobeLoad_Edge);
		firstHalfFirTreeEdges.insert(0, concaveFilletEdge);

		lastLobeReleaf_LastPoint = lobeReleaf_LastPoint;
		lobeLoadEdge_1_FirstPoint = lastLobeReleaf_LastPoint;
		edgeBeforeLobeLoad = lobeReleaf_Edge;

	}


	Standard_Real firTreeHeight = CenterLineLength * 0.7;

	gp_Trsf theTransformationMidBottomPoint;
	theTransformationMidBottomPoint.SetTranslation(firTreeCenterToZAxisLine_UnitVector.Multiplied(firTreeHeight));

	gp_Pnt MidBottomFirTreetPoint = firTreeCenterLinePointOnHubBottomCurve.Transformed(theTransformationMidBottomPoint);
		
	Handle(Geom_TrimmedCurve) centerConnectCurve = GC_MakeSegment(lastLobeReleaf_LastPoint, MidBottomFirTreetPoint);

	TopoDS_Edge centerConnectCurveEdge = BRepBuilderAPI_MakeEdge(centerConnectCurve);
	
	TopoDS_Edge firTreeBottomFillet = make2dFillet(edgeBeforeLobeLoad, centerConnectCurveEdge, lastLobeReleaf_LastPoint, firTreebottomFilletRadius, rotatingAxis3);

	firstHalfFirTreeEdges.insert(0, edgeBeforeLobeLoad);
	firstHalfFirTreeEdges.insert(0, firTreeBottomFillet);
	firstHalfFirTreeEdges.insert(0, centerConnectCurveEdge);


	BRepBuilderAPI_MakeWire wireMaker1;

	for (int i = 0; i < firstHalfFirTreeEdges.length();i++)
	{
		wireMaker1.Add(firstHalfFirTreeEdges.at(i));
	}
	
	TopoDS_Wire halfFirTreeWireProfile = wireMaker1.Wire();

	gp_Trsf aTrsf;
	aTrsf.SetMirror(firTreeCenterAxis);
	BRepBuilderAPI_Transform aBRepTrsf(halfFirTreeWireProfile, aTrsf);
	TopoDS_Shape aMirroredShape = aBRepTrsf.Shape();
	TopoDS_Wire aMirroredWire = TopoDS::Wire(aMirroredShape);

	BRepBuilderAPI_MakeWire mkWire(aMirroredWire);
	mkWire.Add(halfFirTreeWireProfile);
	TopoDS_Wire WireProfile = mkWire.Wire();

	gp_Trsf neckLineTransfer;
	neckLineTransfer.SetMirror(firTreeCenterAxis);
	BRepBuilderAPI_Transform neckLine_BRepTrsf(neckLineEdge, neckLineTransfer);
	TopoDS_Shape MirroredNeckLineShape = neckLine_BRepTrsf.Shape();

	TopoDS_Edge MirroredNeckLineEdge = TopoDS::Edge(MirroredNeckLineShape);


	TopoDS_Edge firTreeUpperFillet_side1 = make2dFillet(hubSide1HubBottomEdge, neckLineEdge, nearToSide1_neckStartPoint, firTreeupperFilletRadius, rotatingAxis3);
	TopoDS_Edge firTreeUpperFillet_side2 = make2dFillet(MirroredNeckLineEdge, hubSide2HubBottomEdge, farToside1_neckStartPoint, firTreeupperFilletRadius, rotatingAxis3);
	

	BRepBuilderAPI_MakeWire cutingToolWireMaker(WireProfile);
	cutingToolWireMaker.Add(neckLineEdge);
	cutingToolWireMaker.Add(MirroredNeckLineEdge);
	cutingToolWireMaker.Add(firTreeUpperFillet_side1);
	cutingToolWireMaker.Add(firTreeUpperFillet_side2);
	cutingToolWireMaker.Add(hubSide1HubBottomEdge);
	cutingToolWireMaker.Add(hubSide2HubBottomEdge);
	cutingToolWireMaker.Add(hubSide1Edge);
	cutingToolWireMaker.Add(hubSide2Edge);
	cutingToolWireMaker.Add(hubFaceEdge1_CuttingTool);
	cutingToolWireMaker.Add(hubFaceEdge2_CuttingTool);
	cutingToolWireMaker.Add(upperConnecter1_Edge);
	cutingToolWireMaker.Add(upperConnecter2_Edge);
	cutingToolWireMaker.Add(lastParameterSide_cuttingToolEdge);
	cutingToolWireMaker.Add(firstParameterSide_cuttingToolEdge);
	cutingToolWireMaker.Add(cuttingTool_BottomEdge);
	
	TopoDS_Wire cuttingToolWire = cutingToolWireMaker.Wire();

	TopoDS_Face firTreeCuttingToolFaceProfile = BRepBuilderAPI_MakeFace(cuttingToolWire);

	gp_Vec depthFromSelectedFace_PrismVec(0, 0, -firTreeDepthFrom_SelectedFace);
	Standard_Real firTreeLength = (50 - firTreeDepthFrom_SelectedFace - firTreeDepthFrom_NextToSelectedFace);
	gp_Vec depthOfFirTree_PrismVec(0, 0, -(firTreeLength));
	gp_Vec depthFromNextToSelectedFace_PrismVec(0, 0, -firTreeDepthFrom_NextToSelectedFace);

	gp_Vec toleranceVector(0, 0, -5);

	Handle(Geom_TrimmedCurve) level1_hubBottomFaceCurve = GC_MakeArcOfCircle(point1, point3, point2);
	TopoDS_Edge level1_hubBottomFaceEdge = BRepBuilderAPI_MakeEdge(level1_hubBottomFaceCurve);

	BRepBuilderAPI_MakeWire depthCutingToolWireMaker(level1_hubBottomFaceEdge);
	depthCutingToolWireMaker.Add(hubSide1Edge);
	depthCutingToolWireMaker.Add(hubSide2Edge);
	depthCutingToolWireMaker.Add(hubFaceEdge1_CuttingTool);
	depthCutingToolWireMaker.Add(hubFaceEdge2_CuttingTool);
	depthCutingToolWireMaker.Add(upperConnecter1_Edge);
	depthCutingToolWireMaker.Add(upperConnecter2_Edge);
	depthCutingToolWireMaker.Add(lastParameterSide_cuttingToolEdge);
	depthCutingToolWireMaker.Add(firstParameterSide_cuttingToolEdge);
	depthCutingToolWireMaker.Add(cuttingTool_BottomEdge);

	TopoDS_Wire depthCuttingToolWire = depthCutingToolWireMaker.Wire();
	TopoDS_Face depthCuttingToolFaceProfile = BRepBuilderAPI_MakeFace(depthCuttingToolWire);

	gp_Trsf translate1;
	translate1.SetTranslation(depthFromSelectedFace_PrismVec - toleranceVector);
	BRepBuilderAPI_Transform translated(firTreeCuttingToolFaceProfile, translate1);
	TopoDS_Shape toolFaceProfileTranslatedShape = translated.Shape();

	gp_Vec Vec(0, 0, -(firTreeDepthFrom_SelectedFace + firTreeLength));
	gp_Trsf translate2;
	translate2.SetTranslation(Vec);
	BRepBuilderAPI_Transform translated2(depthCuttingToolFaceProfile, translate2);
	TopoDS_Shape depthCuttingToolFaceTransformedShape = translated2.Shape();
	

	TopoDS_Shape firTreeCuttingTool = BRepPrimAPI_MakePrism(toolFaceProfileTranslatedShape, depthOfFirTree_PrismVec + toleranceVector);
	TopoDS_Shape depthCuttingTool_FromSelectedFace = BRepPrimAPI_MakePrism(depthCuttingToolFaceProfile, depthFromSelectedFace_PrismVec);
	TopoDS_Shape depthCuttingTool_SelectedFace = BRepPrimAPI_MakePrism(depthCuttingToolFaceTransformedShape, depthFromNextToSelectedFace_PrismVec);
	
	shape = BRepAlgoAPI_Cut(shape, depthCuttingTool_FromSelectedFace);
	shape = BRepAlgoAPI_Cut(shape, depthCuttingTool_SelectedFace);
	shape = BRepAlgoAPI_Cut(shape, firTreeCuttingTool);

	displayASape(shape);

	int nMilliseconds = myTimer.elapsed();
	//BRepTools::Write(shape, "C:/Users/DELL/Desktop/Cut/Sample_2.Brep");

	
}

void occQt::setFilletMode()
{
	if (!edgeSelectModeEnabled)
	{
		myOccView->getContext()->ActivateStandardMode(TopAbs_EDGE);
		edgeSelectModeEnabled = true;
	}
	else
	{
		myOccView->getContext()->ActivateStandardMode(TopAbs_FACE);
		edgeSelectModeEnabled = false;
	}
}

void occQt::setFillet()
{
	/* get the imported blade shape to the parameter shape*/
	TopoDS_Shape shape;
	AIS_ListOfInteractive objList;
	myOccView->getContext()->DisplayedObjects(objList);
	AIS_ListIteratorOfListOfInteractive iter;
	for (iter.Initialize(objList); iter.More(); iter.Next())
	{
		Handle(AIS_InteractiveObject) aisShp = iter.Value();
		if (aisShp->IsKind("AIS_Shape"))
		{
			shape = Handle(AIS_Shape)::DownCast(aisShp)->Shape();
		}
	}

	//removeDisplaiedAISShape();
	//displayASape(shape);

	/* get the selected edge to the parameter selectedEdge*/
	TopoDS_Shape selectedShape;
	myOccView->getContext()->InitSelected();
	selectedShape = myOccView->getContext()->SelectedShape();
	TopoDS_Edge selectedEdge = TopoDS::Edge(selectedShape);

	//removeDisplaiedAISShape();
	//displayASape(selectedEdge);

	BRepFilletAPI_MakeFillet filletMaker(shape);

	Standard_Real radiuse = 0.5;

	filletMaker.Add(radiuse, selectedEdge);

	TopoDS_Shape shape2 = filletMaker.Shape();

	removeDisplaiedAISShape();
	displayASape(shape2);

}


TopoDS_Edge occQt::make2dFillet(TopoDS_Edge& e1, TopoDS_Edge& e2, gp_Pnt CommonPoint, Standard_Real r, gp_Ax3 thePlane)
{
	ChFi2d_FilletAPI fillet(e1, e2, thePlane);
//	ChFi2d_FilletAlgo fillet(e1, e2, thePlane);
	fillet.Perform(r);
	Standard_Integer j = fillet.NbResults(CommonPoint);
	TopoDS_Edge filletEdge;
	if (j == 1)
	{
		filletEdge = fillet.Result(CommonPoint, e1, e2);
	}
	return filletEdge;
}

TopoDS_Edge occQt::makeFilletsOrChamfers(TopoDS_Edge &edge1, TopoDS_Edge &edge2, gp_Pnt commonPoint, double filletRadius, gp_Pln plane, bool &isDone, bool isFillet)
{
	TopoDS_Edge fillet;
	isDone = false;
	ChFi2d_FilletAlgo filletAlgo;
	if (filletRadius>1e-10)
	{
		filletAlgo.Init(edge1, edge2, plane);
		filletAlgo.Perform(filletRadius);
		int results = filletAlgo.NbResults(commonPoint);
		TopoDS_Edge trimmedEdge1, trimmedEdge2;
		if (results == 1)
		{
			fillet = filletAlgo.Result(commonPoint, trimmedEdge1, trimmedEdge2);
			edge1 = trimmedEdge1;
			edge2 = trimmedEdge2;
			isDone = true;
		}
		else
		{
			//ccl::cmessage::logmessage("Failed to create blisk fillet");
			//throw new AxiProgressException();
		}
	}
	else
	{
		//ccl::cmessage::logmessage("Failed to create blisk fillet");
		//throw new AxiProgressException();
	}
	if (!isFillet && isDone) {
		gp_Pnt p1 = BRep_Tool::Pnt(TopExp::FirstVertex(fillet));
		gp_Pnt p2 = BRep_Tool::Pnt(TopExp::LastVertex(fillet));
		//Handle_Geom_Curve curve = makeBSplineCrvFromTwoPoints(p1, p2);
		//return BRepBuilderAPI_MakeEdge(curve);
	}
	else
		return fillet;
}

void occQt::make2dFilletMy(TopoDS_Edge e1, TopoDS_Edge e2)
{
	TopoDS_Edge edge1 = e1;
	TopoDS_Edge edge2 = e2;



}


void occQt::Checker()
{
	
	gp_Pnt a(1,13,0);
	gp_Pnt b(1, 10, 0);
	gp_Pnt c(10, 500, 0);

	TopoDS_Edge aEdge1 = BRepBuilderAPI_MakeEdge(a, b);
	TopoDS_Edge aEdge2 = BRepBuilderAPI_MakeEdge(a, c);



	gp_Dir zDir(0, 0, 1);
	gp_Ax2 rotatingAxis2(a, zDir);
	gp_Ax3 rotatingAxis3(rotatingAxis2);

	//TopoDS_Edge filet = make2dFillet(aEdge1, aEdge2, a, 0.005, rotatingAxis3);
	bool x;
	TopoDS_Edge filet = makeFilletsOrChamfers(aEdge1, aEdge2, a, 0.05, rotatingAxis3, x, true);
	

	//displayASape(aEdge1);
	//displayASape(filet);
	//displayASape(aEdge2);

	
	gp_Dir d(0,-1,0);
	gp_Vec vec(d);
	

	gp_Pnt lastLobeReleaf_LastPoint;
	gp_Pnt lobeLoadEdge_1_FirstPoint = b;
	TopoDS_Edge edgeBeforeLobeLoad = aEdge1;
	TopoDS_Edge lastLobeReleaf;

	for (int i = 0; i < 4; i++)
	{
		Standard_Real LobeLoadLength = (1 - (0.15*i));

		gp_Trsf theTransformationLobeLoadEdge_1_Point;
		//gp_Vec centerUnitVector(faceCenterDirection);
		theTransformationLobeLoadEdge_1_Point.SetTranslation(vec.Multiplied(LobeLoadLength));

		gp_Pnt angleLastPointBefore = lobeLoadEdge_1_FirstPoint.Transformed(theTransformationLobeLoadEdge_1_Point);

		//gp_Pnt aOrigin(0, 0, 0);
		gp_Dir zDir(0, 0, 1);
		gp_Ax1 neckEndPointAxis(lobeLoadEdge_1_FirstPoint, zDir);

		gp_Pnt LobeLoad_LastPoint = angleLastPointBefore.Rotated(neckEndPointAxis, ((3.141 * 2.5) / 6));

		Handle(Geom_TrimmedCurve) LobeLoad_Curve = GC_MakeSegment(lobeLoadEdge_1_FirstPoint, LobeLoad_LastPoint);

		
		TopoDS_Edge lobeLoad_Edge = BRepBuilderAPI_MakeEdge(lobeLoadEdge_1_FirstPoint, LobeLoad_LastPoint);

		//ChFi2d_FilletAPI convexFillet(edgeBeforeLobeLoad, lobeLoad_Edge, rotatingAxis3);
		//convexFillet.Perform(convexRadius);
		//Standard_Integer j = convexFillet.NbResults(lobeLoadEdge_1_FirstPoint);

		bool x;
		TopoDS_Edge convexFilletEdge = makeFilletsOrChamfers(edgeBeforeLobeLoad, lobeLoad_Edge, lobeLoadEdge_1_FirstPoint, 0.005, rotatingAxis3, x, true);
		//TopoDS_Edge convexFilletEdge = make2dFillet(edgeBeforeLobeLoad, lobeLoad_Edge, lobeLoadEdge_1_FirstPoint, 0.5, rotatingAxis3);


		if (i == 0)
		{
			//firstHalfFirTreeEdges.append(edgeBeforeLobeLoad);
			aEdge1 = edgeBeforeLobeLoad;
			displayASape(aEdge1);
		}
		else
		{
			displayASape(edgeBeforeLobeLoad);
		}


		//firstHalfFirTreeEdges.insert(0, lobeLoad_Edge);

		//removeDisplaiedAISShape();
		//displayASape(angle_1_Edge);

		Standard_Real LobeThickness = 1;

		gp_Trsf theTransformationLobeReleaf_Point;
		theTransformationLobeReleaf_Point.SetTranslation(vec.Multiplied(LobeThickness));

		gp_Pnt lobeReleaf_LastPoint = lobeLoadEdge_1_FirstPoint.Transformed(theTransformationLobeReleaf_Point);

		Handle(Geom_TrimmedCurve) lobeReleaf_Curve = GC_MakeSegment(LobeLoad_LastPoint, lobeReleaf_LastPoint);

		TopoDS_Edge lobeReleaf_Edge = BRepBuilderAPI_MakeEdge(LobeLoad_LastPoint, lobeReleaf_LastPoint);

		//firstHalfFirTreeEdges.insert(0, lobeReleaf_Edge);

		//removeDisplaiedAISShape();
		//displayASape(angle_2_Edge);


		TopoDS_Edge concaveFilletEdge = make2dFillet(lobeLoad_Edge, lobeReleaf_Edge, LobeLoad_LastPoint, 0.005, rotatingAxis3);

		if (!convexFilletEdge.IsNull())
		{
			//firstHalfFirTreeEdges.insert(0, convexFilletEdge);
			displayASape(convexFilletEdge);
		}

		//firstHalfFirTreeEdges.insert(0, lobeLoad_Edge);
		//firstHalfFirTreeEdges.insert(0, concaveFilletEdge);
		//firstHalfFirTreeEdges.insert(0, lobeReleaf_Edge);


		displayASape(lobeLoad_Edge);
		displayASape(concaveFilletEdge);
		//displayASape(lobeReleaf_Edge);


		lastLobeReleaf_LastPoint = lobeReleaf_LastPoint;
		lobeLoadEdge_1_FirstPoint = lastLobeReleaf_LastPoint;
		edgeBeforeLobeLoad = lobeReleaf_Edge;

	}
	
}

void occQt::setFirTree3()
{
	FirTreeCreator myFirTreeCreator(myOccView->getContext());
	TopoDS_Shape outputshape =  myFirTreeCreator.build();
	removeDisplaiedAISShape();
	displayASape(outputshape);
}





	
	/*

	QDialog * d = new QDialog();
	d->setWindowTitle("Enter Fir Tree Data");
	QVBoxLayout * vbox = new QVBoxLayout();

	QLabel * lableinterceptA = new QLabel("Intercept A  ");
	QDoubleSpinBox * spinBoxInterceptA = new QDoubleSpinBox();
	spinBoxInterceptA->setDecimals(5);
	spinBoxInterceptA->setSingleStep(0.00001);
	spinBoxInterceptA->setMaximum(std::numeric_limits<double>::infinity());
	QLabel * lableSlopB = new QLabel("Slop B  ");
	QDoubleSpinBox * spinBoxSlopB = new QDoubleSpinBox();
	spinBoxSlopB->setDecimals(5);
	spinBoxSlopB->setSingleStep(0.00001);
	spinBoxSlopB->setMaximum(std::numeric_limits<double>::infinity());
	QLabel * lableInterceptC = new QLabel("Intercept C  ");
	QDoubleSpinBox * spinBoxInterceptC = new QDoubleSpinBox();
	spinBoxInterceptC->setDecimals(5);
	spinBoxInterceptC->setSingleStep(0.00001);
	spinBoxInterceptC->setMaximum(std::numeric_limits<double>::infinity());
	QLabel * lableSlopD = new QLabel("Slop D  ");
	QDoubleSpinBox * spinBoxSlopD = new QDoubleSpinBox();
	spinBoxSlopD->setDecimals(5);
	spinBoxSlopD->setSingleStep(0.00001);
	spinBoxSlopD->setMaximum(std::numeric_limits<double>::infinity());
	QLabel * lableNeckE = new QLabel("Neck E  ");
	QDoubleSpinBox * spinBoxNeckE = new QDoubleSpinBox();
	spinBoxNeckE->setDecimals(5);
	spinBoxNeckE->setSingleStep(0.00001);
	spinBoxNeckE->setMaximum(std::numeric_limits<double>::infinity());
	QLabel * lableRadiusA1 = new QLabel("Radius A1  ");
	QDoubleSpinBox * spinBoxRadiusA1 = new QDoubleSpinBox();
	spinBoxRadiusA1->setDecimals(5);
	spinBoxRadiusA1->setSingleStep(0.00001);
	spinBoxRadiusA1->setMaximum(std::numeric_limits<double>::infinity());
	QLabel * lableRadiusB1 = new QLabel("Radius B1  ");
	QDoubleSpinBox * spinBoxRadiusB1 = new QDoubleSpinBox();
	spinBoxRadiusB1->setDecimals(5);
	spinBoxRadiusB1->setSingleStep(0.00001);
	spinBoxRadiusB1->setMaximum(std::numeric_limits<double>::infinity());
	QLabel * lableAngleC1 = new QLabel("Angle C1  ");
	QDoubleSpinBox * spinBoxAngleC1 = new QDoubleSpinBox();
	spinBoxAngleC1->setDecimals(5);
	spinBoxAngleC1->setSingleStep(0.00001);
	spinBoxAngleC1->setMaximum(std::numeric_limits<double>::infinity());
	QLabel * lableAngleD1 = new QLabel("Angle D1  ");
	QDoubleSpinBox * spinBoxAngleD1 = new QDoubleSpinBox();
	spinBoxAngleD1->setDecimals(5);
	spinBoxAngleD1->setSingleStep(0.00001);
	spinBoxAngleD1->setMaximum(std::numeric_limits<double>::infinity());
	QLabel * lableRadiusA2 = new QLabel("Radius A2  ");
	QDoubleSpinBox * spinBoxRadiusA2 = new QDoubleSpinBox();
	spinBoxRadiusA2->setDecimals(5);
	spinBoxRadiusA2->setSingleStep(0.00001);
	spinBoxRadiusA2->setMaximum(std::numeric_limits<double>::infinity());
	QLabel * lableRadiusB2 = new QLabel("Radius B2  ");
	QDoubleSpinBox * spinBoxRadiusB2 = new QDoubleSpinBox();
	spinBoxRadiusB2->setDecimals(5);
	spinBoxRadiusB2->setSingleStep(0.00001);
	spinBoxRadiusB2->setMaximum(std::numeric_limits<double>::infinity());
	QLabel * lableAngleC2 = new QLabel("Angle C2  ");
	QDoubleSpinBox * spinBoxAngleC2 = new QDoubleSpinBox();
	spinBoxAngleC2->setDecimals(5);
	spinBoxAngleC2->setSingleStep(0.00001);
	spinBoxAngleC2->setMaximum(std::numeric_limits<double>::infinity());
	QLabel * lableAngleD2 = new QLabel("Angle D2  ");
	QDoubleSpinBox * spinBoxAngleD2 = new QDoubleSpinBox();
	spinBoxAngleD2->setDecimals(5);
	spinBoxAngleD2->setSingleStep(0.00001);
	spinBoxAngleD2->setMaximum(std::numeric_limits<double>::infinity());
	QLabel * lableBacePosition = new QLabel("Base Position  ");
	QDoubleSpinBox * spinBoxBacePosition = new QDoubleSpinBox();
	spinBoxBacePosition->setDecimals(5);
	spinBoxBacePosition->setSingleStep(0.00001);
	spinBoxBacePosition->setMaximum(std::numeric_limits<double>::infinity());
	//QLineEdit * lineEditA = new QLineEdit();

	QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	QObject::connect(buttonBox, SIGNAL(accepted()), d, SLOT(accept()));
	QObject::connect(buttonBox, SIGNAL(rejected()), d, SLOT(reject()));

	vbox->addWidget(lableinterceptA);
	vbox->addWidget(lableSlopB);
	vbox->addWidget(lableInterceptC);
	vbox->addWidget(lableSlopD);
	vbox->addWidget(lableNeckE);
	vbox->addWidget(lableRadiusA1);
	vbox->addWidget(lableRadiusB1);
	vbox->addWidget(lableAngleC1);
	vbox->addWidget(lableAngleD1);
	vbox->addWidget(lableRadiusA2);
	vbox->addWidget(lableRadiusB2);
	vbox->addWidget(lableAngleC2);
	vbox->addWidget(lableAngleD2);
	vbox->addWidget(lableBacePosition);
	
	vbox->addWidget(buttonBox);

	d->setLayout(vbox);

	int result = d->exec();

	while (true)
	{
		if (result == QDialog::Accepted)
		{
			//boxLength = spinBoxLength->value();
			//boxWidth = spinBoxWidth->value();
			//boxHeight = spinBoxHeight->value();

		}
		else
		{
			return;
		}
	}

	*/

//}
