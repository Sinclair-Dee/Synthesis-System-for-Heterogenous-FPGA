#ifndef __StringHeap_H
#define __StringHeap_H
/*
 * Copyright (c) 2002-2004 Stephen Williams (steve@icarus.com)
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */
#ifdef HAVE_CVS_IDENT
#ident "$Id: StringHeap.h,v 1.5.2.2 2005/08/13 00:45:53 steve Exp $"
#endif

# include  "config.h"
# include  <string>

using namespace std;

class perm_string {

    public:
      perm_string() : text_(0) { }
      perm_string(const perm_string&that) : text_(that.text_) { }
	  perm_string(const char*t) : text_(t) { };//ADU:
      ~perm_string() { }

      perm_string& operator = (const perm_string&that)
      { text_ = that.text_; return *this; }

      const char*str() const { return text_; }
      operator const char* () const { return str(); }

	// This is an escape for making perm_string objects out of
	// literals. For example, per_string::literal("Label"); Please
	// do *not* cheat and pass arbitrary const char* items here.
      static perm_string literal(const char*t) { return perm_string(t); }

    private:
      friend class StringHeap;
      friend class StringHeapLex;
      //perm_string(const char*t) : text_(t) { };

    private:
      const char*text_;
};

extern bool operator == (perm_string a, perm_string b);
extern bool operator == (perm_string a, const char* b);
extern bool operator != (perm_string a, perm_string b);
extern bool operator != (perm_string a, const char* b);
extern bool operator >  (perm_string a, perm_string b);
extern bool operator <  (perm_string a, perm_string b);
extern bool operator >= (perm_string a, perm_string b);
extern bool operator <= (perm_string a, perm_string b);

/*
 * The string heap is a way to permanently allocate strings
 * efficiently. They only take up the space of the string characters
 * and the terminating nul, there is no malloc overhead.
 */
class StringHeap {

    public:
      StringHeap();
      ~StringHeap();

      const char*add(const char*);
      perm_string make(const char*);

    private:
      enum { HEAPCELL = 0x10000 };

      char*cell_base_;
      unsigned cell_ptr_;
      unsigned cell_count_;

    private: // not implemented
      StringHeap(const StringHeap&);
      StringHeap& operator= (const StringHeap&);
};

/*
 * A lexical string heap is a string heap that makes an effort to
 * return the same pointer for identical strings. This saves further
 * space by not allocating duplicate strings, so in a system with lots
 * of identifiers, this can theoretically save more space.
 */
class StringHeapLex  : private StringHeap {

    public:
      StringHeapLex();
      ~StringHeapLex();

      const char*add(const char*);
      perm_string make(const char*);
      perm_string make(const std::string&);

      unsigned add_count() const;
      unsigned add_hit_count() const;

    private:
      enum { HASH_SIZE = 4096 };
      const char*hash_table_[HASH_SIZE];

      unsigned add_count_;
      unsigned hit_count_;

    private: // not implemented
      StringHeapLex(const StringHeapLex&);
      StringHeapLex& operator= (const StringHeapLex&);
};

/*
 * $Log: StringHeap.h,v $
 * Revision 1.5.2.2  2005/08/13 00:45:53  steve
 *  Fix compilation warnings/errors with newer compilers.
 *
 * Revision 1.5.2.1  2005/06/14 15:33:54  steve
 *  Fix gcc4 build issues.
 *
 * Revision 1.5  2004/02/18 17:11:54  steve
 *  Use perm_strings for named langiage items.
 *
 * Revision 1.4  2003/03/01 06:25:30  steve
 *  Add the lex_strings string handler, and put
 *  scope names and system task/function names
 *  into this table. Also, permallocate event
 *  names from the beginning.
 *
 * Revision 1.3  2003/01/16 21:44:46  steve
 *  Keep some debugging status.
 *
 * Revision 1.2  2002/08/12 01:34:58  steve
 *  conditional ident string using autoconfig.
 *
 * Revision 1.1  2002/08/04 19:13:16  steve
 *  dll uses StringHeap for named items.
 *
 */
#endif
