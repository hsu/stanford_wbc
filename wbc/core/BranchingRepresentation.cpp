/*
 * Stanford Whole-Body Control Framework http://stanford-wbc.sourceforge.net/
 *
 * Copyright (c) 1997-2009 Stanford University. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>
 */

//==========================================================================================
/*!
  \author			Luis Sentis
  \file				BranchingRepresentation.cpp
*/
//==========================================================================================

#include <wbc/core/BranchingRepresentation.hpp>
#include <tao/dynamics/taoNode.h>
#include <tao/dynamics/taoDynamics.h>
#include <tao/dynamics/taoJoint.h> 
#include <tao/utility/TaoDeMassProp.h>
#include <iostream>
#include <sstream>

namespace wbc {

  const SAIVector BranchingRepresentation::zero3 = SAIVector(3);
  
  
  BranchingRepresentation::
  BranchingRepresentation()
    : numJoints_(0), totalMass_(0.0), grav_(3)
  {
  }


  /** \todo how about deleting rootNode_ and tons of other stuff? */
  BranchingRepresentation::
  ~BranchingRepresentation()
  {
  }


  const SAIVector& 
  BranchingRepresentation::forceSensorWRTJointFrame( taoDNode const * node )
    const
  {
    std::map<taoDNode const *, SAIVector>::const_iterator
      ii(linkToSensorMap_.find(node));
    if (ii == linkToSensorMap_.end())
      return zero3;
    return ii->second;
  }
  
  
  double 
  BranchingRepresentation::surfaceDepth( taoDNode const * node )
    const
  {
    std::map<taoDNode const *, double>::const_iterator
      ii(linkToSurfaceDepthMap_.find(node));
    if (ii == linkToSurfaceDepthMap_.end())
      return 0;
    return ii->second;
  }


  taoDNode* 
  BranchingRepresentation::findNodeID( taoDNode* node, int id ) {

    deInt tmpID = node->getID();
    if( tmpID == id ) return node;
    taoDNode* foundNode = NULL;	
    for( taoDNode* n = node->getDChild(); n!= NULL && foundNode == NULL; n = node->getDSibling() )
      foundNode = findNodeID( n, id );
    return foundNode;
  }
  
  
  /**
     \todo Reimplement with a dictionary.
  */
  string
  BranchingRepresentation::canonicalJointName( const string &jointName ) {

    string tag("Null_Joint_Tag");

    if( jointName == "shoulder-yaw" )
      tag = "Shoulder_Yaw";
    else if( jointName == "shoulder-pitch" )
      tag = "Shoulder_Pitch";
    else if( jointName == "shoulder-roll" )
      tag = "Shoulder_Roll";
    else if( jointName == "wrist_pitch" )
      tag = "Wrist_Pitch";
    
    return tag;
  }


  /**
     \todo Reimplement with a dictionary.
  */
  string
  BranchingRepresentation::canonicalLinkName( const string &linkName ) {

    string tag("Null_Link_Tag");

    if( linkName == "upper-arm" )
      tag = "Upper_Arm";
    else if( linkName == "lower-arm" )
      tag = "Lower_Arm";
    else if( linkName == "end-effector" ) 
      tag = "End_Effector";
    else if( linkName == "right-hand" ) 
      tag = "Right_Hand";
    else if( linkName == "left-hand" ) 
      tag = "Left_Hand";
    else if( linkName == "hip" ) 
      tag = "Hip";
    else if( linkName == "right-foot" ) 
      tag = "Right_Foot";
    else if( linkName == "left-foot" ) 
      tag = "Left_Foot";
    else if( linkName == "right-lower-leg" ) 
      tag = "Right_Lower_Leg";
    else if( linkName == "left-lower-leg" ) 
      tag = "Left_Lower_Leg";
    
    return tag;
  }
  
  
  taoDNode * BranchingRepresentation::
  findLink( std::string const & name )
  {
    std::map<std::string, taoDNode*>::iterator ii(linkNameToNodeMap_.find(name));
    if (ii == linkNameToNodeMap_.end())
      return 0;
    return ii->second;
  }
  
  
  taoDNode * BranchingRepresentation::
  findJoint( std::string const & name )
  {
    std::map<std::string, taoDNode*>::iterator ii(jointNameToNodeMap_.find(name));
    if (ii == jointNameToNodeMap_.end())
      return 0;
    return ii->second;
  }
  
  
  void BranchingRepresentation::
  setRootName(std::string const & root_name)
    throw(std::runtime_error)
  {
    idToNodeMap_t::const_iterator node(idToNodeMap_.find(-1));
    if (idToNodeMap_.end() == node) {
      throw runtime_error("wbc::BranchingRepresentation::setRootName(" + root_name + "): no root node");
    }
    linkNameToNodeMap_[root_name] = node->second;
    linkNameToNodeMap_[canonicalLinkName(root_name)] = node->second;
  }
  
  
  void BranchingRepresentation::
  setLinkNames(std::vector<std::string> const & link_names)
    throw(std::runtime_error)
  {
    for (size_t ii(0); ii < link_names.size(); ++ii) {
      idToNodeMap_t::const_iterator node(idToNodeMap_.find(ii));
      if (idToNodeMap_.end() == node) {
	ostringstream msg;
	msg << "wbc::BranchingRepresentation::setLinkNames(): no node with ID " << ii;
	throw runtime_error(msg.str());
      }
      linkNameToNodeMap_[link_names[ii]] = node->second;
      linkNameToNodeMap_[canonicalLinkName(link_names[ii])] = node->second;
    }
  }
  
  
  void BranchingRepresentation::
  setJointNames(std::vector<std::string> const & joint_names)
    throw(std::runtime_error)
  {
    for (size_t ii(0); ii < joint_names.size(); ++ii) {
      idToNodeMap_t::const_iterator node(idToNodeMap_.find(ii));
      if (idToNodeMap_.end() == node) {
	ostringstream msg;
	msg << "wbc::BranchingRepresentation::setJointNames(): no node with ID " << ii;
	throw runtime_error(msg.str());
      }
      jointNameToNodeMap_[joint_names[ii]] = node->second;
      jointNameToNodeMap_[canonicalJointName(joint_names[ii])] = node->second;
    }
  }
  
}