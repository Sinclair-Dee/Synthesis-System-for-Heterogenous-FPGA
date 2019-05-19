/*
* Copyright (c) 2017 Stephen Williams (iecas)
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
#ifndef  __flag_H
#define __flag_H

#ifdef __cplusplus
extern "C"{
#endif

typedef struct 
	{
		unsigned flag_mult_signed_datab0 ;
		unsigned flag_mult_signed_datab1;
		unsigned flag_mult_signed_datab2;
		unsigned flag_mult_signed_datab3;

		unsigned flag_mult_signed_dataa0;
		unsigned flag_mult_signed_dataa1;
		unsigned flag_mult_signed_dataa2;
		unsigned flag_mult_signed_dataa3;

		unsigned flag_mult_signed_result;
	}flat_mult;

extern flat_mult *Flag_mult;
extern flat_mult *  initflag(flat_mult *Flag_mult);

#ifdef __cplusplus
};
#endif


//class FLAG{
//
//public:
//	void setflag_mult_signed_dataa0(unsigned t);
//	void setflag_mult_signed_dataa1(unsigned t);
//	void setflag_mult_signed_dataa2(unsigned t);
//	void setflag_mult_signed_dataa3(unsigned t);
//
//	void setflag_mult_signed_datab0(unsigned t);
//	void setflag_mult_signed_datab1(unsigned t);
//	void setflag_mult_signed_datab2(unsigned t);
//	void setflag_mult_signed_datab3(unsigned t);
//
//	void setflag_mult_signed_result(unsigned t);
//
//	unsigned getflag_mult_signed_dataa0( );
//	unsigned getflag_mult_signed_dataa1();
//	unsigned getflag_mult_signed_dataa2();
//	unsigned getflag_mult_signed_dataa3();
//
//	unsigned getflag_mult_signed_datab0();
//	unsigned getflag_mult_signed_datab1();
//	unsigned getflag_mult_signed_datab2();
//	unsigned getflag_mult_signed_datab3();
//
//	bool getflag_mult_signed_result();
//
//private:
//	unsigned flag_mult_signed_datab0;
//	unsigned flag_mult_signed_datab1;
//	unsigned flag_mult_signed_datab2;
//	unsigned flag_mult_signed_datab3;
//
//	unsigned flag_mult_signed_dataa0;
//	unsigned flag_mult_signed_dataa1;
//	unsigned flag_mult_signed_dataa2;
//	unsigned flag_mult_signed_dataa3;
//
//	unsigned flag_mult_signed_result;
//
//};
////extern FLAG  Flag;
#endif

