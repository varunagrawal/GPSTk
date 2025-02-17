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

#ifndef FDSTREAMBUFF_HPP
#define FDSTREAMBUFF_HPP

#include <cstdio>
#include <iostream>

namespace gpstk
{

   // This is just a stream buffer that attaches to a file descriptor.
   // All buffering is done by streambuf. This class only does the filling and
   // flushing.
   class FDStreamBuff : public std::streambuf
   {
   public:

      FDStreamBuff() : handle(-1), bufferSize(4096)
      {}

      FDStreamBuff(int fd) : handle(fd), bufferSize(4096)
      {}

      ~FDStreamBuff()
      {close();}

      // These are the functions that actually read/write from the socket to/from
      // a given buffer. They return the number of bytes transmitted (or
      // EOF on error)
      int write(const char * buffer, const int n);
      int read(char * buffer, const int n);
      
      bool is_open() const { return handle >= 0; }
      void close();
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
      virtual FDStreamBuff* setbuf(char* p, const int len);
  
      // We limit this stream to be sequential
      std::streampos seekoff(std::streamoff, std::ios::seekdir, int)
      { return EOF; }
#pragma clang diagnostic pop
      void dump(std::ostream& out) const;

      struct Buffer
      {
         char * start;
         char * end;
         bool do_dispose;
         void dispose()
         { 
            if (!do_dispose) return;
            free(start); start = end = 0; do_dispose = false;
         }
         Buffer(void) : start(0), end(0), do_dispose(false) {}
         ~Buffer(void) { dispose(); }
      };

      Buffer buffer;
      int handle;
      unsigned bufferSize;
  
   protected:
      // Standard streambuf functions we have to
      // provide an implementation for
      virtual int overflow(int c = EOF);	// Write out a "put area"
      virtual int underflow(void);		// Fill in a "get area"
      virtual int sync(void);		// Commit all uncommitted writes
      virtual int doallocate(void);	// Allocate a new buffer

      // we use the same buffer for get and put areas
      char * base(void) const { return buffer.start; }
      char * ebuf(void) const { return buffer.end; }
      void setb(char * buffer_beg, char * buffer_end, bool do_dispose)
      { buffer.start = buffer_beg; buffer.end = buffer_end;
      buffer.do_dispose = do_dispose;}
   };

} // end of namespace
#endif
