#include"stdafx.h"
#include"squirrelex.h"

#include<assert.h>
#include<stdio.h>

#include"import/squirrel/squirrel/sqobject.h"
#include"import/squirrel/squirrel/sqpcheader.h"
//#include"import/squirrel/squirrel/sqvm.h"
//#include"import/squirrel/squirrel/sqstring.h"
#include"import/squirrel/squirrel/sqtable.h"
//#include"import/squirrel/squirrel/sqarray.h"
//#include"import/squirrel/squirrel/sqfuncproto.h"
//#include"import/squirrel/squirrel/sqclosure.h"
//#include"import/squirrel/squirrel/squserdata.h"
//#include"import/squirrel/squirrel/sqcompiler.h"
//#include"import/squirrel/squirrel/sqfuncstate.h"
#include"import/squirrel/squirrel/sqclass.h"

SQUserPointer sq_objtoinstance(HSQOBJECT * po)
{
	if(sq_type(*po) == OT_INSTANCE) {
		return _instance(*po)->_userpointer;
	}
	return NULL;
}


