#ifndef OSGHELPER_H
#define OSGHELPER_H

//osg includes
#include <osg/ref_ptr>
#include <osgViewer/GraphicsWindow>
#include <osgViewer/Viewer>
#include <osg/Camera>
#include <osg/ShapeDrawable>
#include <osg/StateSet>
#include <osg/Material>
#include <osgGA/EventQueue>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <osgUtil/SmoothingVisitor>
#include <osgText/Text>
#include <osgText/Font>
#include <osg/BoundingBox>
//oce includes
#include <BRep_Tool.hxx>
#include <BRepTools.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepLib.hxx>

#include <BRepOffsetAPI_MakeThickSolid.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>

#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakePrism.hxx>

#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeSegment.hxx>

#include <GCE2d_MakeSegment.hxx>

#include <gp.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Ax2d.hxx>
#include <gp_Dir.hxx>
#include <gp_Dir2d.hxx>
#include <gp_Pnt.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Trsf.hxx>
#include <gp_Vec.hxx>

#include <Geom_CylindricalSurface.hxx>
#include <Geom_Plane.hxx>
#include <Geom_Surface.hxx>
#include <Geom_TrimmedCurve.hxx>

#include <Geom2d_Ellipse.hxx>
#include <Geom2d_TrimmedCurve.hxx>

#include <TopExp_Explorer.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>

#include <TopTools_ListOfShape.hxx>

#include <AIS.hxx>
#include <V3d_Viewer.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeWedge.hxx>
#include <Poly_Triangulation.hxx>
#include <Poly_Array1OfTriangle.hxx>

#include <STEPControl_Writer.hxx>
#include <STEPControl_Reader.hxx>
#include <StepData_StepModel.hxx>

# include <STEPConstruct_Styles.hxx>
# include <STEPConstruct.hxx>
# include <StepVisual_StyledItem.hxx>
# include <StepShape_ShapeRepresentation.hxx>
# include <StepVisual_PresentationStyleByContext.hxx>
# include <StepVisual_StyleContextSelect.hxx>
# include <StepVisual_PresentationStyleByContext.hxx>
# include <StepRepr_RepresentedDefinition.hxx>
# include <StepShape_ShapeDefinitionRepresentation.hxx>
# include <StepRepr_CharacterizedDefinition.hxx>
# include <StepRepr_ProductDefinitionShape.hxx>
# include <StepRepr_AssemblyComponentUsage.hxx>
# include <StepRepr_AssemblyComponentUsage.hxx>
# include <StepRepr_SpecifiedHigherUsageOccurrence.hxx>
# include <StepBasic_Product.hxx>
# include <StepBasic_Product.hxx>
# include <StepBasic_ProductDefinition.hxx>
# include <StepBasic_ProductDefinition.hxx>
# include <StepBasic_ProductDefinitionFormation.hxx>

osg::Group* createScene();

TopoDS_Shape MakeBottle(const Standard_Real myWidth, const Standard_Real myHeight,
                        const Standard_Real myThickness);
osg::Geometry* createGeometryFromShape(TopoDS_Shape& shape,
                                                    const osg::Vec3& geomColor, gp_Trsf& transformation);
osg::Group* readSceneFile(std::string filePath);
osg::Geode* createBottle(double width, double height, double thickness,
                                      const osg::Vec3f &geomColor/*, const Matrixd &viewMatrix*/);
osg::Geode* readStepFile(std::string path);

#endif // OSGHELPER_H
