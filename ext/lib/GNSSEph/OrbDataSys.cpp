//==============================================================================
//
//  This file is part of GPSTk, the GPS Toolkit.
//
//  The GPSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GPSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GPSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//  
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin.
//  Copyright 2004-2020, The Board of Regents of The University of Texas System
//
//==============================================================================

//==============================================================================
//
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin, under contract to an agency or agencies
//  within the U.S. Department of Defense. The U.S. Government retains all
//  rights to use, duplicate, distribute, disclose, or release this software.
//
//  Pursuant to DoD Directive 523024 
//
//  DISTRIBUTION STATEMENT A: This software has been approved for public 
//                            release, distribution is unlimited.
//
//==============================================================================

/**
 * @file OrbDataSys.cpp
 * OrbDataSys data encapsulated in engineering terms
 */

#include "OrbDataSys.hpp"

namespace gpstk
{
   using namespace std;
   using namespace gpstk;
   OrbDataSys::OrbDataSys(): OrbData(), UID(0)
   { }

   bool OrbDataSys::isSameData(const OrbData* right) const
   {
         // First, test whether the test object is actually a OrbDataSys object.
      const OrbDataSys* p = dynamic_cast<const OrbDataSys*>(right);
      if (p==0) return false; 

      if (!OrbData::isSameData(right))  return false; 
      if (UID != p->UID)            return false;
      return true;
   }

   std::list<std::string> OrbDataSys::compare(const OrbDataSys* right) const
   {
      std::list<std::string> retList = OrbData::compare(right);
      if (UID            != right->UID)      retList.push_back("UID");
      return retList;
   }

   void OrbDataSys::dumpFooter(std::ostream& s) const
   { }

   std::ostream& operator<<(std::ostream& s,
                            const OrbDataSys& eph)
   {
      eph.dump(s);
      return s; 
   }

} // namespace
