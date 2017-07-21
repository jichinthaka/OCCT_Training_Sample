#include "FirTreeCreator.h"

#include <AIS_ListOfInteractive.hxx>
#include <AIS_Shape.hxx>
#include <Standard_Handle.hxx>
#include <TopExp_Explorer.hxx>

#include <gp_Circ.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shell.hxx>

#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepAlgoAPI_Cut.hxx>
//#include <BRepAlgo_Cut.hxx>
#include <BRep_Tool.hxx>
#include <BRepGProp.hxx>
#include <BRepTools.hxx>

#include <GC_MakeSegment.hxx>
#include <GC_MakeArcOfCircle.hxx>

#include <Geom_Curve.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <GeomLProp_CLProps.hxx>
#include <GeomLib_Tool.hxx>
#include <GeomConvert.hxx>

#include <GProp_GProps.hxx>

#include <ChFi2d_FilletAPI.hxx>

#include <QMessageBox>


FirTreeCreator::FirTreeCreator(Handle_AIS_InteractiveContext context)
{
	myContext = context;
}

FirTreeCreator::~FirTreeCreator()
{
}

TopoDS_Shape FirTreeCreator::build()
{
	/* convert the angles in degrees to radian angle which are using in my fuctions*/
	/* hear, assume pi as 3.141 because otherwise it give error with cutting fuction.*/
	/* this assumption is not effected us, because it give a difference after 5th decimal point 
		when comparing with that if we use the real pi*/
	A1 = ((3.141 / 180)*A1InDegrees);
	A2 = ((3.141 / 180)*A2InDegrees);
	A3 = ((3.141 / 180)*A3InDegrees);
	A4 = ((3.141 / 180)*A4InDegrees);
	
	TopoDS_Shape givenBladeShape = getBlade();
	TopoDS_Shape solidBladeShape = getSolidBlade(givenBladeShape);

	TopoDS_Face selectedFace = getSelectedFace();

	QVector<TopoDS_Edge> curvedTopoDSEdges;
	QVector<TopoDS_Edge> straightTopoDSEdges;
	getFaceEdges(selectedFace, curvedTopoDSEdges, straightTopoDSEdges);

	TopoDS_Edge longerCurveEdge;
	TopoDS_Edge shoterCurveEdge;
	getCurveEdges(curvedTopoDSEdges, longerCurveEdge, shoterCurveEdge);

	gp_Pnt firstParameterPointOfLongerCurve;
	gp_Pnt lastParameterPointOfLongerCurve;
	gp_Pnt longerCurveCenterPoint;
	Standard_Real firstParameterOfLongerCurve;
	Standard_Real lastParameterOfLongerCurve;
	Standard_Real midParameterOflongerCurve;
	Handle(Geom_Curve) longerCurve;
	getEdgeProperties(longerCurveEdge, firstParameterPointOfLongerCurve, lastParameterPointOfLongerCurve, longerCurveCenterPoint,
		firstParameterOfLongerCurve, lastParameterOfLongerCurve, midParameterOflongerCurve, longerCurve);

	gp_Pnt firstParameterPointOfShoterCurve;	//not use again
	gp_Pnt lastParameterPointOfShoterCurve;		//not use again
	gp_Pnt shoterCurveCenterPoint;		//use only for get the face center line length
	Standard_Real firstParameterOfShoterCurve;	//not use again
	Standard_Real lastParameterOfShoterCurve;	//not use again
	Standard_Real midParameterOfShoterCurve;	//not use again
	Handle(Geom_Curve) shoterCurve;				//not use again
	getEdgeProperties(shoterCurveEdge, firstParameterPointOfShoterCurve, lastParameterPointOfShoterCurve, shoterCurveCenterPoint,
		firstParameterOfShoterCurve, lastParameterOfShoterCurve, midParameterOfShoterCurve, shoterCurve);

	bladeAxisPointOnHubSerface = longerCurveCenterPoint;  // just for hard code because don't know the blade center

	setRotatingAxiProperties(bladeAxisPointOnHubSerface);

	/* use this center line length to get dimention of fire tree as a frantion (for hard coddings) and
		to get it as the side line length of given shape to creat the cutting tool face side line*/
	Standard_Real CenterLineLength = longerCurveCenterPoint.Distance(shoterCurveCenterPoint);
	givenPlatFormHight = CenterLineLength;	//Assuption only

	/* get the blade axis line vector from longerCurve to rotating point as bladeCenterLineVector*/
	badeAxisLineVector = gp_Vec(bladeAxisPointOnHubSerface, rotatingAxisPointForSelectedPlane);

	gp_Pnt firstParameterSidePoint_CuttingToolOutterWire;
	gp_Pnt lastParameterSidePoint_CuttingToolOutterWire;
	TopoDS_Wire cuttingToolOutterWire = create_CuttingToolOutterWire(firstParameterPointOfLongerCurve, lastParameterPointOfLongerCurve,
		firstParameterOfLongerCurve, lastParameterOfLongerCurve, midParameterOflongerCurve,
		longerCurve, givenPlatFormHight, firstParameterSidePoint_CuttingToolOutterWire, lastParameterSidePoint_CuttingToolOutterWire);

	gp_Pnt firstParameterSideLongCurveCuttingEdgePoint;
	gp_Pnt lastParameterSideLongCurveCuttingEdgePoint;
	TopoDS_Wire cuttingToolOutterWire_WithHubAngles = create_CuttingToolOutterWire_WithHubAngles(cuttingToolOutterWire, longerCurve,
		firstParameterSidePoint_CuttingToolOutterWire, lastParameterSidePoint_CuttingToolOutterWire,
		firstParameterSideLongCurveCuttingEdgePoint, lastParameterSideLongCurveCuttingEdgePoint);
	
	gp_Pnt point1;
	gp_Pnt point2;
	TopoDS_Wire CuttingToolWire_WithHubThickness = create_cuttingToolWire_WithHubThickness(cuttingToolOutterWire_WithHubAngles,
		firstParameterSideLongCurveCuttingEdgePoint, lastParameterSideLongCurveCuttingEdgePoint, point1, point2);
	
	/* creat a deapth cutting tool face*/
	TopoDS_Wire DepthCuttingToolFaceWire = create_DepthCuttingToolFaceWire(CuttingToolWire_WithHubThickness, point1, point2);
	BRepTools::Write(DepthCuttingToolFaceWire, "C:/Users/DELL/Desktop/Cut/DepthCuttingToolFaceWire.Brep");

	TopoDS_Wire firTreeCuttingToolFaceWire = create_firTree2_CuttingToolFaceWire(CuttingToolWire_WithHubThickness, point1, point2);

	TopoDS_Shape CuttingToolSolid_1 = create_CuttingToolSolid(DepthCuttingToolFaceWire, firTreeDepthFrom_SelectedFace, 0, 0);
	//TopoDS_Solid CuttingToolSolid_1_solid = BRepBuilderAPI_MakeSolid(CuttingToolSolid_1);
	TopoDS_Shape CuttingToolSolid_2 = create_CuttingToolSolid(firTreeCuttingToolFaceWire, givenPlatFormThickness - firTreeDepthFrom_NextToSelectedFace - firTreeDepthFrom_SelectedFace, firTreeDepthFrom_SelectedFace, 0);
	//TopoDS_Solid CuttingToolSolid_2_solid = TopoDS::Solid(CuttingToolSolid_2);
	TopoDS_Shape CuttingToolSolid_3 = create_CuttingToolSolid(DepthCuttingToolFaceWire, firTreeDepthFrom_NextToSelectedFace, givenPlatFormThickness - firTreeDepthFrom_NextToSelectedFace, 0);
	//TopoDS_Solid CuttingToolSolid_3_solid = TopoDS::Solid(CuttingToolSolid_3);

	//TopoDS_Face myFaceProfile = BRepBuilderAPI_MakeFace(firTreeCuttingToolFaceWire);

	BRepTools::Write(solidBladeShape, "C:/Users/DELL/Desktop/Cut/solid.Brep");
	BRepTools::Write(CuttingToolSolid_1, "C:/Users/DELL/Desktop/Cut/CuttingToolSolid_1.Brep");
	/*
	TopoDS_Solid solidBlade = TopoDS::Solid(solidBladeShape);
	TopoDS_Shape shape = BRepAlgoAPI_Cut(solidBlade, CuttingToolSolid_1_solid);

	TopoDS_Solid solidshape = TopoDS::Solid(shape);
	shape = BRepAlgoAPI_Cut(solidshape, CuttingToolSolid_2_solid);

	solidshape = TopoDS::Solid(shape);
	shape = BRepAlgoAPI_Cut(solidshape, CuttingToolSolid_3_solid);
	*/
	
	TopoDS_Shape shape;
	TopTools_ListOfShape aLS;
	aLS.Append(solidBladeShape);

	TopTools_ListOfShape aLT;
	aLT.Append(CuttingToolSolid_1);
	aLT.Append(CuttingToolSolid_2);
	aLT.Append(CuttingToolSolid_3);

	BooleanCutTwoShapes(aLS,  aLT, shape);
	
	
	BRepTools::Write(CuttingToolSolid_2, "C:/Users/DELL/Desktop/Cut/CuttingToolSolid_2.Brep");
	BRepTools::Write(shape, "C:/Users/DELL/Desktop/Cut/shape.Brep");

	removeDisplaiedAISShape();
	displayASape(shape);

	return shape;
}

/*Get the boolean cut of given two shapes*/

bool FirTreeCreator::BooleanCutTwoShapes(TopTools_ListOfShape aLS, TopTools_ListOfShape aLT, TopoDS_Shape &cutShape)
{
	Standard_Boolean bRunParallel;
	Standard_Integer iErr;
	Standard_Real aFuzzyValue;
	BRepAlgoAPI_Cut aBuilder;

	bRunParallel = Standard_True;
	aFuzzyValue = 2.1e-5;

	// set the arguments  
	aBuilder.SetArguments(aLS);
	aBuilder.SetTools(aLT);
	//    
	// set parallel processing mode 
	// if  bRunParallel= Standard_True :  the parallel processing is switched on
	// if  bRunParallel= Standard_False :  the parallel processing is switched off
	aBuilder.SetRunParallel(bRunParallel);
	
	// set Fuzzy value
	// if aFuzzyValue=0.: the Fuzzy option is off
	//  if aFuzzyValue>0.: the Fuzzy option is on
	aBuilder.SetFuzzyValue(aFuzzyValue);
	
	// run the algorithm 
	aBuilder.Build();
	iErr = aBuilder.ErrorStatus();
	if (iErr) {
		// an error treatment
		return false;
	}
	cutShape = aBuilder.Shape();
	return true;
}


TopoDS_Shape FirTreeCreator::create_CuttingToolSolid(TopoDS_Wire faceWireCuttingTool, Standard_Real cuttingToolDeapth, 
	Standard_Real depthToStart, Standard_Real cuttingToolDeapthTolerance)
{
	TopoDS_Face faceCuttingTool = BRepBuilderAPI_MakeFace(faceWireCuttingTool);
	if(depthToStart!=0)
	{
	gp_Vec Vec(0, 0, -(depthToStart));
	gp_Trsf translate2;
	translate2.SetTranslation(Vec);
	BRepBuilderAPI_Transform translated2(faceCuttingTool, translate2);
	TopoDS_Shape faceCuttingToolAfterTranceform = translated2.Shape();
	faceCuttingTool = TopoDS::Face(faceCuttingToolAfterTranceform);
	}
	
	gp_Vec depthFromSelectedFace_PrismVec(0, 0, -(cuttingToolDeapth + cuttingToolDeapthTolerance));

	TopoDS_Shape CuttingTool = BRepPrimAPI_MakePrism(faceCuttingTool, depthFromSelectedFace_PrismVec);

	return CuttingTool;
}


/* 2D fillet making is in seperatly with this function for firTree creation because ChFi2d_FilletAPI does not work 
	for some conditions. If so, we can change the 2D filleting way hear*/
TopoDS_Edge FirTreeCreator::make2dFillet(TopoDS_Edge& e1, TopoDS_Edge& e2, gp_Pnt CommonPoint, Standard_Real r, gp_Ax3 thePlane)
{
	ChFi2d_FilletAPI fillet(e1, e2, thePlane);
	fillet.Perform(r);
	Standard_Integer j = fillet.NbResults(CommonPoint);
	TopoDS_Edge filletEdge;
	if (j == 1)
	{
		filletEdge = fillet.Result(CommonPoint, e1, e2);
	}
	return filletEdge;
}

/*In this firTree creation function, mesure neck thickness, lobe thicknesses along the firTree center line and we can
	set the lobe width from firTree center line*/
TopoDS_Wire FirTreeCreator::create_firTree2_CuttingToolFaceWire(TopoDS_Wire CuttingToolWire_WithHubThickness, gp_Pnt point1, gp_Pnt point2)
{
	gp_Dir bladeCenterLine_Direction(badeAxisLineVector);
	gp_Vec bladeCenterLine_UnitVector(bladeCenterLine_Direction);

	gp_Trsf theTransformation_bladeCenterPoint;
	theTransformation_bladeCenterPoint.SetTranslation(bladeCenterLine_UnitVector.Multiplied(hubThickness));
	gp_Pnt point3 = bladeAxisPointOnHubSerface.Transformed(theTransformation_bladeCenterPoint);

	Handle(Geom_TrimmedCurve) hubBottomFaceCurve = GC_MakeArcOfCircle(point1, point3, point2);

	/* find the parameter values of first, last and firTree Center on bottom hubface curve*/
	Standard_Real hubSide1ParameterOnHubBottomFaceCurve;
	GeomLib_Tool::Parameter(hubBottomFaceCurve, point1, 1e-7, hubSide1ParameterOnHubBottomFaceCurve);

	Standard_Real hubSide2ParameterOnHubBottomFaceCurve;
	GeomLib_Tool::Parameter(hubBottomFaceCurve, point2, 1e-7, hubSide2ParameterOnHubBottomFaceCurve);

	gp_Pnt firTreeCenterLinePointOnHubBottomCurve = point3.Rotated(rotatingAxis, A3);

	gp_Vec firTreeCenterToZAxisLine_Vector(firTreeCenterLinePointOnHubBottomCurve, rotatingAxisPointForSelectedPlane);
	gp_Dir firTreeCenterToZAxisLine_VectorDirection(firTreeCenterToZAxisLine_Vector);
	gp_Vec firTreeCenterToZAxisLine_UnitVector(firTreeCenterToZAxisLine_VectorDirection);
	gp_Ax1 firTreeCenterAxis(firTreeCenterLinePointOnHubBottomCurve, firTreeCenterToZAxisLine_Vector);

	/* find the neck start points on botton neck surface*/
	gp_Pnt nearToSide1_neckStartPoint = point3.Rotated(rotatingAxis, A3 - (A4 / 2));
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

	neckHeight = givenPlatFormHight * 0.3; // assume the neck height as a fraction of the given platform height, just for hard cordding

	gp_Trsf theTransformationNeckPoint;
	theTransformationNeckPoint.SetTranslation(firTreeCenterToZAxisLine_UnitVector.Multiplied(neckHeight));
	gp_Pnt neckEndPointOnCenterAxis = firTreeCenterLinePointOnHubBottomCurve.Transformed(theTransformationNeckPoint);

	bottomNeckWidth = givenPlatFormHight * 0.1;	// assume the bottomNeckWidth as a fraction of the given platform height, just for hard cordding

	gp_Trsf theTransformationNeckEndPoint;
	theTransformationNeckEndPoint.SetTranslation(firTreeCenterToZAxisLine_UnitVector.Multiplied(bottomNeckWidth));
	gp_Pnt neckEndPointBeforeRotate = neckEndPointOnCenterAxis.Transformed(theTransformationNeckEndPoint);

	gp_Ax1 neckEndPointAxisOnCenterAxis(neckEndPointOnCenterAxis, gp::DZ());
	gp_Pnt neckEndPoint = neckEndPointBeforeRotate.Rotated(neckEndPointAxisOnCenterAxis, M_PI_2);

	Handle(Geom_TrimmedCurve) neckLineCurve = GC_MakeSegment(neckStartPoint, neckEndPoint);

	TopoDS_Edge neckLineEdge = BRepBuilderAPI_MakeEdge(neckLineCurve);

	QVector<TopoDS_Edge> firstHalfFirTreeEdges;

	gp_Pnt lastLobeReleaf_LastPoint;
	gp_Pnt lobeLoadEdge_1_FirstPointOnCenterAxis = neckEndPointOnCenterAxis;
	gp_Pnt lobeLoadEdge_1_FirstPoint = neckEndPoint;
	TopoDS_Edge edgeBeforeLobeLoad = neckLineEdge;
	TopoDS_Edge lastLobeReleaf;

	for (int i = 0; i < (numberOfLobes); i++)
	{
		// assume the lobe Load Length as a fraction of the given platform height, just for hard cordding
		Standard_Real lobeLoadLength = givenPlatFormHight * (0.55 - (0.15*i));

		//*********don't delete following commented line. In real, we use that instead of above one.
		//Standard_Real lobeLoadLength = lobeLoadLengths.value(i);

		gp_Trsf theTransformationLobeLoadEdge_1_Point;
		theTransformationLobeLoadEdge_1_Point.SetTranslation(firTreeCenterToZAxisLine_UnitVector.Multiplied(lobeLoadLength));

		gp_Pnt angleLastPointBefore = lobeLoadEdge_1_FirstPoint.Transformed(theTransformationLobeLoadEdge_1_Point);

		gp_Ax1 neckEndPointAxis(lobeLoadEdge_1_FirstPoint, gp::DZ());

		// assume the lobe Load Angle as following, just for hard cordding
		Standard_Real lobeLodeAngle = (3.141 * 2.5) / 6;

		//*********don't delete following commented line. In real, we use that instead of above one.
		//Standard_Real lobeLodeAngle = lobeLodeAngles.value(i);

		gp_Pnt LobeLoad_LastPoint = angleLastPointBefore.Rotated(neckEndPointAxis, lobeLodeAngle);

		Handle(Geom_TrimmedCurve) LobeLoad_Curve = GC_MakeSegment(lobeLoadEdge_1_FirstPoint, LobeLoad_LastPoint);

		TopoDS_Edge lobeLoad_Edge = BRepBuilderAPI_MakeEdge(LobeLoad_Curve);

		// assume the convex Radius as following, just for hard cordding
		Standard_Real convexRadius = 1.0;

		//*********don't delete following commented line. In real, we use that instead of above one.
		//Standard_Real convexRadius = convexRadius.value(i);

		TopoDS_Edge convexFilletEdge = make2dFillet(edgeBeforeLobeLoad, lobeLoad_Edge, lobeLoadEdge_1_FirstPoint, convexRadius, rotatingAxis3);


		if (i == 0)
		{
			neckLineEdge = edgeBeforeLobeLoad;
			BRepTools::Write(neckLineEdge, "C:/Users/DELL/Desktop/neck.brep");
		}
		else
		{
			firstHalfFirTreeEdges.insert(0, edgeBeforeLobeLoad);
			BRepTools::Write(edgeBeforeLobeLoad, "C:/Users/DELL/Desktop/edgeBeforeLobeLoad.brep");
		}

		// assume the Lobe Thickness as a fraction of the given platform height, just for hard cordding
		Standard_Real lobeThickness = givenPlatFormHight * 0.2;

		//*********don't delete following commented line. In real, we use that instead of above one.
		//Standard_Real lobeThickness = lobeThicknesses.value(i);

		gp_Trsf theTransformationLobeReleaf_PointOnCenterAxis;
		theTransformationLobeReleaf_PointOnCenterAxis.SetTranslation(firTreeCenterToZAxisLine_UnitVector.Multiplied(lobeThickness));
		gp_Pnt lobeReleaf_LastPointOnCenterAxis = lobeLoadEdge_1_FirstPointOnCenterAxis.Transformed(theTransformationLobeReleaf_PointOnCenterAxis);

		// assume the Lobe releafBottomWidth as a fraction of the given platform height, just for hard cordding
		Standard_Real releafBottomWidth = givenPlatFormHight * (0.11-(0.01*i));

		//*********don't delete following commented line. In real, we use that instead of above one.
		//Standard_Real releafBottomWidth = releafBottomWidths.value(i);

		gp_Trsf theTransformationFobeReleaf_LastPoint;
		theTransformationFobeReleaf_LastPoint.SetTranslation(firTreeCenterToZAxisLine_UnitVector.Multiplied(releafBottomWidth));
		gp_Pnt lobeReleaf_LastPointBeforeRotate = lobeReleaf_LastPointOnCenterAxis.Transformed(theTransformationFobeReleaf_LastPoint);

		gp_Ax1 lobeReleaf_LastPointAxisOnCenterAxis(lobeReleaf_LastPointOnCenterAxis, gp::DZ());
		gp_Pnt lobeReleaf_LastPoint = lobeReleaf_LastPointBeforeRotate.Rotated(lobeReleaf_LastPointAxisOnCenterAxis, M_PI_2);

		Handle(Geom_TrimmedCurve) lobeReleaf_Curve = GC_MakeSegment(LobeLoad_LastPoint, lobeReleaf_LastPoint);

		TopoDS_Edge lobeReleaf_Edge = BRepBuilderAPI_MakeEdge(lobeReleaf_Curve);

		// assume the concave Radius as following, just for hard cordding
		Standard_Real concaveRadius = 1.0;

		//*********don't delete following commented line. In real, we use that instead of above one.
		//Standard_Real concaveRadius = concaveRadius.value(i);

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
		lobeLoadEdge_1_FirstPointOnCenterAxis = lobeReleaf_LastPointOnCenterAxis;

	}

	gp_Pnt MidBottomFirTreetPoint = lobeLoadEdge_1_FirstPointOnCenterAxis;

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
	cutingToolWireMaker.Add(MirroredNeckLineEdge);
	cutingToolWireMaker.Add(firTreeUpperFillet_side2);
	cutingToolWireMaker.Add(hubSide2HubBottomEdge);
	cutingToolWireMaker.Add(CuttingToolWire_WithHubThickness);
	cutingToolWireMaker.Add(hubSide1HubBottomEdge);
	cutingToolWireMaker.Add(firTreeUpperFillet_side1);
	cutingToolWireMaker.Add(neckLineEdge);

	TopoDS_Wire firTreeCuttingToolFaceWire = cutingToolWireMaker.Wire();
	return firTreeCuttingToolFaceWire;
}


TopoDS_Wire FirTreeCreator::create_firTree1_CuttingToolFaceWire(TopoDS_Wire CuttingToolWire_WithHubThickness, gp_Pnt point1, gp_Pnt point2)
{
	gp_Dir bladeCenterLine_Direction(badeAxisLineVector);
	gp_Vec bladeCenterLine_UnitVector(bladeCenterLine_Direction);

	gp_Trsf theTransformation_bladeCenterPoint;
	theTransformation_bladeCenterPoint.SetTranslation(bladeCenterLine_UnitVector.Multiplied(hubThickness));
	gp_Pnt point3 = bladeAxisPointOnHubSerface.Transformed(theTransformation_bladeCenterPoint);

	Handle(Geom_TrimmedCurve) hubBottomFaceCurve = GC_MakeArcOfCircle(point1, point3, point2);

	/* find the parameter values of first, last and firTree Center on bottom hubface curve*/
	Standard_Real hubSide1ParameterOnHubBottomFaceCurve;
	GeomLib_Tool::Parameter(hubBottomFaceCurve, point1, 1e-7, hubSide1ParameterOnHubBottomFaceCurve);

	Standard_Real hubSide2ParameterOnHubBottomFaceCurve;
	GeomLib_Tool::Parameter(hubBottomFaceCurve, point2, 1e-7, hubSide2ParameterOnHubBottomFaceCurve);

	gp_Pnt firTreeCenterLinePointOnHubBottomCurve = point3.Rotated(rotatingAxis, A3);

	gp_Vec firTreeCenterToZAxisLine_Vector(firTreeCenterLinePointOnHubBottomCurve, rotatingAxisPointForSelectedPlane);
	gp_Dir firTreeCenterToZAxisLine_VectorDirection(firTreeCenterToZAxisLine_Vector);
	gp_Vec firTreeCenterToZAxisLine_UnitVector(firTreeCenterToZAxisLine_VectorDirection);
	gp_Ax1 firTreeCenterAxis(firTreeCenterLinePointOnHubBottomCurve, firTreeCenterToZAxisLine_Vector);

	/* find the neck start points on botton neck surface*/
	gp_Pnt nearToSide1_neckStartPoint = point3.Rotated(rotatingAxis, A3 - (A4 / 2));
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

	neckHeight = givenPlatFormHight * 0.3; // assume the neck height as a fraction of the given platform height, just for hard cordding

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

	for (int i = 0; i < (numberOfLobes); i++)
	{
		// assume the lobe Load Length as a fraction of the given platform height, just for hard cordding
		Standard_Real lobeLoadLength = givenPlatFormHight * (0.6 - (0.15*i));

		//*********don't delete following commented line. In real, we use that instead of above one.
		//Standard_Real lobeLoadLength = lobeLoadLengths.value(i);

		gp_Trsf theTransformationLobeLoadEdge_1_Point;
		theTransformationLobeLoadEdge_1_Point.SetTranslation(firTreeCenterToZAxisLine_UnitVector.Multiplied(lobeLoadLength));

		gp_Pnt angleLastPointBefore = lobeLoadEdge_1_FirstPoint.Transformed(theTransformationLobeLoadEdge_1_Point);

		gp_Ax1 neckEndPointAxis(lobeLoadEdge_1_FirstPoint, gp::DZ());

		// assume the lobe Load Angle as following, just for hard cordding
		Standard_Real lobeLodeAngle = (3.141 * 2.5) / 6;

		//*********don't delete following commented line. In real, we use that instead of above one.
		//Standard_Real lobeLodeAngle = lobeLodeAngles.value(i);

		gp_Pnt LobeLoad_LastPoint = angleLastPointBefore.Rotated(neckEndPointAxis, lobeLodeAngle);

		Handle(Geom_TrimmedCurve) LobeLoad_Curve = GC_MakeSegment(lobeLoadEdge_1_FirstPoint, LobeLoad_LastPoint);

		TopoDS_Edge lobeLoad_Edge = BRepBuilderAPI_MakeEdge(LobeLoad_Curve);

		// assume the convex Radius as following, just for hard cordding
		Standard_Real convexRadius = 1.8;

		//*********don't delete following commented line. In real, we use that instead of above one.
		//Standard_Real convexRadius = convexRadius.value(i);

		TopoDS_Edge convexFilletEdge = make2dFillet(edgeBeforeLobeLoad, lobeLoad_Edge, lobeLoadEdge_1_FirstPoint, convexRadius, rotatingAxis3);


		if (i == 0)
		{
			neckLineEdge = edgeBeforeLobeLoad;
		}
		else
		{
			firstHalfFirTreeEdges.insert(0, edgeBeforeLobeLoad);
		}

		// assume the Lobe Thickness as a fraction of the given platform height, just for hard cordding
		Standard_Real lobeThickness = givenPlatFormHight * 0.1;

		//*********don't delete following commented line. In real, we use that instead of above one.
		//Standard_Real lobeThickness = lobeThicknesses.value(i);

		gp_Trsf theTransformationLobeReleaf_Point;
		theTransformationLobeReleaf_Point.SetTranslation(firTreeCenterToZAxisLine_UnitVector.Multiplied(lobeThickness));

		gp_Pnt lobeReleaf_LastPoint = lobeLoadEdge_1_FirstPoint.Transformed(theTransformationLobeReleaf_Point);

		Handle(Geom_TrimmedCurve) lobeReleaf_Curve = GC_MakeSegment(LobeLoad_LastPoint, lobeReleaf_LastPoint);

		TopoDS_Edge lobeReleaf_Edge = BRepBuilderAPI_MakeEdge(lobeReleaf_Curve);

		// assume the concave Radius as following, just for hard cordding
		Standard_Real concaveRadius = 1.0;

		//*********don't delete following commented line. In real, we use that instead of above one.
		//Standard_Real concaveRadius = concaveRadius.value(i);

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

	// firTreeHeight according to the above assumptions (lobeThickness, numberOfLobes and neckHeight)
	Standard_Real firTreeHeight = neckHeight + ((givenPlatFormHight * 0.1)*2);

	//*********don't delete following commented lines. In real, we use that instead of above one.

	
	//Standard_Real sumOfLobeThicknesses = 0;
	//for (int j = 0; j < lobeThicknesses.length();j++)
	//{
	//	sumOfLobeThicknesses += lobeThicknesses.value(j);
	//}
	//Standard_Real firTreeHeight = neckHeight + (sumOfLobeThicknesses);
	

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
	cutingToolWireMaker.Add(MirroredNeckLineEdge);
	cutingToolWireMaker.Add(firTreeUpperFillet_side2);
	cutingToolWireMaker.Add(hubSide2HubBottomEdge);
	cutingToolWireMaker.Add(CuttingToolWire_WithHubThickness);
	cutingToolWireMaker.Add(hubSide1HubBottomEdge);
	cutingToolWireMaker.Add(firTreeUpperFillet_side1);
	cutingToolWireMaker.Add(neckLineEdge);
	
	TopoDS_Wire firTreeCuttingToolFaceWire = cutingToolWireMaker.Wire();
	return firTreeCuttingToolFaceWire;
}



/* use this function to create the deapthcutting tool face face wire*/
TopoDS_Wire FirTreeCreator::create_DepthCuttingToolFaceWire(TopoDS_Wire CuttingToolWire_WithHubThickness, gp_Pnt point1, gp_Pnt point2)
{
	gp_Dir bladeCenterLine_Direction(badeAxisLineVector);
	gp_Vec bladeCenterLine_UnitVector(bladeCenterLine_Direction);

	gp_Trsf theTransformation_bladeCenterPoint;
	theTransformation_bladeCenterPoint.SetTranslation(bladeCenterLine_UnitVector.Multiplied(hubThickness));
	gp_Pnt point3 = bladeAxisPointOnHubSerface.Transformed(theTransformation_bladeCenterPoint);

	Handle(Geom_TrimmedCurve) hubBottomFaceCurve = GC_MakeArcOfCircle(point1, point3, point2);

	TopoDS_Edge hubBottomFaceEdge = BRepBuilderAPI_MakeEdge(hubBottomFaceCurve);

	BRepBuilderAPI_MakeWire wireMaker1;
	wireMaker1.Add(CuttingToolWire_WithHubThickness);
	wireMaker1.Add(hubBottomFaceEdge);

	TopoDS_Wire depthCuttingToolFaceWire = wireMaker1.Wire();

	
	return depthCuttingToolFaceWire;
}

/* we create cutting tool wire with hub thickness as a seperate funtion because we have to use this same wire
	to create the deapth cutting tool face wire and the fir tree cutting tool wire. so, use use this funtion 
	to seperate these two works*/
TopoDS_Wire FirTreeCreator::create_cuttingToolWire_WithHubThickness(TopoDS_Wire cuttingToolOutterWire_WithHubAngles, 
	gp_Pnt firstParameterSideLongCurveCuttingEgdgePoint, gp_Pnt lastParameterSideLongCurveCuttingEgdgePoint,
	gp_Pnt& point1, gp_Pnt& point2)
{
	gp_Vec firstCuttingEdgePointToZAxis_Vector(firstParameterSideLongCurveCuttingEgdgePoint, rotatingAxisPointForSelectedPlane);
	gp_Dir firstCuttingEdgePointToZAxis_Direction(firstCuttingEdgePointToZAxis_Vector);
	gp_Vec firstCuttingEdgePointToZAxis_UnitVector(firstCuttingEdgePointToZAxis_Direction);

	gp_Vec lastCuttingEdgePointToZAxis_Vector(lastParameterSideLongCurveCuttingEgdgePoint, rotatingAxisPointForSelectedPlane);
	gp_Dir lastCuttingEdgePointToZAxis_Direction(lastCuttingEdgePointToZAxis_Vector);
	gp_Vec lastCuttingEdgePointToZAxis_UnitVector(lastCuttingEdgePointToZAxis_Direction);

	gp_Trsf theTransformation_FirstCuttingEdgePoint;
	theTransformation_FirstCuttingEdgePoint.SetTranslation(firstCuttingEdgePointToZAxis_UnitVector.Multiplied(hubThickness));
	point1 = firstParameterSideLongCurveCuttingEgdgePoint.Transformed(theTransformation_FirstCuttingEdgePoint);

	gp_Trsf theTransformation_LastCuttingEdgePoint;
	theTransformation_LastCuttingEdgePoint.SetTranslation(lastCuttingEdgePointToZAxis_UnitVector.Multiplied(hubThickness));
	point2 = lastParameterSideLongCurveCuttingEgdgePoint.Transformed(theTransformation_LastCuttingEdgePoint);

	Handle(Geom_TrimmedCurve) hubSide1Curve = GC_MakeSegment(point1, firstParameterSideLongCurveCuttingEgdgePoint);
	Handle(Geom_TrimmedCurve) hubSide2Curve = GC_MakeSegment(point2, lastParameterSideLongCurveCuttingEgdgePoint);

	TopoDS_Edge hubSide1Edge = BRepBuilderAPI_MakeEdge(hubSide1Curve);
	TopoDS_Edge hubSide2Edge = BRepBuilderAPI_MakeEdge(hubSide2Curve);

	BRepBuilderAPI_MakeWire wireMaker1;
	wireMaker1.Add(hubSide2Edge);
	wireMaker1.Add(cuttingToolOutterWire_WithHubAngles);
	wireMaker1.Add(hubSide1Edge);

	TopoDS_Wire cuttingToolWire_WithHubThickness = wireMaker1.Wire();

	return cuttingToolWire_WithHubThickness;
}

/* If we want to cut different hub thicknesses, we have to use Cutting Tool Outter Wire With Hub Angles gain and again.
	So, define this function to seperate these two works*/
TopoDS_Wire FirTreeCreator::create_CuttingToolOutterWire_WithHubAngles(TopoDS_Wire cuttingToolOutterWire, Handle(Geom_Curve) longerCurve,
	gp_Pnt firstParameterSidePoint_CuttingToolOutterWire, gp_Pnt lastParameterSidePoint_CuttingToolOutterWire, 
	gp_Pnt& firstParameterSideLongCurveCuttingEgdgePoint, gp_Pnt& lastParameterSideLongCurveCuttingEgdgePoint)
{
	TopoDS_Wire cuttingToolOutterWire_WithHubAngles;
	
	Standard_Real bladeCenterToFirstParameterPointAngle = qAbs(badeAxisLineVector.Angle(firstParameterOfLongerCurveToZAxisVector));

	/* get the cutting edge point on longercuve by A1 angle*/
	gp_Pnt pointOnLongerCurveByA1_AngleFromBladeCenterLine = bladeAxisPointOnHubSerface.Rotated(rotatingAxis, A1);
	Standard_Real parameterByA1_AngleFromBladeCenterLine;
	GeomLib_Tool::Parameter(longerCurve, pointOnLongerCurveByA1_AngleFromBladeCenterLine, 1e-7, parameterByA1_AngleFromBladeCenterLine);
	gp_Vec pointOnLongerCurveByA1ToZAxisVector(pointOnLongerCurveByA1_AngleFromBladeCenterLine, rotatingAxisPointForSelectedPlane);


	/* get the cutting edge point on longercuve by A2 angle for next side*/
	gp_Pnt pointOnLongerCurveByA2_AngleFromBladeCenterLine = bladeAxisPointOnHubSerface.Rotated(rotatingAxis, -A2);
	Standard_Real parameterByA2_AngleFromBladeCenterLine;
	GeomLib_Tool::Parameter(longerCurve, pointOnLongerCurveByA2_AngleFromBladeCenterLine, 1e-7, parameterByA2_AngleFromBladeCenterLine);
	gp_Vec pointOnLongerCurveByA2ToZAxisVector(pointOnLongerCurveByA2_AngleFromBladeCenterLine, rotatingAxisPointForSelectedPlane);


	/* what is near to the firstParameterOfLongerCurve from parameterByA1_AngleFromBladeCenterLine
	and parameterByA2_AngleFromBladeCenterLine*/
	Standard_Real firstParameterSideLongCurveCuttingEgdgeParameter;
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

	gp_Vec firstCuttingEdgePointToZAxis_Vector(firstParameterSideLongCurveCuttingEgdgePoint, rotatingAxisPointForSelectedPlane);
	gp_Dir firstCuttingEdgePointToZAxis_Direction(firstCuttingEdgePointToZAxis_Vector);
	gp_Vec firstCuttingEdgePointToZAxis_UnitVector(firstCuttingEdgePointToZAxis_Direction);

	gp_Vec lastCuttingEdgePointToZAxis_Vector(lastParameterSideLongCurveCuttingEgdgePoint, rotatingAxisPointForSelectedPlane);
	gp_Dir lastCuttingEdgePointToZAxis_Direction(lastCuttingEdgePointToZAxis_Vector);
	gp_Vec lastCuttingEdgePointToZAxis_UnitVector(lastCuttingEdgePointToZAxis_Direction);

	gp_Trsf Transformation_firstParameterSideLongCurveCuttingEgdgePointCuttingTool;
	Transformation_firstParameterSideLongCurveCuttingEgdgePointCuttingTool.SetTranslation(-((firstCuttingEdgePointToZAxis_UnitVector.Multiplied(5))));
	gp_Pnt firstParameterSideLongCurveCuttingEgdgePoint_CuttingTool =
		firstParameterSideLongCurveCuttingEgdgePoint.Transformed(Transformation_firstParameterSideLongCurveCuttingEgdgePointCuttingTool);

	gp_Trsf Transformation_lastParameterSideLongCurveCuttingEgdgePointCuttingTool;
	Transformation_lastParameterSideLongCurveCuttingEgdgePointCuttingTool.SetTranslation(-((lastCuttingEdgePointToZAxis_UnitVector.Multiplied(5))));
	gp_Pnt lastParameterSideLongCurveCuttingEgdgePoint_CuttingTool =
		lastParameterSideLongCurveCuttingEgdgePoint.Transformed(Transformation_lastParameterSideLongCurveCuttingEgdgePointCuttingTool);

	gp_Circ hubfaceCuttingToolCircle(rotatingAxis2, firstParameterSideLongCurveCuttingEgdgePoint_CuttingTool.Distance(rotatingAxisPointForSelectedPlane));

	Handle(Geom_TrimmedCurve) hubFaceCurve1_CuttingTool = GC_MakeArcOfCircle(hubfaceCuttingToolCircle,
		firstParameterSidePoint_CuttingToolOutterWire,
		firstParameterSideLongCurveCuttingEgdgePoint_CuttingTool, true);
	Handle(Geom_TrimmedCurve) hubFaceCurve2_CuttingTool = GC_MakeArcOfCircle(hubfaceCuttingToolCircle,
		lastParameterSideLongCurveCuttingEgdgePoint_CuttingTool,
		lastParameterSidePoint_CuttingToolOutterWire, true);

	TopoDS_Edge hubFaceEdge1_CuttingTool = BRepBuilderAPI_MakeEdge(hubFaceCurve1_CuttingTool);
	TopoDS_Edge hubFaceEdge2_CuttingTool = BRepBuilderAPI_MakeEdge(hubFaceCurve2_CuttingTool);

	Handle(Geom_TrimmedCurve) hubSide1Curve = GC_MakeSegment(firstParameterSideLongCurveCuttingEgdgePoint_CuttingTool, firstParameterSideLongCurveCuttingEgdgePoint);
	Handle(Geom_TrimmedCurve) hubSide2Curve = GC_MakeSegment(lastParameterSideLongCurveCuttingEgdgePoint_CuttingTool, lastParameterSideLongCurveCuttingEgdgePoint);

	TopoDS_Edge hubSide1Edge = BRepBuilderAPI_MakeEdge(hubSide1Curve);
	TopoDS_Edge hubSide2Edge = BRepBuilderAPI_MakeEdge(hubSide2Curve);

	BRepBuilderAPI_MakeWire wireMaker1;
	wireMaker1.Add(hubSide2Edge);
	wireMaker1.Add(hubFaceEdge2_CuttingTool);
	wireMaker1.Add(cuttingToolOutterWire);
	wireMaker1.Add(hubFaceEdge1_CuttingTool);
	wireMaker1.Add(hubSide1Edge);

	cuttingToolOutterWire_WithHubAngles = wireMaker1.Wire();
	

	return cuttingToolOutterWire_WithHubAngles;
}

/* we have to use the cutting tool Outter wire as same as it for many cases if want, such as that
	if want to cut hub surface with different side angles for some levels, we can use this outter cutting tool wire 
	again and again.*/
TopoDS_Wire FirTreeCreator::create_CuttingToolOutterWire(gp_Pnt firstParameterPointOfLongerCurve, gp_Pnt lastParameterPointOfLongerCurve,
		Standard_Real firstParameterOfLongerCurve, Standard_Real lastParameterOfLongerCurve, Standard_Real midParameterOflongercurve,
		Handle(Geom_Curve) longerCurve, Standard_Real givenPlatformHight, gp_Pnt& firstParameterPoint_CuttingTool, gp_Pnt& lastParameterPoint_CuttingTool)
{
	TopoDS_Wire cuttingToolOutterWire;

	firstParameterOfLongerCurveToZAxisVector = gp_Vec(firstParameterPointOfLongerCurve, rotatingAxisPointForSelectedPlane);
	gp_Dir firstParameterOfLongerCurveToZAxis_Direction(firstParameterOfLongerCurveToZAxisVector);
	gp_Vec firstParameterOfLongerCurveToZAxis_UnitVector(firstParameterOfLongerCurveToZAxis_Direction);

	gp_Vec lastParameterOfLongerCurveToZAxisVector(lastParameterPointOfLongerCurve, rotatingAxisPointForSelectedPlane);
	gp_Dir lastParameterOfLongerCurveToZAxis_Direction(lastParameterOfLongerCurveToZAxisVector);
	gp_Vec lastParameterOfLongerCurveToZAxis_UnitVector(lastParameterOfLongerCurveToZAxis_Direction);

	gp_Trsf Transformation_firstParameterPointCuttingTool;
	Transformation_firstParameterPointCuttingTool.SetTranslation(-((firstParameterOfLongerCurveToZAxis_UnitVector.Multiplied(5))));
	firstParameterPoint_CuttingTool =
		firstParameterPointOfLongerCurve.Transformed(Transformation_firstParameterPointCuttingTool);

	gp_Trsf Transformation_lastParameterPointCuttingTool;
	Transformation_lastParameterPointCuttingTool.SetTranslation(-((lastParameterOfLongerCurveToZAxis_UnitVector.Multiplied(5))));
	lastParameterPoint_CuttingTool =
		lastParameterPointOfLongerCurve.Transformed(Transformation_lastParameterPointCuttingTool);

	gp_Pnt firstParameterSide_CuttingToolPoint = longerCurve->Value(firstParameterOfLongerCurve - (midParameterOflongercurve / 5));
	gp_Pnt lastParameterSide_CuttingToolPoint = longerCurve->Value(lastParameterOfLongerCurve + (midParameterOflongercurve / 5));


	Handle(Geom_TrimmedCurve) upperConnecter1_Curve = GC_MakeSegment(firstParameterPoint_CuttingTool,
		firstParameterSide_CuttingToolPoint);
	Handle(Geom_TrimmedCurve) upperConnecter2_Curve = GC_MakeSegment(lastParameterPoint_CuttingTool,
		lastParameterSide_CuttingToolPoint);

	TopoDS_Edge upperConnecter1_Edge = BRepBuilderAPI_MakeEdge(upperConnecter1_Curve);
	TopoDS_Edge upperConnecter2_Edge = BRepBuilderAPI_MakeEdge(upperConnecter2_Curve);

	gp_Vec firstParameterSide_CuttingToolPointToZaxis_Vector(firstParameterSide_CuttingToolPoint, rotatingAxisPointForSelectedPlane);
	gp_Vec lastParameterSide_CuttingToolPointToZaxis_Vector(lastParameterSide_CuttingToolPoint, rotatingAxisPointForSelectedPlane);

	if (givenPlatformHight != NULL)
	{
		gp_Dir firstParameterSide_CuttingToolPointToZaxis_VectorDirection(firstParameterSide_CuttingToolPointToZaxis_Vector);
		gp_Vec firstParameterSide_CuttingToolPointToZaxis_UnitVector(firstParameterSide_CuttingToolPointToZaxis_VectorDirection);


		gp_Dir lastParameterSide_CuttingToolPointToZaxis_VectorDirection(lastParameterSide_CuttingToolPointToZaxis_Vector);
		gp_Vec lastParameterSide_CuttingToolPointToZaxis_UnitVector(lastParameterSide_CuttingToolPointToZaxis_VectorDirection);

		gp_Trsf Transformation_firstParameterSide_CuttingToolPoint;
		Transformation_firstParameterSide_CuttingToolPoint.SetTranslation(firstParameterSide_CuttingToolPointToZaxis_UnitVector * (givenPlatformHight + 6));
		gp_Pnt firstParameterSide_CuttingTool_BottomPoint =
			firstParameterSide_CuttingToolPoint.Transformed(Transformation_firstParameterSide_CuttingToolPoint);

		gp_Trsf Transformation_lastParameterSide_CuttingToolPoint;
		Transformation_lastParameterSide_CuttingToolPoint.SetTranslation(lastParameterSide_CuttingToolPointToZaxis_UnitVector * (givenPlatformHight + 6));
		gp_Pnt lastParameterSide_CuttingTool_BottomPoint =
			lastParameterSide_CuttingToolPoint.Transformed(Transformation_lastParameterSide_CuttingToolPoint);

		Handle(Geom_TrimmedCurve) firstParameterSide_cuttingToolCurve = GC_MakeSegment(firstParameterSide_CuttingToolPoint,
			firstParameterSide_CuttingTool_BottomPoint);
		Handle(Geom_TrimmedCurve) lastParameterSide_cuttingToolCurve = GC_MakeSegment(lastParameterSide_CuttingToolPoint,
			lastParameterSide_CuttingTool_BottomPoint);

		TopoDS_Edge firstParameterSide_cuttingToolEdge = BRepBuilderAPI_MakeEdge(firstParameterSide_cuttingToolCurve);
		TopoDS_Edge lastParameterSide_cuttingToolEdge = BRepBuilderAPI_MakeEdge(lastParameterSide_cuttingToolCurve);

		Handle(Geom_TrimmedCurve) cuttingTool_BottomCurve = GC_MakeSegment(firstParameterSide_CuttingTool_BottomPoint,
			lastParameterSide_CuttingTool_BottomPoint);

		TopoDS_Edge cuttingTool_BottomEdge = BRepBuilderAPI_MakeEdge(cuttingTool_BottomCurve);

		BRepBuilderAPI_MakeWire wireMaker1;
		wireMaker1.Add(upperConnecter2_Edge);
		wireMaker1.Add(lastParameterSide_cuttingToolEdge);
		wireMaker1.Add(cuttingTool_BottomEdge);
		wireMaker1.Add(firstParameterSide_cuttingToolEdge);
		wireMaker1.Add(upperConnecter1_Edge);

		
		cuttingToolOutterWire = wireMaker1.Wire();
	}
	else
	{
		// create cutting tool up to rotating axis if givenPlatformHight is not given
		//*********************ask from sir ???????????
	}

	return cuttingToolOutterWire;
}

/* given blade shape is a imported one from the out side to the our context in the programe.
	So, we get this imported shape to a variable(shape) from the context by this function*/
TopoDS_Shape FirTreeCreator::getBlade(void)
{
	/* get the imported blade shape to the parameter shape*/
	TopoDS_Shape shape;
	AIS_ListOfInteractive objList;
	myContext->DisplayedObjects(objList);
	AIS_ListIteratorOfListOfInteractive iter;
	for (iter.Initialize(objList); iter.More(); iter.Next())
	{
		Handle(AIS_InteractiveObject) aisShp = iter.Value();
		Handle(AIS_Shape) aisShape = Handle(AIS_Shape)::DownCast(aisShp);
		handleAISShapesVector.append(aisShape);
		if (aisShp->IsKind("AIS_Shape"))
		{
			shape = aisShape->Shape();
		}
	}

	if (shape.IsNull())
	{
		QMessageBox msgBox;
		msgBox.setText("Please, Import the bladeshape");
		msgBox.exec();
		
	}

	return shape;
}

/* If the given blade shape is a shell we have to convert it to a solid for cutting purposes.
	As a sample, I was given a shell*/
TopoDS_Shape FirTreeCreator::getSolidBlade(TopoDS_Shape shellShape)
{
	/* convert given shell to a solid*/
	TopoDS_Shell shell = TopoDS::Shell(shellShape);
	TopoDS_Shape shape = BRepBuilderAPI_MakeSolid(shell);
	return shape;
}

/* use this fuction to get the selected face from the context to a variable*/
TopoDS_Face FirTreeCreator::getSelectedFace()
{
	/* get the selected face to the parameter selectedFace*/
	TopoDS_Shape selectedShape;
	myContext->InitSelected();
	selectedShape = myContext->SelectedShape();
	TopoDS_Face selectedFace = TopoDS::Face(selectedShape);
	/*
	if (selectedFace.IsNull())
	{
		QMessageBox msgBox;
		msgBox.setText("Please, Select the front face of the blade platform");
		msgBox.exec();

	}
	*/

	return selectedFace;
}

/* In my case, select a face as an input to get the position and the initially wanted properties from the given face.
	For that, want to find the longer edge and the shorter edge seperatly from the selected face. for that , want to identify the 
	all edges from the selected face as the curved Edges and Straight edges. use this function for that.*/
void FirTreeCreator::getFaceEdges(TopoDS_Face selectedFace, QVector<TopoDS_Edge>& curvedTopoDSEdges, QVector<TopoDS_Edge>& straightTopoDSEdges)
{
	/* find what the curved edges and straight edges*/
	TopExp_Explorer edgeExplorer(selectedFace, TopAbs_EDGE);

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
}

/* In my case, select a face as an input to get the position and the initially wanted properties from the given face.
	For that, want to find the longer edge and the shorter edge seperatly from the selected face. use this function for that.*/
void FirTreeCreator::getCurveEdges(QVector<TopoDS_Edge> curvedTopoDSEdges, TopoDS_Edge& longerCurveEdge, TopoDS_Edge& shoterCurveEdge)
{
	/* find what the longer curve edge*/
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
}

/* we use this fuction to find the following properties for the hub surface curve on the related face (Assume selected face)*/
void FirTreeCreator::getEdgeProperties(TopoDS_Edge edge, gp_Pnt& firstParameterPoint, gp_Pnt& lastParameterPoint, gp_Pnt& CenterPoint,
	Standard_Real& firstParameter, Standard_Real& lastParameter, Standard_Real& centerParameter, Handle(Geom_Curve)& curve)
{
	curve = BRep_Tool::Curve(edge, firstParameter, lastParameter);
	centerParameter = (firstParameter + lastParameter) / 2;

	CenterPoint = curve->Value(centerParameter);
	firstParameterPoint = curve->Value(firstParameter);
	lastParameterPoint = curve->Value(lastParameter);
}

/* we want to find the find the shaft Axial point related to the initialy considering face (Assume selected face).
	the we want to find different Axises which are useful more through out the code.
	so, we set them as following.*/
void FirTreeCreator::setRotatingAxiProperties(gp_Pnt bladeAxisPointOnHubSerface)
{
	/* get rotating Axis properties*/
	rotatingAxisPointForSelectedPlane = gp_Pnt(0, 0, bladeAxisPointOnHubSerface.Z());
	rotatingAxis = gp_Ax1(rotatingAxisPointForSelectedPlane, gp::DZ());
	rotatingAxis2 = gp_Ax2(rotatingAxisPointForSelectedPlane, gp::DZ());
	rotatingAxis3 = gp_Ax3(rotatingAxis2);
}

void FirTreeCreator::setA1InDegrees(Standard_Real A1InDegrees_)
{
	A1InDegrees = A1InDegrees_;
}
void FirTreeCreator::setA2InDegrees(Standard_Real A2InDegrees_)
{
	A2InDegrees = A2InDegrees_;
}
void FirTreeCreator::setA3InDegrees(Standard_Real A3InDegrees_)
{
	A3InDegrees = A3InDegrees_;
}
void FirTreeCreator::setA4InDegrees(Standard_Real A4InDegrees_)
{
	A4InDegrees = A4InDegrees_;
}
void FirTreeCreator::setBottomNeckWidth(Standard_Real bottomNeckWidth_)
{
	bottomNeckWidth = bottomNeckWidth_;
}
void FirTreeCreator::setHubThickness(Standard_Real hubThickness_)
{
	hubThickness = hubThickness_;
}
void FirTreeCreator::setNeckHeight(Standard_Real neckHeight_)
{
	neckHeight = neckHeight_;
}
void FirTreeCreator::setLobeLodeAngle(QVector<Standard_Real> lobeLodeAngles_)
{
	lobeLodeAngles = lobeLodeAngles_;
}
void FirTreeCreator::setNumberOfLobes(Standard_Real numberOfLobes_)
{
	numberOfLobes = numberOfLobes_;
}
void FirTreeCreator::setConcaveRadius(QVector<Standard_Real> concaveRadius_)
{
	concaveRadius = concaveRadius_;
}
void FirTreeCreator::setConvexRadius(QVector<Standard_Real> convexRadius_)
{
	convexRadius = convexRadius_;
}
void FirTreeCreator::setFirTreebottomFilletRadius(Standard_Real firTreebottomFilletRadius_)
{
	firTreebottomFilletRadius = firTreebottomFilletRadius_;
}
void FirTreeCreator::setFirTreeupperFilletRadius(Standard_Real firTreeupperFilletRadius_)
{
	firTreeupperFilletRadius = firTreeupperFilletRadius_;
}
void FirTreeCreator::setLobeLoadLengths(QVector<Standard_Real> lobeLoadLengths_)
{
	lobeLoadLengths = lobeLoadLengths_;
}
void FirTreeCreator::setLobeThicknesses(QVector<Standard_Real> lobeThicknesses_)
{
	lobeThicknesses = lobeThicknesses_;
}

void FirTreeCreator::displayASape(TopoDS_Shape topoDSShape)
{
	Handle(AIS_Shape) anAis = new AIS_Shape(topoDSShape);
	anAis->SetColor(Quantity_NOC_CORAL);
	myContext->Display(anAis);
	handleAISShapesVector.append(anAis);
}

void FirTreeCreator::removeDisplaiedAISShape(void)
{
	while (handleAISShapesVector.isEmpty() != TRUE) {
		Handle(AIS_Shape) anAis = handleAISShapesVector.takeLast();
		myContext->Remove(anAis);
	}
	
	myContext->UpdateCurrentViewer();

}
