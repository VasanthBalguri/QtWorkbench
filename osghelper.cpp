#include "osghelper.h"


//need to be removed
ref_ptr<Group> createScene()
{
    osg::ref_ptr<osg::Cylinder> cylinder    = new osg::Cylinder( osg::Vec3( 0.f, 0.f, 0.f ), 0.25f, 0.5f );
    osg::ref_ptr<osg::ShapeDrawable> sd = new osg::ShapeDrawable( cylinder );
    sd->setColor( osg::Vec4( 0.8f, 0.5f, 0.2f, 1.f ) );
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    geode->addDrawable(sd);
    osg::ref_ptr<osg::Group> root = new osg::Group();

    root->addChild(geode);

    osg::ref_ptr<osg::StateSet> stateSet = root->getOrCreateStateSet();
    osg::ref_ptr<osg::Material> material = new osg::Material;
    material->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
    stateSet->setAttributeAndModes( material, osg::StateAttribute::ON );
    stateSet->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );

    return root;
}

//sample code to create bottle in osg, will remove it later
TopoDS_Shape
MakeBottle(const Standard_Real myWidth, const Standard_Real myHeight,
           const Standard_Real myThickness)
{
  // Profile : Define Support Points
  gp_Pnt aPnt1(-myWidth / 2., 0, 0);
  gp_Pnt aPnt2(-myWidth / 2., -myThickness / 4., 0);
  gp_Pnt aPnt3(0, -myThickness / 2., 0);
  gp_Pnt aPnt4(myWidth / 2., -myThickness / 4., 0);
  gp_Pnt aPnt5(myWidth / 2., 0, 0);

  // Profile : Define the Geometry
  Handle(Geom_TrimmedCurve) anArcOfCircle = GC_MakeArcOfCircle(aPnt2,aPnt3,aPnt4);
  Handle(Geom_TrimmedCurve) aSegment1 = GC_MakeSegment(aPnt1, aPnt2);
  Handle(Geom_TrimmedCurve) aSegment2 = GC_MakeSegment(aPnt4, aPnt5);

  // Profile : Define the Topology
  TopoDS_Edge anEdge1 = BRepBuilderAPI_MakeEdge(aSegment1);
  TopoDS_Edge anEdge2 = BRepBuilderAPI_MakeEdge(anArcOfCircle);
  TopoDS_Edge anEdge3 = BRepBuilderAPI_MakeEdge(aSegment2);
  TopoDS_Wire aWire  = BRepBuilderAPI_MakeWire(anEdge1, anEdge2, anEdge3);

  // Complete Profile
  gp_Ax1 xAxis = gp::OX();
  gp_Trsf aTrsf;

  aTrsf.SetMirror(xAxis);
  BRepBuilderAPI_Transform aBRepTrsf(aWire, aTrsf);
  TopoDS_Shape aMirroredShape = aBRepTrsf.Shape();
  TopoDS_Wire aMirroredWire = TopoDS::Wire(aMirroredShape);

  BRepBuilderAPI_MakeWire mkWire;
  mkWire.Add(aWire);
  mkWire.Add(aMirroredWire);
  TopoDS_Wire myWireProfile = mkWire.Wire();

  // Body : Prism the Profile
  TopoDS_Face myFaceProfile = BRepBuilderAPI_MakeFace(myWireProfile);
  gp_Vec aPrismVec(0, 0, myHeight);
  TopoDS_Shape myBody = BRepPrimAPI_MakePrism(myFaceProfile, aPrismVec);

  // Body : Apply Fillets
  BRepFilletAPI_MakeFillet mkFillet(myBody);
  TopExp_Explorer anEdgeExplorer(myBody, TopAbs_EDGE);
  while(anEdgeExplorer.More()){
    TopoDS_Edge anEdge = TopoDS::Edge(anEdgeExplorer.Current());
    //Add edge to fillet algorithm
    mkFillet.Add(myThickness / 12., anEdge);
    anEdgeExplorer.Next();
  }

  myBody = mkFillet.Shape();

  // Body : Add the Neck
  gp_Pnt neckLocation(0, 0, myHeight);
  gp_Dir neckAxis = gp::DZ();
  gp_Ax2 neckAx2(neckLocation, neckAxis);

  Standard_Real myNeckRadius = myThickness / 4.;
  Standard_Real myNeckHeight = myHeight / 10.;

  BRepPrimAPI_MakeCylinder MKCylinder(neckAx2, myNeckRadius, myNeckHeight);
  TopoDS_Shape myNeck = MKCylinder.Shape();

  myBody = BRepAlgoAPI_Fuse(myBody, myNeck);

  // Body : Create a Hollowed Solid
  TopoDS_Face   faceToRemove;
  Standard_Real zMax = -1;

  for(TopExp_Explorer aFaceExplorer(myBody, TopAbs_FACE); aFaceExplorer.More(); aFaceExplorer.Next()){
    TopoDS_Face aFace = TopoDS::Face(aFaceExplorer.Current());
    // Check if <aFace> is the top face of the bottle’s neck
    Handle(Geom_Surface) aSurface = BRep_Tool::Surface(aFace);
    if(aSurface->DynamicType() == STANDARD_TYPE(Geom_Plane)){
      Handle(Geom_Plane) aPlane = Handle(Geom_Plane)::DownCast(aSurface);
      gp_Pnt aPnt = aPlane->Location();
      Standard_Real aZ   = aPnt.Z();
      if(aZ > zMax){
        zMax = aZ;
        faceToRemove = aFace;
      }
    }
  }

  TopTools_ListOfShape facesToRemove;
  facesToRemove.Append(faceToRemove);
  myBody = BRepOffsetAPI_MakeThickSolid(myBody, facesToRemove, -myThickness / 50, 1.e-3);
  // Threading : Create Surfaces
  Handle(Geom_CylindricalSurface) aCyl1 = new Geom_CylindricalSurface(neckAx2, myNeckRadius * 0.99);
  Handle(Geom_CylindricalSurface) aCyl2 = new Geom_CylindricalSurface(neckAx2, myNeckRadius * 1.05);

  // Threading : Define 2D Curves
  gp_Pnt2d aPnt(2. * M_PI, myNeckHeight / 2.);
  gp_Dir2d aDir(2. * M_PI, myNeckHeight / 4.);
  gp_Ax2d anAx2d(aPnt, aDir);

  Standard_Real aMajor = 2. * M_PI;
  Standard_Real aMinor = myNeckHeight / 10;

  Handle(Geom2d_Ellipse) anEllipse1 = new Geom2d_Ellipse(anAx2d, aMajor, aMinor);
  Handle(Geom2d_Ellipse) anEllipse2 = new Geom2d_Ellipse(anAx2d, aMajor, aMinor / 4);
  Handle(Geom2d_TrimmedCurve) anArc1 = new Geom2d_TrimmedCurve(anEllipse1, 0, M_PI);
  Handle(Geom2d_TrimmedCurve) anArc2 = new Geom2d_TrimmedCurve(anEllipse2, 0, M_PI);
  gp_Pnt2d anEllipsePnt1 = anEllipse1->Value(0);
  gp_Pnt2d anEllipsePnt2 = anEllipse1->Value(M_PI);

  Handle(Geom2d_TrimmedCurve) aSegment = GCE2d_MakeSegment(anEllipsePnt1, anEllipsePnt2);
  // Threading : Build Edges and Wires
  TopoDS_Edge anEdge1OnSurf1 = BRepBuilderAPI_MakeEdge(anArc1, aCyl1);
  TopoDS_Edge anEdge2OnSurf1 = BRepBuilderAPI_MakeEdge(aSegment, aCyl1);
  TopoDS_Edge anEdge1OnSurf2 = BRepBuilderAPI_MakeEdge(anArc2, aCyl2);
  TopoDS_Edge anEdge2OnSurf2 = BRepBuilderAPI_MakeEdge(aSegment, aCyl2);
  TopoDS_Wire threadingWire1 = BRepBuilderAPI_MakeWire(anEdge1OnSurf1, anEdge2OnSurf1);
  TopoDS_Wire threadingWire2 = BRepBuilderAPI_MakeWire(anEdge1OnSurf2, anEdge2OnSurf2);
  BRepLib::BuildCurves3d(threadingWire1);
  BRepLib::BuildCurves3d(threadingWire2);

  // Create Threading
  BRepOffsetAPI_ThruSections aTool(Standard_True);
  aTool.AddWire(threadingWire1);
  aTool.AddWire(threadingWire2);
  aTool.CheckCompatibility(Standard_False);

  TopoDS_Shape myThreading = aTool.Shape();

  // Building the Resulting Compound
  TopoDS_Compound aRes;
  BRep_Builder aBuilder;
  aBuilder.MakeCompound (aRes);
  aBuilder.Add (aRes, myBody);
  aBuilder.Add (aRes, myThreading);

  return aRes;
}

//function to convert opencascade shape to osg geometry
osg::ref_ptr<osg::Geometry> createGeometryFromShape(TopoDS_Shape& shape, const osg::Vec3& geomColor, gp_Trsf& transformation)
{
    // vector to save vertices
    osg::ref_ptr<osg::Vec3Array> vertexList = new osg::Vec3Array();


    osg::Array::Binding colorBinding = osg::Array::BIND_OVERALL;

    // vector to save _colorTool
    osg::ref_ptr<osg::Vec3Array> colorList = new osg::Vec3Array();
    if (colorBinding==osg::Array::BIND_OVERALL)
    {
        colorList->push_back(geomColor);
    }

    // create one osg primitive set
    osg::ref_ptr<osg::DrawElementsUInt> triangleStrip = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);

    osg::ref_ptr<osg::Vec3Array> normalArray = new osg::Vec3Array;

    unsigned int noOfTriangles = 0;

    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

    // large vertex datasets work best with VBO.
    geom->setUseVertexBufferObjects(true);

    if(!shape.IsNull())
    {
        // clean any previous triangulation
        BRepTools::Clean(shape);

        //_healShape(shape);

        #ifdef _LOG_DEBUG_
            std::cout << std::endl << "Building a Mesh !!" ;
        #endif

        /// call to incremental mesh on this shape
        /// \todo not sure why this 1 is passed. Its called deflection BTW
        ///       need to find a way to calculate it
        double linearDeflection = 1.0;
        BRepMesh_IncrementalMesh(shape, linearDeflection);

        ///iterate faces
        // this variable will help in keeping track of face indices
        unsigned int index = 0;
        for (TopExp_Explorer ex(shape, TopAbs_FACE); ex.More(); ex.Next())
        {
            TopoDS_Face face = TopoDS::Face(ex.Current());
            TopLoc_Location location;

            /// triangulate current face
            Handle (Poly_Triangulation) triangulation = BRep_Tool::Triangulation(face, location);
            if (!triangulation.IsNull())
            {
                int noOfNodes = triangulation->NbNodes();

                // Store vertices. Build vertex array here
                for(int j = 1; j <= triangulation->NbNodes(); j++)
                {
                    // populate vertex list
                    // Ref: http://www.opencascade.org/org/forum/thread_16694/?forum=3
                    gp_Pnt pt = (triangulation->Nodes())(j).Transformed(transformation * location.Transformation());
                    vertexList->push_back(osg::Vec3(pt.X(), pt.Y(), pt.Z()));
                   // std::cout<<pt.X()<<"\t"<<pt.Y()<<"\t"<<pt.Z()<<std::endl;
                    // populate color list
                    if (colorBinding==osg::Array::BIND_PER_VERTEX)
                    {
                        colorList->push_back(geomColor);
                    }
                }

                /// now we need to get face indices for triangles
                // get list of triangle first
                const Poly_Array1OfTriangle& triangles = triangulation->Triangles();

                //No of triangles in this triangulation
                noOfTriangles = triangulation->NbTriangles();

                Standard_Integer v1, v2, v3;
                for (unsigned int j = 1; j <= noOfTriangles; j++)
                {
                    /// If face direction is reversed then we add verticews in reverse order
                    /// order of vertices is important for normal calculation later
                    if (face.Orientation() == TopAbs_REVERSED)
                    {
                        triangles(j).Get(v1, v3, v2);
                    }
                    else
                    {
                        triangles(j).Get(v1, v2, v3);
                    }

                    gp_XYZ aPnt1 = (triangulation->Nodes())(v1).XYZ();
                    gp_XYZ aPnt2 = (triangulation->Nodes())(v2).XYZ();
                    gp_XYZ aPnt3 = (triangulation->Nodes())(v3).XYZ();

                    gp_XYZ aV12 = aPnt2 - aPnt1;
                    gp_XYZ aV23 = aPnt3 - aPnt2;
                    gp_XYZ aNor = aV12^aV23;

                    normalArray->push_back(osg::Vec3(aNor.X(),aNor.Y(),aNor.Z()));


                    triangleStrip->push_back(index + v1 - 1);
                    triangleStrip->push_back(index + v2 - 1);
                    triangleStrip->push_back(index + v3 - 1);
                }
                index = index + noOfNodes;
            }
        }

        #ifdef _LOG_DEBUG_
            std::cout << "Creating a geometry.." << std::endl;
        #endif

        geom->setVertexArray(vertexList.get());
        geom->setVertexAttribArray(0, geom->getVertexArray(), osg::Array::BIND_PER_VERTEX);
        geom->setColorArray(colorList.get(), colorBinding);
        geom->setVertexAttribArray(2, geom->getColorArray(), osg::Array::BIND_OVERALL);
       /* osg::ref_ptr<osg::StateSet> stateSet = geom->getOrCreateStateSet();
        osg::ref_ptr<osg::Program> program = new osg::Program;

        osg::ref_ptr<osg::Shader> vertShader = new osg::Shader(osg::Shader::VERTEX);
        if (!vertShader->loadShaderSourceFromFile("../shaders/myShader.vert"))
            std::cerr << "Could not read VERTEX shader from file" << std::endl;
        program->addShader(vertShader);

        osg::ref_ptr<osg::Shader> fragShader = new osg::Shader(osg::Shader::FRAGMENT);
        if (!fragShader->loadShaderSourceFromFile("../shaders/myShader.frag"))
            std::cerr << "Could not read FRAGMENT shader from file" << std::endl;
        program->addShader(fragShader);

        stateSet->setAttributeAndModes(program.get(), osg::StateAttribute::ON);*/


        #ifdef _LOG_DEBUG_
            std::cout << "Adding Primitive set" << std::endl;
        #endif
        geom->setNormalArray(normalArray.get());
        geom->setVertexAttribArray(1, geom->getNormalArray(), osg::Array::BIND_PER_VERTEX);
        geom->addPrimitiveSet(triangleStrip.get());
    }

    return geom;
}

//must be clubbed as a single class instead of helper functions
osg::ref_ptr<osg::Geode> createBottle(double width,double height,double thickness,const osg::Vec3f& geomColor/*,const osg::Matrixd& viewMatrix*/)
{
    gp_Trsf transform;

//    transform.SetValues((Standard_Real)viewMatrix(0,0),
//                        (Standard_Real)viewMatrix(0,1),
//                        (Standard_Real)viewMatrix(0,2),
//                        (Standard_Real)viewMatrix(0,3),
//                        (Standard_Real)viewMatrix(1,0),
//                        (Standard_Real)viewMatrix(1,1),
//                        (Standard_Real)viewMatrix(1,2),
//                        (Standard_Real)viewMatrix(1,3),
//                        (Standard_Real)viewMatrix(2,0),
//                        (Standard_Real)viewMatrix(2,1),
//                        (Standard_Real)viewMatrix(2,2),
//                        (Standard_Real)viewMatrix(2,3));
    TopoDS_Shape shape = MakeBottle((Standard_Real)width,(Standard_Real)height,(Standard_Real)thickness);
    osg::ref_ptr<osg::Geometry> geom = createGeometryFromShape(shape,geomColor,transform);
    osg::ref_ptr<osg::Drawable> sd = (osg::ref_ptr<osg::Drawable>)geom->asDrawable();
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    osg::ref_ptr<osg::Group> root = new osg::Group();

    geode->addDrawable(sd);
    root->addChild(geode);
    osg::ref_ptr<osg::StateSet> stateSet = geode->getOrCreateStateSet();
    osg::ref_ptr<osg::Material> material = new osg::Material;
    material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
    stateSet->setAttributeAndModes( material, osg::StateAttribute::ON );
    stateSet->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );
    return geode;
}
