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
 * @file StudentDistribution.hpp
 * This class implements the t-Student distribution.
 */

#include "StudentDistribution.hpp"


using namespace std;

namespace gpstk
{


      /* Computes the probability density function
       *
       * @param x    Value
       */
   double StudentDistribution::pdf(double x)
   {

         // If ndf == 1, this is a Cauchy distribution
      if( ndf == 1 )
      {
         return ( 1.0 / ( PI * ( 1.0  + x*x ) ) );
      }


         // If ndf == 2, we use a simpler equation
      if( ndf == 2 )
      {
         double temp( 2.0 + x*x );
         return ( 1.0 / ( std::sqrt( temp * temp * temp ) ) );
      }

      double nu( static_cast<double>(ndf) );

         // Let's compute some terms
      double t1( 0.5*nu );
      double t2( t1 + 0.5 );
      double t3( std::log( std::sqrt(nu*PI) ) );

      return ( std::exp( lngamma(t2) - t2 * std::log(1.0 + x*x/nu)
                         - t3 - lngamma(t1) ) );

   }  // End of method 'StudentDistribution::pdf()'



      /* Computes the cumulative distribution function
       *
       * @param x    Value
       */
   double StudentDistribution::cdf(double x)
   {

         // If ndf == 1, this is a Cauchy distribution
      if( ndf == 1 )
      {
         return ( 0.5 + ( std::atan(x) / PI ) );
      }

         // If ndf == 2, we use a simpler equation
      if( ndf == 2 )
      {
         return ( 0.5 * ( 1.0 + x / std::sqrt( 2.0 + x*x ) ) );
      }


      double nu( static_cast<double>(ndf) );

         // Let's compute some terms
      double t1( 0.5*nu );
      double t2( std::sqrt(x*x+nu) );
      double t3( 0.5 * ( 1.0 + ( x / t2 ) )  );

      return ( regIncompleteBeta(t3, t1, t1) );

   }  // End of method 'StudentDistribution::cdf()'



      /* Set the number of degrees of freedom.
       *
       * @param n       Degrees of freedom
       *
       * \warning "n" must be > 0.0, otherwise n = |n|.
       */
   StudentDistribution& StudentDistribution::setNDF(int n)
   {

      if( n == 0 )
      {
         InvalidParameter e( "Invalid value for NDF." );
         GPSTK_THROW(e);
      }

      if( n < 0 )
      {
         ndf = -n;
      }
      else
      {
         ndf = n;
      }

      return (*this);

   }  // End of method 'StudentDistribution::setNDF()'



}  // End of namespace gpstk
