#include "osghelper.h"


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
    material->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
    stateSet->setAttributeAndModes( material, osg::StateAttribute::ON );
    stateSet->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON );

    return root;
}




