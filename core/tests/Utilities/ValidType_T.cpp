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

#include "ValidType.hpp"
#include "TestUtil.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

using namespace gpstk;
class ValidType_T
{
public:
   ValidType_T(){ eps = 1E-15;}// Default Constructor, set the precision value
   ~ValidType_T() {} // Default Desructor

   int methodTest(void)
   {
      TUDEF( "ValidType", "isValid");
      std::string failMesg;

      ValidType<float> vfloat0;

      //Is the invalid Valid object set as valid?
      TUASSERT(!vfloat0.is_valid());

      //Is the invalid Valid object's value 0?
      TUASSERTFE(0.0f, vfloat0.get_value());

      ValidType<float> vfloat (5);

      //Does the get_value method return the correct value?
      TUASSERTFE(5.0f, vfloat.get_value());

      //Is the valid Valid object set as valid?
      TUASSERT(vfloat.is_valid());

      vfloat.set_valid(false);

      //Was the valid Valid object correctly set to invalid?
      TUASSERT(!vfloat.is_valid());

      TURETURN();
   }

   int operatorTest(void)
   {
      TUDEF( "ValidType", " == Operator");
      std::string failMesg;

      ValidType<float> Compare1 (6.);
      ValidType<float> Compare2 (6.);
      ValidType<float> Compare3 (8.);
      ValidType<int> Compare4 (6);
      ValidType<float> vfloat;

      //Are two equvalent objects equal?
      TUASSERT(Compare1 == Compare2);

      //Are two non-equvalent objects equal?
      TUASSERT(Compare1 != Compare3);

      vfloat = 7.;

      TUCSM(" = Operator");
      //Did the = operator store the value correctly?
      TUASSERT(vfloat.get_value() == 7.);

      //Did the = operator set the object as valid?
      TUASSERT(vfloat.is_valid());

      TUCSM(" += Operator");

      vfloat += 3.;
      //Did the += operator store the value correctly?
      TUASSERT(vfloat.get_value() == 10.);

      //Did the += operator change the object's valid bool?
      TUASSERT(vfloat.is_valid());

      TUCSM(" -= Operator");

      vfloat -= 5.;

      //Did the -= operator store the value correctly?
      TUASSERT(vfloat.get_value() == 5.);

      //Did the -= operator change the object's valid bool?
      TUASSERT(vfloat.is_valid());

      TUCSM(" << Operator");

      vfloat = 11;

      std::stringstream streamOutput;
      std::string stringOutput;
      std::string stringCompare;

      streamOutput <<  vfloat;
      stringOutput = streamOutput.str();

      stringCompare = "11";

      //Did the << operator ouput valid object correctly?
      TUASSERT(stringCompare == stringOutput);

      streamOutput.str("");	// Resetting stream
      vfloat.set_valid(false);

      streamOutput << vfloat;
      stringOutput = streamOutput.str();

      stringCompare = "Unknown";

      // Did the << operator output invalid object correctly?
      TUASSERT(stringCompare == stringOutput);

      TURETURN();
   }

private:
   double eps;
};

int main() //Main function to initialize and run all tests above
{
	int errorTotal = 0;
	ValidType_T testClass;

	errorTotal += testClass.methodTest();
	errorTotal += testClass.operatorTest();

	std::cout << "Total Failures for " << __FILE__ << ": " << errorTotal << std::endl;

	return errorTotal; //Return the total number of errors
}
