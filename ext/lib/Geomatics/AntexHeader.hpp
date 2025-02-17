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

/// @file AntexHeader.hpp
/// Encapsulate header of ANTEX antenna file, including I/O

#ifndef GPSTK_ANTEX_HEADER_HPP
#define GPSTK_ANTEX_HEADER_HPP

#include <string>
#include <vector>

#include "AntexBase.hpp"
#include "FFStream.hpp"

namespace gpstk
{
   /// @ingroup ANTEX 
   //@{

      /**
       * This class models the header for an ANTEX antenna file
       * @sa gpstk::AntexData and gpstk::AntexStream.
       */
   class AntexHeader : public AntexBase
   {
   public:

         /// Default and only constructor.
      AntexHeader() : valid(false), version(1.3) {}
         /// Clear (empty out) header
      inline void clear()
      {
         version = 1.3;
         valid = false;
         commentList.clear();
      }

         /// @name AntexHeaderFormatStrings
         /// ANTEX Header Formatting Strings
         //@{
      static const std::string versionString;         ///< "ANTEX VERSION / SYST"
      static const std::string pcvTypeString;         ///< "PCV TYPE / REFANT"
      static const std::string headerCommentString;   ///< "COMMENT"
      static const std::string endOfHeaderString;     ///< "END OF HEADER"
         //@}

         /// Validity bits for the ANTEX Header
         /// NB. if version is updated, add allValid<ver> and update isValid()
      enum validBits
      {
         versionValid = 0x01,           ///< "ANTEX VERSION / SYST"
         systemValid = 0x02,            ///< "ANTEX VERSION / SYST"
         pcvTypeValid = 0x04,           ///< "PCV TYPE / REFANT"
         commentValid = 0x08,           ///< "COMMENT"
         endValid   = 0x080000000,      ///< "END OF HEADER"
         allValid13 = 0x080000007       ///< mask for all required valid fields
      };

         /// @name AntexHeaderValues
         //@{
      unsigned long valid;
      double version;                        ///< ANTEX VERSION & TYPE
      char system;                           ///< The ANTEX satellite system
      char pcvType;                          ///< The PCV type (A:absolute R:relative)
      std::string refAntType;                ///< Reference antenna type
      std::string refAntSerNum;              ///< Reference antenna serial number
      std::vector<std::string> commentList;  ///< Comments in header (optional)
         //@}
     
         /// Destructor
      virtual ~AntexHeader() {}

         // our common interface
         /// AntexHeader is a "header" so this function always returns true.
      virtual bool isHeader() const {return true;}
     
         /// This is a simple output function.
         /// It writes the version, name and antenna number of this ANTEX header.
      virtual void dump(std::ostream& s) const;

         /// Parse a single header record, and modify valid accordingly.
         /// Used by reallyGetRecord for AntexHeader
         /// @throw FFStreamError
      void ParseHeaderRecord(std::string& line);

         /// Write all valid header records to the given stream.
         /// Used by reallyPutRecord for AntexHeader
         /// @throw FFSTreamError
         /// @throw StringUtils::StringException
      void WriteHeaderRecords(FFStream& s) const;

         /// Return boolean : is this a valid Rinex header?
      bool isValid() const { return ((valid & allValid13) == allValid13); }
         
   protected:
         /** outputs this record to the stream correctly formatted.
          * @throw std::exception
          * @throw FFStreamError
          * @throw StringUtils::StringException
          */
      virtual void reallyPutRecord(FFStream& s) const;

         /// This function retrieves the ANTEX Header from the given FFStream.
         /// If an stream error is encountered, the stream is reset to its
         ///  original position and its fail-bit is set.
         /// @throw std::exception
         /// @throw StringException when a StringUtils function fails
         /// @throw FFStreamError when exceptions(failbit) is set and
         ///  a read or formatting error occurs.  This also resets the
         ///  stream to its pre-read position.
      virtual void reallyGetRecord(FFStream& s) ;

   }; // end class AntexHeader

   //@}

} // namespace

#endif // GPSTK_ANTEX_HEADER_HPP
