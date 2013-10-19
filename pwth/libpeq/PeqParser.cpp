/* License & includes {{{1 */
/* 
   ESESC: Enhanced Super ESCalar simulator
   Copyright (C) 2009 University of California, Santa Cruz.

   Contributed by Meeta Sinha-Verma

This file is part of ESESC.

ESESC is free software; you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation;
either version 2, or (at your option) any later version.

ESESC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
ESESC; see the file COPYING. If not, write to the Free Software Foundation, 59
Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

/* License & includes {{{1 */
/* 
   ESESC: Enhanced Super ESCalar simulator
   Copyright (C) 2009 University of California, Santa Cruz.

   Contributed by Meeta Sinha-Verma

This file is part of ESESC.

ESESC is free software; you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation;
either version 2, or (at your option) any later version.

ESESC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
ESESC; see the file COPYING. If not, write to the Free Software Foundation, 59
Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include "muParserTest.h"

#define _USE_MATH_DEFINES		

#include <cstdlib>
#include <cstring>
#include <cmath>
#include <string>
#include <iostream>
#include <locale>
#include <limits>
#include <ios> 
#include <iomanip>
#include <numeric>

#include "muParser.h"
#include "PeqParser.h"
#include "SescConf.h"

using namespace std;
using namespace mu;


// Forward declarations

// Operator callback functions
value_type Mega(value_type a_fVal) { return a_fVal * 1e6; }
value_type Milli(value_type a_fVal) { return a_fVal / (value_type)1e3; }
value_type Not(value_type v) { return v==0; }
value_type Add(value_type v1, value_type v2) { return v1+v2; }
value_type Mul(value_type v1, value_type v2) { return v1*v2; }

//---------------------------------------------------------------------------
// Factory function for creating new parser variables
// This could as well be a function performing database queries.
value_type* AddVariable(const char_type *a_szName, void *a_pUserData)
{
  // I don't want dynamic allocation here, so i used this static buffer
  // If you want dynamic allocation you must allocate all variables dynamically
  // in order to delete them later on. Or you find other ways to keep track of 
  // variables that have been created implicitely.
  static value_type afValBuf[100];  
  static int iVal = -1;

  ++iVal;

  mu::console() << _T("Generating new variable \"") 
                << a_szName << std::dec << _T("\" (slots left: ")
                << 99-iVal << _T(")")
                << _T(" User data pointer is:") 
                << std::hex << a_pUserData <<endl;
  afValBuf[iVal] = 0;

  if (iVal>=99)
    throw mu::ParserError( _T("Variable buffer overflow.") );
  else
    return &afValBuf[iVal];
}

int IsHexValue(const char_type *a_szExpr, int *a_iPos, value_type *a_fVal) 
{ 
  if (a_szExpr[1]==0 || (a_szExpr[0]!='0' || a_szExpr[1]!='x') ) 
    return 0;

  unsigned iVal(0);

  // New code based on streams for UNICODE compliance:
  stringstream_type::pos_type nPos(0);
  stringstream_type ss(a_szExpr + 2);
  ss >> std::hex >> iVal;
  nPos = ss.tellg();

  if (nPos==(stringstream_type::pos_type)0)
    return 1;

  *a_iPos += (int)(2 + nPos);
  *a_fVal = (value_type)iVal;

  return 1;
}


//---------------------------------------------------------------------------
void ListVar(const mu::ParserBase &parser)
{
  // Query the used variables (must be done after calc)
  mu::varmap_type variables = parser.GetVar();
  if (!variables.size())
    return;

  cout << "\nParser variables:\n";
  cout <<   "-----------------\n";
  cout << "Number: " << (int)variables.size() << "\n";
  varmap_type::const_iterator item = variables.begin();
  for (; item!=variables.end(); ++item)
    mu::console() << _T("Name: ") << item->first << _T("   Address: [0x") << item->second << _T("]\n");
}

//---------------------------------------------------------------------------
void ListConst(const mu::ParserBase &parser)
{
  mu::console() << _T("\nParser constants:\n");
  mu::console() << _T("-----------------\n");

  mu::valmap_type cmap = parser.GetConst();
  if (!cmap.size())
  {
    mu::console() << _T("Expression does not contain constants\n");
  }
  else
  {
    valmap_type::const_iterator item = cmap.begin();
    for (; item!=cmap.end(); ++item)
      mu::console() << _T("  ") << item->first << _T(" =  ") << item->second << _T("\n");
  }
}

//---------------------------------------------------------------------------
void ListExprVar(const mu::ParserBase &parser)
{
  string_type sExpr = parser.GetExpr();
  if (sExpr.length()==0)
  {
    cout << _T("Expression string is empty\n");
    return;
  }

  // Query the used variables (must be done after calc)
  mu::console() << _T("\nExpression variables:\n");
  mu::console() <<   _T("---------------------\n");
  mu::console() << _T("Expression: ") << parser.GetExpr() << _T("\n");

  varmap_type variables = parser.GetUsedVar();
  if (!variables.size())
  {
    mu::console() << _T("Expression does not contain variables\n");
  }
  else
  {
    mu::console() << _T("Number: ") << (int)variables.size() << _T("\n");
    mu::varmap_type::const_iterator item = variables.begin();
    for (; item!=variables.end(); ++item)
      mu::console() << _T("Name: ") << item->first << _T("   Address: [0x") << item->second << _T("]\n");
  }
}

////////////////////////////////////////////////////////////
PeqParser::PeqParser()
{

  parser.DefinePostfixOprt(_T("M"), Mega);
  parser.DefinePostfixOprt(_T("m"), Milli);
  parser.DefineInfixOprt(_T("!"), Not);
  parser.DefineOprt(_T("add"), Add, 0);
  parser.DefineOprt(_T("mul"), Mul, 1);

  // Define the variable factory
  parser.SetVarFactory(AddVariable, &parser);
}

PeqParser::~PeqParser()
{
}

void PeqParser::SetParserExpression()
{
	const char *EqExpression;
	EqExpression = SescConf->getCharPtr("equation_sample1","leakage");
	parser.SetExpr((string_type)EqExpression);
}
/*value_type PeqParser::ParserEvalEq()
{ 
   gref = GStats::getRef(ROB_AR);
   I(gref);
   uint64_t ROB_counter = gref->getSamples(); 
   printf("ROB_Counter = %d\n", ROB_counter);

} */
void PeqParser::testParser()
{
	// create stat counters for acitivity rate 
	// peqglue.createStatCounters(); 
  printf("FIXME: 'createStatCounters' should not be called here in PeqParser. The testParser Caller should create the counters. Exiting...\n");
  
	this->SetParserExpression();
	string_type s_expr = parser.GetExpr();
//        string_type s_expr1 = parser.GetExpr();
	cout<<"Parser Expression is "<<s_expr<<"\n";
       // cout<<"Activity Rate for ROB read is " <<s_expr1<<"\n";
	parser.Eval();
	ListVar(parser);
}


