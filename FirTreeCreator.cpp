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
#include <BRep_Tool.hxx>
#include <BRepGProp.hxx>
#include <BRepTools.hxx>

#include <GC_MakeSegment.hxx>
#include <GC_MakeArcOfCircle.hxx>

#include <Geom_Curve.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <GeomLProp_CLProps.hxx>
#include <GeomLib_Tool.hxx>

#include <GProp_GProps.hxx>



FirTreeCreator::FirTreeCreator(Handle_AIS_InteractiveContext context)
{
	myContext = context;
}


FirTreeCreator::~FirTreeCreator()
{
}

TopoDS_Shape FirTreeCreator::build()
{
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
	TopoDS_Wire DepthCuttingToolFaceWire = creat_DepthCuttingToolFaceWire(CuttingToolWire_WithHubThickness, point1, point2);



	




	return DepthCuttingToolFaceWire;
}

TopoDS_Wire FirTreeCreator::creat_DepthCuttingToolFaceWire(TopoDS_Wire CuttingToolWire_WithHubThickness, gp_Pnt point1, gp_Pnt point2)
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
	wireMaker1.Add(cuttingToolOutterWire_WithHubAngles);
	wireMaker1.Add(hubSide1Edge);
	wireMaker1.Add(hubSide2Edge);

	TopoDS_Wire cuttingToolWire_WithHubThickness = wireMaker1.Wire();

	return cuttingToolWire_WithHubThickness;
}

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
	wireMaker1.Add(cuttingToolOutterWire);
	wireMaker1.Add(hubFaceEdge1_CuttingTool);
	wireMaker1.Add(hubFaceEdge2_CuttingTool);
	wireMaker1.Add(hubSide1Edge);
	wireMaker1.Add(hubSide2Edge);
	wireMaker1.Add(hubFaceEdge1_CuttingTool);
	wireMaker1.Add(hubFaceEdge2_CuttingTool);

	cuttingToolOutterWire_WithHubAngles = wireMaker1.Wire();
	

	return cuttingToolOutterWire_WithHubAngles;
}

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
		wireMaker1.Add(upperConnecter1_Edge);
		wireMaker1.Add(firstParameterSide_cuttingToolEdge);
		wireMaker1.Add(cuttingTool_BottomEdge);
		wireMaker1.Add(lastParameterSide_cuttingToolEdge);
		wireMaker1.Add(upperConnecter2_Edge);
		
		cuttingToolOutterWire = wireMaker1.Wire();
	}
	else
	{
		// create cutting tool up to rotating axis if givenPlatformHight is not given
		//*********************ask from sir ???????????
	}

	return cuttingToolOutterWire;
}


TopoDS_Shape FirTreeCreator::getBlade(void)
{
	/* get the imported blade shape to the parameter shape*/
	TopoDS_Shape shape;
	AIS_ListOfInteractive objList;
	//myOccView->getContext()->DisplayedObjects(objList);
	myContext->DisplayedObjects(objList);
	AIS_ListIteratorOfListOfInteractive iter;
	for (iter.Initialize(objList); iter.More(); iter.Next())
	{
		Handle(AIS_InteractiveObject) aisShp = iter.Value();
		if (aisShp->IsKind("AIS_Shape"))
		{
			shape = Handle(AIS_Shape)::DownCast(aisShp)->Shape();
		}
	}
	return shape;
}


TopoDS_Shape FirTreeCreator::getSolidBlade(TopoDS_Shape shellShape)
{
	/* convert given shell to a solid*/
	TopoDS_Shell shell = TopoDS::Shell(shellShape);
	TopoDS_Shape shape = BRepBuilderAPI_MakeSolid(shell);
	return shape;
}

TopoDS_Face FirTreeCreator::getSelectedFace()
{
	/* get the selected face to the parameter selectedFace*/
	TopoDS_Shape selectedShape;
	//myOccView->getContext()->InitSelected();
	myContext->InitSelected();
	selectedShape = myContext->SelectedShape();
	TopoDS_Face selectedFace = TopoDS::Face(selectedShape);
	return selectedFace;
}

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

void FirTreeCreator::getEdgeProperties(TopoDS_Edge edge, gp_Pnt& firstParameterPoint, gp_Pnt& lastParameterPoint, gp_Pnt& CenterPoint,
	Standard_Real& firstParameter, Standard_Real& lastParameter, Standard_Real& centerParameter, Handle(Geom_Curve)& curve)
{
	curve = BRep_Tool::Curve(edge, firstParameter, lastParameter);
	centerParameter = (firstParameter + lastParameter) / 2;

	CenterPoint = curve->Value(centerParameter);
	firstParameterPoint = curve->Value(firstParameter);
	lastParameterPoint = curve->Value(lastParameter);
}

void FirTreeCreator::setRotatingAxiProperties(gp_Pnt bladeAxisPointOnHubSerface)
{
	/* get rotating Axis properties*/
	rotatingAxisPointForSelectedPlane = gp_Pnt(0, 0, bladeAxisPointOnHubSerface.Z());
	rotatingAxis = gp_Ax1(rotatingAxisPointForSelectedPlane, gp::DZ());
	rotatingAxis2 = gp_Ax2(rotatingAxisPointForSelectedPlane, gp::DZ());
	rotatingAxis3 = gp_Ax3(rotatingAxis2);
}
