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
 * @file IonexData.hpp
 * Encapsulate IONEX file data, including I/O
 */

#ifndef GPSTK_IONEXDATA_HPP
#define GPSTK_IONEXDATA_HPP

#include <map>

#include "FFStream.hpp"
#include "Position.hpp"
#include "Vector.hpp"
#include "WGS84Ellipsoid.hpp"

#include "IonexStream.hpp"



namespace gpstk
{

      /// @ingroup IonosphereMaps 
      //@{

      /** This class models a IONEX Data Record.
       *
       * @sa gpstk::IonexStream and gpstk::IonexHeader
       * @sa main ionex test.cpp for example
       */
   class IonexData : public IonexBase
   {
   public:


         /**
          * @name IonexDataFormatStrings
          * IONEX Data Formatting Strings
          */
         //@{
      static const std::string startTecMapString;     ///<"START OF TEC MAP"
      static const std::string startRmsMapString;     ///<"START OF RMS MAP"
      static const std::string startHgtMapString;     ///<"START OF HEIGHT MAP"
      static const std::string currentEpochString;    ///<"EPOCH OF CURRENT MAP"
      static const std::string dataBlockString;       ///<"LAT/LON1/LON2/DLON/H"
      static const std::string endTecMapString;       ///<"END OF TEC MAP"
      static const std::string endRmsMapString;       ///<"END OF RMS MAP"
      static const std::string endHgtMapString;       ///<"END OF HEIGHT MAP"
      static const std::string endOfFile;             ///<"END OF FILE"
         //@}


         /// A structure used to store IONEX Value Types
      struct IonexValType
      {

         std::string type;          ///< type e.g. TEC, RMS
         std::string description;   ///< Description (optional)
         std::string units;         ///< units (optional). E.g. "meters"

         IonexValType()
            : type( std::string("UN") ),
              description( std::string("Unknown or Invalid") ),
              units( std::string("") )
         {};

         IonexValType(std::string t, std::string d, std::string u)
            : type(t), description(d), units(u) {};

      }; // End of struct 'IonexValType'


         /**
          * @name Standard IONEX value types
          */
         //@{
      static const IonexValType UN;
      static const IonexValType TEC;
      static const IonexValType RMS;
         //@}


         /**
          * @name IonexDataValues
          */
         //@{
      int mapID;     ///< denote the internal number of the current map
      int dim[3];    ///< How many values are along latitude, longitude, height
      CommonTime time;  ///< the time corresponding to the current data records

      IonexValType type;      ///< Type of data either TEC or RMS
      Vector<double> data;    ///< TEC or RMS data

         // The following fields are similar to the ones in 'IonexHeader'
      int exponent;           ///< Exponent defining the unit of the values

      double lat[3];          ///< Definition of a grid in latitude
      double lon[3];          ///< Definition of a grid in longitude
      double hgt[3];          ///< Definition of a grid in height

      bool valid;             ///< Validity flag
         //@}


         /// Default constructor.
      IonexData()
         : time(CommonTime::BEGINNING_OF_TIME), valid(false) {};


         /// Destructor
      virtual ~IonexData() {};



         // The next four lines define our common interface

         /// IonexData is a "data", so this function always returns true
      virtual bool isData() const
      { return true; }


         /// Am I an valid object?
      virtual bool isValid() const
      { return valid; };


         /// A debug output function.
      virtual void dump (std::ostream& s = std::cout) const;


         /** Get the position of a grid point based on input position
          *
          * @param in     input lat, lon and height (Triple object)
          * @param type   grid point to be returned
          *               (1) neareast grid point
          *               (2) lower left hand grid point
          * @param out    output lat, lon and height (Triple object)
          * @return       the index within the data
          * @throw InvalidRequest
          * @warning Keep in mind the assumptions of IONEX grid (i.e., latitude
          *          between [87.5, -87.5], longitude between [-180, 180])
          *          when you construct a Triple object.
          */
      int getIndex( const Triple& in, const int& type, Triple& out ) const;


         /** Get IONEX TEC or RMS value as a function of the position
          *  and nominal height.
          *
          * A simple 4-point formula is applied to interpolate between
          * grid points.
          *
          * For more information see page 3 of IONEX manual:
          *
          * http://igscb.jpl.nasa.gov/igscb/data/format/ionex1.pdf
          *
          * @param pos             input position (Position object).
          *
          * @return                Computed TEC or RMS value.
          * @throw InvalidRequest
          * @throw FFStreamError
          */
      double getValue(const Position& pos) const;


   protected:

         /** Writes a correctly formatted record from this data to stream \a s.
          *
          * @throw std::exception
          * @throw FFStreamError
          * @throw StringException when a StringUtils function fails
          */
      virtual void reallyPutRecord(FFStream& s) const;


         /** This function obtains a IONEX Data record from
          *  the given FFStream.
          *
          * If there is an error reading the stream, it is reset
          * to its original position and its fail-bit is set.
          *
          * @throw std::exception
          * @throw StringException when a StringUtils function fails
          * @throw FFStreamError when exceptions(failbit) is set and
          *   a read or formatting error occurs.  This also resets the
          *   stream to its pre-read position.
          */
      virtual void reallyGetRecord(FFStream& s);


   private:


         /** Writes the CommonTime object into IONEX format. If it's a bad time,
          * it will return blanks.
          *
          * @param dt    time to be written into a IONEX data record.
          * @throw StringUtils::StringException
          */
      std::string writeTime(const CommonTime& dt) const;


         /** This function constructs a CommonTime object from the given
          *  parameters.
          *
          * @param line    Encoded time string found in the IONEX record.
          */
      CommonTime parseTime(const std::string& line) const;


   }; // End of class 'IonexData'


      /// operator == for IonexData::IonexValType
   inline bool operator==( const IonexData::IonexValType& x,
                           const IonexData::IonexValType& y )
   {return (x.type == y.type); };


      /// operator != for IonexData::IonexValType
   inline bool operator!=( const IonexData::IonexValType& x,
                           const IonexData::IonexValType& y )
   {return (x.type != y.type); };


      /// operator < for IonexData::IonexValType
   inline bool operator<( const IonexData::IonexValType& x,
                          const IonexData::IonexValType& y )
   {return (x.type < y.type); };


      /// operator << for IonexData::IonexValType
   inline std::ostream& operator<<( std::ostream& s,
                                    const IonexData::IonexValType ivt )
   {
      return s << "Type= " << ivt.type
               << ", Description= " << ivt.description
               << ", Units= " << ivt.units;
   };


      //@}


}  // End of namespace gpstk
#endif   // GPSTK_IONEXDATA_HPP
