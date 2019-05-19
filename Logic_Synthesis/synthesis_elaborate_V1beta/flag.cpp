/*
* Copyright (c) 2017 Stephen Williams (IECAS)
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
#include "flag.h"
//#ifdef HAVE_MALLOC_H
# include  <malloc.h>

extern flat_mult *  initflag(flat_mult *Flag_mult){
	Flag_mult = (flat_mult*)malloc(sizeof(flat_mult));
	Flag_mult->flag_mult_signed_dataa0 = 0;
	Flag_mult->flag_mult_signed_dataa1 = 0;
	Flag_mult->flag_mult_signed_dataa2 = 0;
	Flag_mult->flag_mult_signed_dataa3 = 0;
	Flag_mult->flag_mult_signed_datab0 = 0;
	Flag_mult->flag_mult_signed_datab1 = 0;
	Flag_mult->flag_mult_signed_datab2 = 0;
	Flag_mult->flag_mult_signed_datab3 = 0;
	Flag_mult->flag_mult_signed_result   = 0;
	return Flag_mult;
}

//
//void FLAG::setflag_mult_signed_dataa0(bool t){
//	flag_mult_signed_dataa0 = t;
//}
//void FLAG::setflag_mult_signed_dataa1(bool t){
//	flag_mult_signed_dataa1 = t;
//}
//void FLAG::setflag_mult_signed_dataa2(bool t){
//	flag_mult_signed_dataa2 = t;
//}
//void FLAG::setflag_mult_signed_dataa3(bool t){
//	flag_mult_signed_dataa3 = t;
//}
//
//void FLAG::setflag_mult_signed_datab0(bool t){
//	flag_mult_signed_datab0 = t;
//}
//void FLAG::setflag_mult_signed_datab1(bool t){
//	flag_mult_signed_datab1 = t;
//}
//void FLAG::setflag_mult_signed_datab2(bool t){
//	flag_mult_signed_datab2 = t;
//}
//void FLAG::setflag_mult_signed_datab3(bool t){
//	flag_mult_signed_datab3 = t;
//}
//void FLAG::setflag_mult_signed_result(bool t){
//	flag_mult_signed_result = t;
//}
//
//bool FLAG::getflag_mult_signed_dataa0(){
//	return flag_mult_signed_dataa0;
//}
//bool FLAG::getflag_mult_signed_dataa1(){
//	return flag_mult_signed_dataa1;
//}
//bool FLAG::getflag_mult_signed_dataa2(){
//	return flag_mult_signed_dataa2;
//}
//bool FLAG::getflag_mult_signed_dataa3(){
//	return flag_mult_signed_dataa3;
//}
//
//bool FLAG::getflag_mult_signed_datab0(){
//	return flag_mult_signed_datab0;
//}
//bool FLAG::getflag_mult_signed_datab1(){
//	return flag_mult_signed_datab1;
//}
//bool FLAG::getflag_mult_signed_datab2(){
//	return flag_mult_signed_datab2;
//}
//bool FLAG::getflag_mult_signed_datab3(){
//	return flag_mult_signed_datab3;
//}
//
//bool FLAG::getflag_mult_signed_result(){
//	return flag_mult_signed_result;
//}
//
///*
//* the whole flag
//*/
////extern FLAG Flag;