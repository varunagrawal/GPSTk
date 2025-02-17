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
 * @file RinexMetData.hpp
 * Encapsulates RINEX 2 & 3 Met file data, including I/O.
 */

#ifndef GPSTK_RINEXMETDATA_HPP
#define GPSTK_RINEXMETDATA_HPP

#include <map>

#include "CommonTime.hpp"
#include "FFStream.hpp"
#include "RinexMetBase.hpp"
#include "RinexMetHeader.hpp"

namespace gpstk
{
      /// @ingroup FileHandling
      //@{

      /**
       * This class stores, reads, and writes RINEX 2 & 3 Met records.
       * @warning When writing a RinexMetData, the RinexMetStream::headerData
       * must have the correct observation types set or else no data will
       * be written.
       *
       * @sa rinex_met_read_write.cpp for an example.
       * @sa rinex_met_test.cpp for an example.
       * @sa RinexMetStream.
       * @sa RinexMetHeader for information on writing RINEX 2 & 3 Met files.
       */
   class RinexMetData : public RinexMetBase
   {

   public:

      RinexMetData()
            : time(gpstk::CommonTime::BEGINNING_OF_TIME)
      {}

         /// RinexMetData is "data" so this function always returns true.
      virtual bool isData(void) const { return true; }

         /**
          * Returns a string representation of the data in this record.
          * Interface stability: Committed
          */
      virtual std::string stableText() const;

         /**
          * A debug output function.
          * Interface stability: Volatile
          */
      virtual void dump(std::ostream& s) const;

         /// less-than operator, for use with STL sort()
      bool operator<(const RinexMetData& right) const
      { return (time < right.time); }

         /// A map for storing one line of observations,
         /// mapping the observation type to its value.
      typedef std::map<RinexMetHeader::RinexMetType, double> RinexMetMap;

      CommonTime time;   ///< The time this data was recorded, in GPS time system.
      RinexMetMap data;  ///< The data itself in map form.

         /// The maximum number of obs per line before you need a new line
      static const int maxObsPerLine;
         /// The max number of obs per continuation line before you need
         /// a new line.
      static const int maxObsPerContinuationLine;

   protected:

         /** Writes the met data to the file stream formatted correctly.
          * @throw std::exception
          * @throw FFStreamError
          * @throw StringUtils::StringException
          */
      void reallyPutRecord(FFStream& s) const;

         /**
          * This function retrieves a RINEX 2 or 3 Met record from the
          * given FFStream.  If an error is encountered reading from
          * the stream, the stream is returned to its original
          * position and its fail-bit is set.
          * @throw std::exception
          * @throw StringException when a StringUtils function fails
          * @throw FFStreamError when exceptions(failbit) is set and a
          *         read or formatting error occurs.  This also resets the
          *         stream to its pre-read position.
          */
      virtual void reallyGetRecord(FFStream& s);

   private:

         /** Parses string \a line to get time and met data
          * @param version of Rinex file (3.02, 3.01, 2.11, ...)
          * @throw FFStreamError
          */
      void processFirstLine(const std::string& line,
                            const RinexMetHeader& hdr,
                            double version);

         /** Parses string \a line to get data on continuation lines.
          * @throw FFStreamError
          */
      void processContinuationLine(const std::string& line,
                                   const RinexMetHeader& hdr);

         /** Parses the time portion of a line into a CommonTime object.
          * @param version of Rinex file (3.02, 3.01, 2.11, ...)
          * @throw FFStreamError
          */
      CommonTime parseTime(const std::string& line, double version) const;

         /** Writes the CommonTime object into RINEX format. If it's a
          * bad time, it will return blanks.
          * @param version Rinex version, default to 2.11 for
          *   backwards compatability
          * @throw StringUtils::StringException
          */
      std::string writeTime(const CommonTime& dtd, double version = 2.11) const;

   };  // class RinexMetData

      //@}

} // namespace


#endif
