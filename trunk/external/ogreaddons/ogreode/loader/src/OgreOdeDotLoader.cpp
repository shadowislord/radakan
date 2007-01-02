#include "OgreOde_Core.h"
#include "OgreOde_Prefab.h"
#include "OgreOde_Loader.h"

#include "tinyxml.h"

using namespace Ogre;
using namespace OgreOde;
using namespace OgreOde_Prefab;
using namespace OgreOde_Loader;

//------------------------------------------------------------------------------------------------
static const TiXmlElement *getFirstOgreOde(const TiXmlElement *root);
static const TiXmlElement *getFirstOgreOde(const TiXmlElement *root)                       
{
    // should have some recursive algo 
    if (!strcmp(root->Value(), "ogreode"))
    {
        return root;
    }
    for (const TiXmlNode *child = root->FirstChild(); child; child = child->NextSibling())
    {
        if (child->Type() == TiXmlNode::ELEMENT)
        {
            const TiXmlElement *res = getFirstOgreOde(child->ToElement ());
            if (res)
                return res;
        }
    }
    return 0;
}

//------------------------------------------------------------------------------------------------
DotLoader::DotLoader(OgreOde::World * world, OgreOde::Space * space) :
    _world(world),
    _space (space)
{

}
//------------------------------------------------------------------------------------------------
void DotLoader::saveObject(const String &filename, const String &object_name, Object  *object)
{

}
//------------------------------------------------------------------------------------------------
void DotLoader::serializeObject(const String &filename, const String &object_name, Object  *)
{

}
//------------------------------------------------------------------------------------------------
void DotLoader::serializeVehicle(const String &filename, const String &vehicle_name, Vehicle *)
{

}
//------------------------------------------------------------------------------------------------
void DotLoader::serializeRagdoll(const String &filename, const String &ragdoll_name, Ragdoll *)
{

}
//------------------------------------------------------------------------------------------------
void DotLoader::save(const String &filename, const StringVector &objects_names, ObjectList  *)
{

}
//------------------------------------------------------------------------------------------------
ObjectList  *DotLoader::load(const String &filename, const StringVector &objects_names, const StringVector &instance_names)
{
    ObjectList* objectList = new ObjectList();

    //

    return  objectList;

}


//------------------------------------------------------------------------------------------------
Object* DotLoader::loadObject(const String &filename, const String &object_name, const String &instance_name)
{
   // load and check file
   TiXmlDocument *doc = loadFile (filename);
   const TiXmlElement *root = getFirstOgreOde(doc->RootElement());
   // search for any element
   Object* objLoading = 0;

   for (const TiXmlNode *child = root->FirstChild(); child; child = child->NextSibling() )
   {
       if (child->Type() == TiXmlNode::ELEMENT)
       {
           objLoading = parseObject (child, object_name, instance_name);
           if (objLoading)
            break;
       }
   }
   delete doc;
   return objLoading;
}

//------------------------------------------------------------------------------------------------
Object* DotLoader::parseObject(const TiXmlNode *child, const String &object_name, const String &instance_name)
{
    Object* objLoading = 0;
    if ((child->Type() == TiXmlNode::ELEMENT) && (!strcmp(child->Value(), "vehicle")))
    {
        objLoading = parseVehicle (child, object_name, instance_name);
    }
    else if ((child->Type() == TiXmlNode::ELEMENT) && (!strcmp(child->Value(), "ragdoll")))
    {
        objLoading = parseRagdoll (child, object_name, instance_name);
    }
    else if ((child->Type() == TiXmlNode::ELEMENT) && (!strcmp(child->Value(), "composite")))
    {
        objLoading = parseCompositeObject  (child, object_name, instance_name);
    }
    else if ((child->Type() == TiXmlNode::ELEMENT) && (!strcmp(child->Value(), "object")))
    {
        objLoading = parseSingleObject (child, object_name, instance_name);
    }
    return objLoading;
}

//------------------------------------------------------------------------------------------------
Object*  DotLoader::parseCompositeObject(const TiXmlNode*child, const String &object_name, const String &instance_name)
{
	Object* object = new Object(ObjectType_CompositeObject, _world);

       

	return  object;
}
//------------------------------------------------------------------------------------------------
Object*  DotLoader::parseSingleObject(const TiXmlNode*child, const String &object_name, const String &instance_name)
{
    Object* object = new Object(ObjectType_Single_Object, _world);



    return  object;
}
//------------------------------------------------------------------------------------------------
Vehicle* DotLoader::parseVehicle(const TiXmlNode* child, const String &vehicle_name, const String &instance_name)
{
	String my_name = vehicle_name;
	Vehicle *vehicle = 0;

	const TiXmlElement *vehicleElement = (const TiXmlElement*)child;
	if (vehicleElement->Attribute("name") && (!strcmp(vehicleElement->Attribute("name"),my_name.c_str())))
	{

        String instanceFinalName ((instance_name == StringUtil::BLANK) ?
            vehicle_name + StringConverter::toString(OgreOde_Prefab::Object::getInstanceNumberAndIncrement ())
            : instance_name);

        vehicle = new Vehicle(instanceFinalName, _world, _space);

		for (const TiXmlNode *tag = child->FirstChild(); tag; tag = tag->NextSibling() )
		{
			if (tag->Type() == TiXmlNode::ELEMENT)
			{
				const TiXmlElement *element = (const TiXmlElement*)tag;

				if (!strcmp(tag->Value(),"body"))
				{
					if (element->Attribute("mesh"))
					{
						MeshManager::getSingleton().load(
							element->Attribute("mesh"),
							// note that you can change the group by pre-loading the mesh
							ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME );
						vehicle->setEntity (_world->getSceneManager()->createEntity(
							vehicle->getName() + Ogre::StringConverter::toString (vehicle->getInstanceNumber()) +  "_Entity", 
							element->Attribute("mesh")));
						vehicle->getEntity()->setCastShadows(true);

						vehicle->getTransNode()->attachObject(vehicle->getEntity());

						EntityInformer ei(vehicle->getEntity());
						Vector3 box = ei.getSize();

						if (!vehicle->getGeometry()) 
							vehicle->setGeometry(new BoxGeometry(box, _world, _space));
						vehicle->getGeometry()->setUserObject(vehicle);

						vehicle->setTransformGeometry(new TransformGeometry(_world, vehicle->getSpace()));
						vehicle->getTransformGeometry()->setEncapsulatedGeometry(vehicle->getGeometry());
						vehicle->getTransformGeometry()->setBody(vehicle->getBody());
					}

					for (const TiXmlNode *sub_tag = tag->FirstChild(); sub_tag; sub_tag = sub_tag->NextSibling() )
					{
						if (sub_tag->Type() == TiXmlNode::ELEMENT)
						{
							const TiXmlElement *sub_element = (const TiXmlElement*)sub_tag;

							if (vehicle->getEntity() && (!strcmp(sub_tag->Value(),"mass")))
							{
								double tmp;
								if (sub_element->Attribute("value",&tmp))
								{
									EntityInformer ei(vehicle->getEntity());
									Vector3 box = ei.getSize();

									BoxMass box_mass((Real)tmp,Vector3(box.x,box.y,box.z));
									vehicle->getBody()->setMass(box_mass);
								}

								Vector3 offset = Ogre::Vector3::ZERO;

								if (sub_element->Attribute("x",&tmp)) offset.x = (Real)tmp; 
								if (sub_element->Attribute("y",&tmp)) offset.y = (Real)tmp; 
								if (sub_element->Attribute("z",&tmp)) offset.z = (Real)tmp; 

								vehicle->setOffset(-offset);
							}
						}
					}
				}
				else if (!strcmp(tag->Value(),"wheel"))
				{
					Vehicle::Wheel *wheel = 0;
					double x,y,z,mass;

					if (element->Attribute("mesh"))
					{
						wheel = vehicle->addWheel(element->Attribute("mesh"),
										Vector3((element->Attribute("x",&x))?(Real)x:0.0,
												(element->Attribute("y",&y))?(Real)y:0.0,
												(element->Attribute("z",&z))?(Real)z:0.0),
										(element->Attribute("mass",&mass))?(Real)mass:1.0);
					}

					if (wheel)
					{
						for (const TiXmlNode *sub_tag = tag->FirstChild(); sub_tag; sub_tag = sub_tag->NextSibling() )
						{
							if (sub_tag->Type() == TiXmlNode::ELEMENT)
							{
								const TiXmlElement *sub_element = (const TiXmlElement*)sub_tag;

								if (!strcmp(sub_tag->Value(),"steer"))
								{
									double tmp;
									if (sub_element->Attribute("factor",&tmp)) wheel->setSteerFactor(tmp);
									if (sub_element->Attribute("limit",&tmp)) wheel->setSteerLimit(tmp);
									if (sub_element->Attribute("force",&tmp)) wheel->setSteerForce(tmp);
									if (sub_element->Attribute("speed",&tmp)) wheel->setSteerSpeed(tmp);
								}
								else if (!strcmp(sub_tag->Value(),"power"))
								{
									double tmp;
									if (sub_element->Attribute("factor",&tmp)) wheel->setPowerFactor(tmp);
								}
								else if (!strcmp(sub_tag->Value(),"brake"))
								{
									double tmp;
									if (sub_element->Attribute("factor",&tmp)) wheel->setBrakeFactor(tmp);
								}
								else if (!strcmp(sub_tag->Value(),"contact"))
								{
									double bouncyness,friction,fds;
									wheel->setContact((sub_element->Attribute("bouncyness",&bouncyness))?(Real)bouncyness:0.0,(sub_element->Attribute("friction",&friction))?(Real)friction:0.0,(sub_element->Attribute("fds",&fds))?(Real)fds:0.0);
								}
								else if (!strcmp(sub_tag->Value(),"suspension"))
								{
									double spring,damping,step;
									vehicle->setSuspension((sub_element->Attribute("spring",&spring))?(Real)spring:0.0,(sub_element->Attribute("damping",&damping))?(Real)damping:0.0,(sub_element->Attribute("step",&step))?(Real)step:0.0);
								}
							}
						}
					}
				}
				else if (!strcmp(tag->Value(),"suspension"))
				{
					double spring,damping,step;
					vehicle->setSuspension((element->Attribute("spring",&spring))?(Real)spring:0.0,(element->Attribute("damping",&damping))?(Real)damping:0.0,(element->Attribute("step",&step))?(Real)step:0.0);
				}
				else if (!strcmp(tag->Value(),"engine"))
				{
					double tmp;

					if (element->Attribute("redline",&tmp)) vehicle->getEngine()->setRevLimit((Real)tmp);
					if (element->Attribute("brake",&tmp)) vehicle->getEngine()->setBrakeForce((Real)tmp);

					static const unsigned int MAX_SAMPLES = 255;
					Real torqueCurve[MAX_SAMPLES];
					unsigned int torqueSamples = 0;
					for (const TiXmlNode *sub_tag = tag->FirstChild(); sub_tag; sub_tag = sub_tag->NextSibling() )
					{
						if (sub_tag->Type() == TiXmlNode::ELEMENT)
						{
							const TiXmlElement *sub_element = (const TiXmlElement*)sub_tag;

							if (!strcmp(sub_tag->Value(),"torque"))
							{
								double min_torque,max_torque;
								vehicle->getEngine()->setTorque((sub_element->Attribute("min",&min_torque))?min_torque:0.0,(sub_element->Attribute("max",&max_torque))?max_torque:0.0);
							}
							else if (!strcmp(sub_tag->Value(),"torquecurve"))
							{
								double curveValue;
								if( sub_element->Attribute( "value", &curveValue ) )
								{
									if( torqueSamples < MAX_SAMPLES )
										torqueCurve[torqueSamples++] = curveValue;
								}
							}
						}
					}
					if( torqueSamples )
					{
						vehicle->getEngine()->setTorque( torqueCurve, torqueSamples );
					}
				}
				else if (!strcmp(tag->Value(),"antisway"))
				{
					vehicle->enableAntiSway (true);

					double dtmp;
					if (element->Attribute("swayForce",&dtmp)) 
						vehicle->setSwayForce((Real) dtmp);

					int uitmp;
					if (element->Attribute("forceLimit",&uitmp)) 
						vehicle->setSwayForceLimit((unsigned int) uitmp);

					if (element->Attribute("rate",&dtmp)) 
						vehicle->setSwayForceRate((Real) dtmp);
				}
			}
		}
	}

	

    if (vehicle)
    {
        if (vehicle->isAntiSwayEnabled() && vehicle->getWheelCount() != 4)
	    {
		    assert (0 && "no sway for non 4 wheels");
		    vehicle->enableAntiSway (false);
	    }
    }
	return vehicle;
}
//------------------------------------------------------------------------------------------------
Ragdoll* DotLoader::parseRagdoll(const TiXmlNode *child, const String &ragdoll_name, const String &instance_name)
{
    Ragdoll *ragdoll = 0;

	const TiXmlElement *ragdollElement = (const TiXmlElement*)child;
	if ((ragdoll_name == StringUtil::BLANK) || ((ragdollElement->Attribute("name")) 
		&& (!strcmp(ragdollElement->Attribute("name"), ragdoll_name.c_str()))))
	{
        const String meshFileName (ragdollElement->Attribute("meshName"));

        String instanceFinalName ((instance_name == StringUtil::BLANK) ?
             meshFileName + StringConverter::toString(OgreOde_Prefab::Object::getInstanceNumberAndIncrement ())
            : instance_name);
  

        NameValuePairList params;
        params["mesh"] = meshFileName;
        ragdoll = static_cast<OgreOde_Prefab::Ragdoll*>
            (_world->getSceneManager ()->createMovableObject(instanceFinalName, 
                                                                            OgreOde_Prefab::RagdollFactory::FACTORY_TYPE_NAME,
                                                                            &params)); 

		Ragdoll::BoneSettings settings;

		for (const TiXmlNode *tag = child->FirstChild(); tag; tag = tag->NextSibling() )
		{
			if (tag->Type() == TiXmlNode::ELEMENT)
			{
                const TiXmlElement *element = (const TiXmlElement*)tag;
				if (!strcmp(tag->Value(),"defaults"))
				{
					parseRagdollSettings(&settings,tag);
					ragdoll->setDefaultBoneSettings(settings);
				}
				else if ((!strcmp(tag->Value(),"bone")) 
                        && (element->Attribute("name")))
				{
					parseRagdollSettings(&settings,tag);
					ragdoll->setBoneSettings(element->Attribute("name"),settings); 
				}
			}
		}
	}
	return ragdoll;
}

//------------------------------------------------------------------------------------------------
void DotLoader::parseRagdollSettings(Ragdoll::BoneSettings *bone_settings,const void *tag) const
{
    const TiXmlElement *element = (const TiXmlElement *)tag;

    for (const TiXmlNode *node = element->FirstChild(); node; node = node->NextSibling() )
    {
        if ((node->Type() ==  TiXmlNode::ELEMENT) && (!stricmp(node->Value(),"settings")))
        {
            const TiXmlElement *settings = (const TiXmlElement*)node;
            double tmp;

            if (settings->Attribute("geometry"))
            {
                if (!strcmp(settings->Attribute("geometry"),"capsule")) bone_settings->_geometry_class = Geometry::Class_Capsule;
                else if (!strcmp(settings->Attribute("geometry"),"box")) bone_settings->_geometry_class = Geometry::Class_Box;
                else if (!strcmp(settings->Attribute("geometry"),"none")) bone_settings->_geometry_class = Geometry::Class_NoGeometry;
            }

            if (settings->Attribute("joint"))
            {
                if (!strcmp(settings->Attribute("joint"),"fixed")) bone_settings->_joint_type = Joint::Type_FixedJoint;
                else if (!strcmp(settings->Attribute("joint"),"hinge")) bone_settings->_joint_type = Joint::Type_HingeJoint;
                else if (!strcmp(settings->Attribute("joint"),"ball")) bone_settings->_joint_type = Joint::Type_BallJoint;
                else if (!strcmp(settings->Attribute("joint"),"universal")) bone_settings->_joint_type = Joint::Type_UniversalJoint;
            }

            if (settings->Attribute("joint"))
            {
                if (!strcmp(settings->Attribute("joint"),"fixed")) bone_settings->_joint_type = Joint::Type_FixedJoint;
                else if (!strcmp(settings->Attribute("joint"),"hinge")) bone_settings->_joint_type = Joint::Type_HingeJoint;
                else if (!strcmp(settings->Attribute("joint"),"ball")) bone_settings->_joint_type = Joint::Type_BallJoint;
                else if (!strcmp(settings->Attribute("joint"),"universal")) bone_settings->_joint_type = Joint::Type_UniversalJoint;
            }

            if (settings->Attribute("collapse"))
            {
                if (!strcmp(settings->Attribute("collapse"),"none")) bone_settings->_collapse = Ragdoll::BoneSettings::Collapse_None;
                else if (!strcmp(settings->Attribute("collapse"),"up")) bone_settings->_collapse = Ragdoll::BoneSettings::Collapse_Up;
                else if (!strcmp(settings->Attribute("collapse"),"down")) bone_settings->_collapse = Ragdoll::BoneSettings::Collapse_Down;
            }

            if (settings->Attribute("mass",&tmp)) bone_settings->_mass = (Real)tmp;
            if (settings->Attribute("radius",&tmp)) bone_settings->_radius = (Real)tmp;

            for (const TiXmlNode *sub_node = node->FirstChild(); sub_node; sub_node = sub_node->NextSibling() )
            {
                if (sub_node->Type() ==  TiXmlNode::ELEMENT)
                {
                    const TiXmlElement *sub_settings = (const TiXmlElement*)sub_node;

                    if (!stricmp(sub_node->Value(),"damping"))
                    {
                        if (sub_settings->Attribute("angular",&tmp)) bone_settings->_angular_damping = (Real)tmp;
                        if (sub_settings->Attribute("linear",&tmp)) bone_settings->_linear_damping = (Real)tmp;
                    }
                    else if (!stricmp(sub_node->Value(),"axis"))
                    {
                        int n;
                        if (sub_settings->Attribute("number",&n))
                        {
                            if (n == 1)
                            {
                                if (sub_settings->Attribute("x",&tmp)) bone_settings->_primary_axis.x = (Real)tmp;
                                if (sub_settings->Attribute("y",&tmp)) bone_settings->_primary_axis.y = (Real)tmp;
                                if (sub_settings->Attribute("z",&tmp)) bone_settings->_primary_axis.z = (Real)tmp;

                                bone_settings->_primary_axis.normalise();
                            }
                            else if (n == 2)
                            {
                                if (sub_settings->Attribute("x",&tmp)) bone_settings->_secondary_axis.x = (Real)tmp;
                                if (sub_settings->Attribute("y",&tmp)) bone_settings->_secondary_axis.y = (Real)tmp;
                                if (sub_settings->Attribute("z",&tmp)) bone_settings->_secondary_axis.z = (Real)tmp;

                                bone_settings->_secondary_axis.normalise();
                            }

                            for (const TiXmlNode *sub_sub_node = sub_node->FirstChild(); sub_sub_node; sub_sub_node = sub_sub_node->NextSibling() )
                            {
                                if ((sub_sub_node->Type() ==  TiXmlNode::ELEMENT) && (!stricmp(sub_sub_node->Value(),"stop")))
                                {
                                    const TiXmlElement *sub_sub_settings = (const TiXmlElement*)sub_sub_node;

                                    if (n == 1)
                                    {
                                        if (sub_sub_settings->Attribute("low",&tmp)) bone_settings->_primary_lostop = (Real)tmp;
                                        if (sub_sub_settings->Attribute("high",&tmp)) bone_settings->_primary_histop = (Real)tmp;
                                    }
                                    else if (n == 2)
                                    {
                                        if (sub_sub_settings->Attribute("low",&tmp)) bone_settings->_secondary_lostop = (Real)tmp;
                                        if (sub_sub_settings->Attribute("high",&tmp)) bone_settings->_secondary_histop = (Real)tmp;
                                    }
                                }	
                            }
                        }
                    }
                }
            }
        }
    }
}

//------------------------------------------------------------------------------------------------
TiXmlDocument * DotLoader::loadFile(const String &filename)
{
    // load and check file
    DataStreamPtr file = ResourceGroupManager::getSingleton().openResource(filename);
    TiXmlDocument *doc = new TiXmlDocument(filename.c_str());
    doc->Parse(file->getAsString().c_str());
    if (doc->Error())
    {
        throw new Exception(doc->ErrorId(),
            doc->ErrorDesc(),
            __FILE__,
            ((char*)(file->getName().c_str())),
            doc->ErrorRow());
    }

    // get the ogreode element.
    TiXmlNode* node = doc->FirstChild( "ogreode" );
    if (!node)
    {
        Ogre::LogManager::getSingleton().logMessage("  cannot find ogreode root in XML file!");
        return 0;
    }
    return doc;
}
