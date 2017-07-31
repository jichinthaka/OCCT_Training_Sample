#include <Standard_TypeDef.hxx>
#include <AIS_InteractiveContext.hxx>

#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopTools_ListOfShape.hxx>

#include <Geom_Curve.hxx>

#include <AIS_Shape.hxx>

#include <QVector>

//class OccView;

class FirTreeCreatorV2
{
public:
	FirTreeCreatorV2(Handle_AIS_InteractiveContext context);
	~FirTreeCreatorV2();


private:
	TopoDS_Shape getBlade(void);
	TopoDS_Shape getSolidBlade(TopoDS_Shape shell);
	TopoDS_Face getSelectedFace();
	void getFaceEdges(TopoDS_Face selectedFace, QVector<TopoDS_Edge>& curvedTopoDSEdges, QVector<TopoDS_Edge>& straightTopoDSEdges);
	void getCurveEdges(QVector<TopoDS_Edge> curvedTopoDSEdges, TopoDS_Edge& longerCurveEdge, TopoDS_Edge& shoterCurveEdge);

	void getEdgeProperties(TopoDS_Edge edge, gp_Pnt& firstParameterPoint, gp_Pnt& lastParameterPoint, gp_Pnt& CenterPoint,
		Standard_Real& firstParameter, Standard_Real& lastParameter, Standard_Real& midParameter, Handle(Geom_Curve)& curve);

	TopoDS_Wire create_CuttingToolOutterWire(gp_Pnt firstParameterPointOfLongerCurve, gp_Pnt lastParameterPointOfLongerCurve,
		Standard_Real firstParameterOfLongerCurve, Standard_Real lastParameterOfLongerCurve, Standard_Real midParameterOflongercurve,
		Handle(Geom_Curve) longerCurve, Standard_Real givenPlatformHight, gp_Pnt& firstParameterPoint_CuttingTool,
		gp_Pnt& lastParameterPoint_CuttingTool);

	TopoDS_Wire create_CuttingToolOutterWire_WithHubAngles(TopoDS_Wire cuttingToolOutterWire, Handle(Geom_Curve) longerCurve,
		gp_Pnt firstParameterSidePoint_CuttingToolOutterWire, gp_Pnt lastParameterSidePoint_CuttingToolOutterWire,
		gp_Pnt& firstParameterSideLongCurveCuttingEgdgePoint, gp_Pnt& lastParameterSideLongCurveCuttingEgdgePoint,
		bool isForPlatformCutting);

	TopoDS_Wire create_cuttingToolWire_WithHubThickness(TopoDS_Wire cuttingToolOutterWire_WithHubAngles,
		gp_Pnt firstParameterSideLongCurveCuttingEgdgePoint, gp_Pnt lastParameterSideLongCurveCuttingEgdgePoint,
		gp_Pnt& point1, gp_Pnt& point2);

	TopoDS_Wire create_DepthCuttingToolFaceWire(TopoDS_Wire CuttingToolWire_WithHubThickness, gp_Pnt point1, gp_Pnt point2);

	TopoDS_Wire create_platFormCuttingToolFaceWire(TopoDS_Wire cuttingToolOutterWire_WithHubAngles,
		gp_Pnt firstParameterSideLongCurveCuttingEgdgePoint, gp_Pnt lastParameterSideLongCurveCuttingEgdgePoint);

	TopoDS_Shape create_CuttingToolSolid(TopoDS_Wire faceWireCuttingTool, Standard_Real cuttingToolDeapth,
		Standard_Real depthToStart, Standard_Real cuttingToolDeapthTolerance, Standard_Real cuttingAngle);

	TopoDS_Wire create_firTree_CuttingToolFaceWire(TopoDS_Wire CuttingToolWire_WithHubThickness, gp_Pnt point1, gp_Pnt point2);

	TopoDS_Wire moveCuttingToolFace_WithAngle(TopoDS_Wire startFace, Standard_Real angle, Standard_Real depth);



private:
	void setRotatingAxiProperties(gp_Pnt bladeAxisPointOnHubSerface);

	TopoDS_Edge make2dFillet(TopoDS_Edge& e1, TopoDS_Edge& e2, gp_Pnt CommonPoint, Standard_Real r, gp_Ax3 thePlane);

	void displayASape(TopoDS_Shape topoDSShape);

	void removeDisplaiedAISShape(void);

	bool BooleanCutTwoShapes(TopTools_ListOfShape aLS, TopTools_ListOfShape aLT, TopoDS_Shape &cutShape);


public:
	void inItAlgo(Standard_Real A1, Standard_Real A2, Standard_Real A3, Standard_Real A4, gp_Pnt bladeCenterPointOnHubSerface,
		Standard_Real hubThickness, Standard_Real neckHeight, Standard_Real lobeLoadLength, Standard_Real lobeThickness,
		Standard_Real lobeLodeAngle, Standard_Real concaveRadius, Standard_Real convexRadius, Standard_Real firTreebottomFilletRadius,
		Standard_Real firTreeupperFilletRadius, Standard_Real firTreeDepthFrom_SelectedFace, Standard_Real firTreeDepthFrom_NextToSelectedFace);

	TopoDS_Shape build(void);

	void setA1InDegrees(Standard_Real A1InDegrees_);
	void setA2InDegrees(Standard_Real A2InDegrees_);
	void setA3InDegrees(Standard_Real A3InDegrees_);
	void setA4InDegrees(Standard_Real A4InDegrees_);
	void setBottomNeckWidth(Standard_Real bottomNeckWidth_);
	void setHubThickness(Standard_Real hubThickness_);
	void setNeckHeight(Standard_Real neckHeight_);
	void setLobeLodeAngle(QVector<Standard_Real> lobeLodeAngles_);
	void setNumberOfLobes(Standard_Real numberOfLobes_);
	void setConcaveRadius(QVector<Standard_Real> concaveRadius_);
	void setConvexRadius(QVector<Standard_Real> convexRadius_);
	void setFirTreebottomFilletRadius(Standard_Real firTreebottomFilletRadius_);
	void setFirTreeupperFilletRadius(Standard_Real firTreeupperFilletRadius_);
	void setLobeLoadLengths(QVector<Standard_Real> lobeLoadLengths_);
	void setLobeThicknesses(QVector<Standard_Real> lobeThicknesses_);
	//void setFirTreeDepthFrom_SelectedFace(Standard_Real firTreeDepthFrom_SelectedFace);
	//void setFirTreeDepthFrom_NextToSelectedFace(Standard_Real firTreeDepthFrom_NextToSelectedFace);

private:
	//OccView* myOccView;
	Handle_AIS_InteractiveContext myContext;

	QVector<Handle(AIS_Shape)> handleAISShapesVector;

	gp_Pnt rotatingAxisPointForSelectedPlane;
	gp_Ax1 rotatingAxis;
	gp_Ax2 rotatingAxis2;
	gp_Ax3 rotatingAxis3;

	gp_Vec badeAxisLineVector;
	//gp_Vec firstParameterOfLongerCurveToZAxisVector;



private:
	/* these are my inputs. hard coded just for now*/

	Standard_Real A1InDegrees = 35.22;
	Standard_Real A2InDegrees = 20.19;
	Standard_Real A3InDegrees = 8;
	Standard_Real A4InDegrees = 14;
	Standard_Real platformCuttingAngle_InDegrees = 17;
	Standard_Real firTreeCuttingAngle_InDegrees = 15;

	Standard_Real A1 = ((3.141 / 180)*35.56);/* 0.75;//0492;//*///(3.141 * 0.8) / 6;	//angle from blade center to one hub side
	Standard_Real A2 = 0.6108;//0865;//*/(3.141 * 1.165234002) / 6;		//angle from blade center to next hub side
	Standard_Real A3 = 0; //-(3.141 * (0.7)) / 6;	//angle from blade center to fir tree center

	Standard_Real A4 = (3.141 * 0.5) / 15;	//just for now, assume Neck Thickness by an angle

	Standard_Real givenPlatFormHight = NULL; // this use only to reduce the cutting tool are

	Standard_Real givenPlatFormThickness = 50;	//just for now we use this, we should be able to find that using given shape


	QVector<Standard_Real> releafBottomWidths;

	gp_Pnt bladeAxisPointOnHubSerface;	//asume it is the selected face longer curve center 
	Standard_Real hubThickness = 13;
	Standard_Real neckHeight;	//just hard coded as a fraction of selected face center line length
	Standard_Real bottomNeckWidth;
	QVector<Standard_Real> lobeLoadLengths;	//this is the first firtree upper angle length
											//get it as a fraction of selected face center line length
											//other other angle lengths are redusing fraction of this
	QVector<Standard_Real> lobeThicknesses;	//get it as a fraction of selected face center line length
											//other other angle lengths are redusing fraction of this

	Standard_Real numberOfLobes = 4;	// we can calculate this from the length of the lobeLoadLengths vector
										//this value is only a hard codded one

										//Standard_Real lobeLodeAngle = (3.141 * 2.5) / 6;
	QVector<Standard_Real> lobeLodeAngles;
	
	QVector<Standard_Real> concaveRadius;

	QVector<Standard_Real> convexRadius;
	
	Standard_Real firTreebottomFilletRadius = 3.8;
	Standard_Real firTreeupperFilletRadius = 0.9;

	Standard_Real firTreeDepthFrom_SelectedFace = 5;
	Standard_Real firTreeDepthFrom_NextToSelectedFace = 5;

	Standard_Real platformCuttingAngle;
	Standard_Real firTreeCuttingAngle;


};

