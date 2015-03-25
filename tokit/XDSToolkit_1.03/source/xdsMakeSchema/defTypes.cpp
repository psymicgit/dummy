// ----------------------------------------------------------------------------
// 
// This file is part of the XDS Toolkit, Copyright (C) 2003-2004 Mark T. Price
// For conditions of distribution and use, see XdsLicense.h
//
// ----------------------------------------------------------------------------

#include "XDSinternal.h"
#include "xdsMakeSchema.h"
extern "C" {
#include "crossrefP.h"
}

#define ANONYMOUS_TYPE_NAME "_xdsType"


static char      *g_szScope = NULL;

static char *makeTypeName(void)
{
	// create new type name
	static int iTypeNum = 0;
	char name[80];
	sprintf_s(name, sizeof(name), ANONYMOUS_TYPE_NAME "%d", ++iTypeNum);

	return xdsStringDup(name);
}

// build a type structure corresponding to a built-in type
/* struct xdsType_t *xdsBaseType(unsigned short iType)
{
	static xdsType_t type;
	memset(&type, 0, sizeof(struct xdsType_t));

	type.iClass = 0;
	type.iNodeSizeLength = 0;
	type.iNodeSizeType = XDS_NODESIZE_FIXED;
	type.iTypeId = iType;
	type.szName  = NULL;

	switch(iType)
	{
	case XDS_TYPE_CHAR:
		type.iType   = XDS_META_TYPE_INTEGER;
		type.iNodeSize = sizeof(u8);
		type.var.int_t.bSigned = true;
		type.var.int_t.iValueSize = sizeof(u8);
		break;
	case XDS_TYPE_BYTE:
		type.iType   = XDS_META_TYPE_INTEGER;
		type.iNodeSize = sizeof(u8);
		type.var.int_t.bSigned = false;
		type.var.int_t.iValueSize = sizeof(u8);
		break;
	case XDS_TYPE_SHORT:
		type.iType   = XDS_META_TYPE_INTEGER;
		type.iNodeSize = sizeof(u16);
		type.var.int_t.bSigned = true;
		type.var.int_t.iValueSize = sizeof(u16);
		break;
	case XDS_TYPE_WORD:
		type.iType   = XDS_META_TYPE_INTEGER;
		type.iNodeSize = sizeof(u16);
		type.var.int_t.bSigned = false;
		type.var.int_t.iValueSize = sizeof(u16);
		break;
	case XDS_TYPE_LONG:
		type.iType   = XDS_META_TYPE_INTEGER;
		type.iNodeSize = sizeof(u32);
		type.var.int_t.bSigned = true;
		type.var.int_t.iValueSize = sizeof(u32);
		break;
	case XDS_TYPE_DWORD:
		type.iType   = XDS_META_TYPE_INTEGER;
		type.iNodeSize = sizeof(u32);
		type.var.int_t.bSigned = false;
		type.var.int_t.iValueSize = sizeof(u32);
		break;
	case XDS_TYPE_BCSTRING:
		type.iType   = XDS_META_TYPE_STRING;
		type.iNodeSize = sizeof(u8);
		type.iNodeSizeLength = sizeof(u8);
		type.iNodeSizeType = XDS_NODESIZE_INDIRECT;
		type.var.string_t.iEncoding = XDS_ENCODING_UTF8;
		type.var.string_t.iLength   = 0;
		type.var.string_t.iLengthSize = sizeof(u8);
		break;
	case XDS_TYPE_WCSTRING:
		type.iType   = XDS_META_TYPE_STRING;
		type.iNodeSize = sizeof(u8);
		type.iNodeSizeLength = sizeof(u16);
		type.iNodeSizeType = XDS_NODESIZE_INDIRECT;
		type.var.string_t.iEncoding = XDS_ENCODING_UTF8;
		type.var.string_t.iLength   = 0;
		type.var.string_t.iLengthSize = sizeof(u16);
		break;
	case XDS_TYPE_FLOAT:
		type.iType   = XDS_META_TYPE_FLOATINGPOINT;
		type.iNodeSize = sizeof(float);
		type.var.float_t.iCoeffLen = 24;
		type.var.float_t.iExpLen   = 7;
		type.var.float_t.iPadLen   = 0;
		type.var.float_t.iTypeLen  = sizeof(float);
		break;
	case XDS_TYPE_DOUBLE:
		type.iType   = XDS_META_TYPE_FLOATINGPOINT;
		type.iNodeSize = sizeof(double);
		type.var.float_t.iCoeffLen = 50;
		type.var.float_t.iExpLen   = 11;
		type.var.float_t.iPadLen   = 2;
		type.var.float_t.iTypeLen  = sizeof(double);
		break;
	default:
		return NULL;
	}

	return &type;
}
*/

// create an alias for an existing user-defined or built-in type
static unsigned short aliasType(unsigned short iType, char *szName)
{
	struct xdsType_t *pType;

	if(iType < XDS_FIRST_DEFID)
	{
//		pType = xdsBaseType(iType);
		static xdsType_t type;
		memset(&type, 0, sizeof(struct xdsType_t));
		pType = &type;
	}
	else
	{
		pType = xdsFindType(g_hXds, iType);
		if(!pType)
			return 0;
	}
	struct xdsType_t *pNewType = (struct xdsType_t *)XDS_ALLOC(NULL, sizeof(struct xdsType_t), XDS_MEMTYPE_SCRATCH);
	if(!pNewType)
		return 0;
	memcpy(pNewType, pType, sizeof(struct xdsType_t));  // copy existing type entry
	pNewType->szName = xdsStringDup(szName);            // put in new name
	pNewType->iClass = 0;                               // hack: use '0' as marker for type alias class
	pNewType->iTypeId = iType;
	g_hXds->pTypeDbByName->Add((void*)pNewType);        // only add to name database...

	return pType->iTypeId;
}


static unsigned short findPointerType(unsigned short iBaseType)
{
#if 0
	CTrieIterator it(*g_hXds->pTypeDb);

	for(; !it.AtEnd(); ++it)
	{
		struct xdsType_t *pType = (struct xdsType_t *)it.GetData();

		if(pType->iClass != XDS_RECORD_TYPEDEF)
			continue;
		if(pType->iType != XDS_META_TYPE_POINTER)
			continue;

		if(pType->var.array_t.iBaseType == iBaseType)
			return pType->iTypeId;
	}
	return = xdsDefPointerType(g_hXds, makeTypeName(), iBaseType, (unsigned char)g_iPointerLengthSize);
#else
	// since the XDS lite library doesn't yet support any special processing
	// on different pointer types, we'll just return a pointer to byte for
	// all pointer types
	static unsigned short iPointerType = 0;
	if(!iPointerType)
		iPointerType = xdsDefPointerType(g_hXds, makeTypeName(), XDS_TYPE_BYTE, (unsigned char)g_iPointerLengthSize);
	return iPointerType;
#endif
}

static unsigned short findArrayType(unsigned short iBaseType, unsigned long iCount)
{
	CTrieIterator it(*g_hXds->pTypeDb);

	for(; !it.AtEnd(); ++it)
	{
		struct xdsType_t *pType = (struct xdsType_t *)it.GetData();

		if(pType->iClass != XDS_RECORD_TYPEDEF)
			continue;
		if(pType->iType != XDS_META_TYPE_ARRAY)
			continue;
		if(pType->var.array_t.iBaseType == iBaseType && pType->var.array_t.iDefaultSize == iCount)
			return pType->iTypeId;
	}
	return xdsDefArrayType(g_hXds, makeTypeName(), iBaseType, iCount, (unsigned char)g_iArrayLengthSize);
}

void renameType(unsigned short iType, char *szName)
{
	// rename the existing type
	struct xdsType_t *pType = xdsFindType(g_hXds, iType);
	g_hXds->pTypeDbByName->Del((unsigned char*)pType->szName, strlen(pType->szName));
	XDS_FREE((void *)pType->szName);
	pType->szName = xdsStringDup(szName);
	g_hXds->pTypeDbByName->Add(pType);
}


static int decodeDeclaration1(Declaration_t decl, bool bValidate, unsigned short iBaseType, unsigned short &iDeclType, int &iCount, char **szName, int iDeclaratorIndex);


static unsigned short decodeBaseType(Declaration_t decl, bool bValidate)
{
	// ignore templates and other miscellaneous items
	if(decl->iType != DECLARATION_OBJECT && decl->iType != DECLARATION_MEMBER)
		return 0;

	// ignore const and void data objects
	if(decl->s_const || decl->s_void)
		return 0;

	// ignore unions
	if(decl->Class && decl->Class->iType == SN_UNION)
		return 0;

	// handle built-in types
	if(decl->s_bool)
		return XDS_TYPE_DWORD;
	else if(decl->s_char)
	{
		if(decl->s_unsigned)
			return XDS_TYPE_BYTE;
		else
			return XDS_TYPE_CHAR;
	}
	else if(decl->s_short)
	{
		if(decl->s_unsigned)
			return XDS_TYPE_WORD;
		else
			return XDS_TYPE_SHORT;
	}
	else if(decl->s_int || decl->s_long)
	{
		if(decl->s_unsigned)
			return XDS_TYPE_DWORD;
		else
			return XDS_TYPE_LONG;
	}
	else if(decl->s_float)
		return XDS_TYPE_FLOAT;
	else if(decl->s_double)
		return XDS_TYPE_DOUBLE;

	// handle enums
	if(decl->Enum)
	{
		Enum_t et = decl->Enum;
		if(!et->ListEnumerator)
			return 0;

		unsigned short iEnumType;

		if(et->Name == NULL)
			iEnumType = xdsDefEnumeratedType(g_hXds, makeTypeName(), 'u', (unsigned char)g_iEnumLengthSize);	// anonymous enum
		else
			iEnumType = xdsDefEnumeratedType(g_hXds, et->Name->pcName, 'u', (unsigned char)g_iEnumLengthSize);

		if(et->ListEnumerator)
		{
			int iValue = 0;
			Enumerator_t en = (Enumerator_t)et->ListEnumerator->ElemFirst;
			while(en)
			{
				if(en->Expr && en->Expr->Type->s_int)
					iValue = en->Expr->intValue;

				xdsDefEnumerand(g_hXds, iEnumType, en->Name->pcName, iValue);

				++iValue;
				en = en->EnumeratorNext;
			}
		}

		return iEnumType;
	}

	// handle classes & structures
	if(decl->Class)
	{
		Class_t cl = decl->Class;

		if(!cl->ListMember)
		{
			if(cl->Name)
			{
				xdsType_t *pType = (xdsType_t *)g_hXds->pTypeDbByName->Find((unsigned char *)cl->Name->pcName, strlen(cl->Name->pcName));
				if(pType)
					return pType->iTypeId;
			}
			return 0;
		}

		// don't support classes with virtual methods...
		for(Member_t m = (Member_t)cl->ListMember->ElemFirst; m; m = m->MemberNext)
		{
			if(m->Declaration && m->Declaration->fct_specifier == SN_VIRTUAL)
				return 0;
		}

		// don't support derived classes derived from unknown (or unsupported) classes...
		if(cl->ListBase)
		{
			Base_t b = (Base_t)cl->ListBase->ElemFirst; 
			
			// don't support classes with multiple inheritance
			if(b->BaseNext)
				return 0;

			if(b->bVirtual || !b->Name)
				return 0;
			struct xdsType_t *pType = (struct xdsType_t *)g_hXds->pTypeDbByName->Find((unsigned char *)b->Name->pcName, strlen(b->Name->pcName));
			if(!pType || pType->iType != XDS_META_TYPE_STRUCTURE)
				return 0;
		}


		char *szStructName = (cl->Name) ? cl->Name->pcName : makeTypeName();

		// ensure all types used in structure are already defined before defining the structure itself
		if(cl->ListMember)
		{
			char *szSaveScope = g_szScope;
			g_szScope = szStructName;

			for(Member_t m = (Member_t)cl->ListMember->ElemFirst; m; m = m->MemberNext)
			{
				if(m->Declaration)
				{
					unsigned short iBaseType = 0;
					unsigned short iFieldType = 0;
					int iCount = 0;
					char *szFieldName;
					int iDeclStatus;

					// register base type
					iBaseType = decodeBaseType(m->Declaration, bValidate);
					if(!iBaseType)
						break;

					for(int iDeclaratorIndex = 0; iDeclStatus = decodeDeclaration1(m->Declaration, bValidate, iBaseType, iFieldType, iCount, &szFieldName, iDeclaratorIndex); ++iDeclaratorIndex)
					{
						if(iDeclStatus < 0)
						{
							if(g_bVerbose)
								fprintf(stderr, "%s: invalid type for '%s.%s'\n", szProgName, szStructName, szFieldName);
							return 0;
						}
						else if(iDeclStatus == FUNCTION)
							continue;	// skip class methods

						iCount = 0;
					}
				}
			}

			g_szScope = szSaveScope;
		}


		unsigned short iStructType = xdsDefStructType(g_hXds, szStructName);

		// add all fields from base types
		if(cl->ListBase)
		{
			Base_t b = (Base_t)cl->ListBase->ElemFirst;

			struct xdsType_t *pType = (struct xdsType_t *)g_hXds->pTypeDbByName->Find((unsigned char *)b->Name->pcName, strlen(b->Name->pcName));
			struct xdsField_t *pField = pType->var.struct_t.pFields;
			for(int ii = 0; ii < pType->var.struct_t.iFieldCount; ++ii, ++pField)
			{
				xdsDefStructField(g_hXds, iStructType, (char *)pField->szName, pField->iType, pField->iArraySize);
			}
		}

		if(cl->ListMember)
		{
			char *szSaveScope = g_szScope;
			g_szScope = szStructName;

			for(Member_t m = (Member_t)cl->ListMember->ElemFirst; m; m = m->MemberNext)
			{
				if(m->Declaration)
				{
					unsigned short iBaseType = 0;
					unsigned short iFieldType = 0;
					int iCount = 0;
					char *szFieldName;
					int iDeclStatus;

					// register base type
					iBaseType = decodeBaseType(m->Declaration, bValidate);
					if(!iBaseType)
						break;

					for(int iDeclaratorIndex = 0; iDeclStatus = decodeDeclaration1(m->Declaration, bValidate, iBaseType, iFieldType, iCount, &szFieldName, iDeclaratorIndex); ++iDeclaratorIndex)
					{
						if(iDeclStatus < 0)
						{
							if(g_bVerbose)
								fprintf(stderr, "%s: invalid type for '%s.%s'\n", szProgName, szStructName, szFieldName);
							// remove type from database
							struct xdsType_t *pType = (struct xdsType_t *)g_hXds->pTypeDb->Del((unsigned char *)iStructType, sizeof(iStructType));
							if(pType)
							{
								g_hXds->pTypeDbByName->Del((unsigned char *)pType->szName, strlen(pType->szName));
							}
							return 0;
						}
						else if(iDeclStatus == FUNCTION)
							continue;	// skip class methods

						xdsDefStructField(g_hXds, iStructType, szFieldName, iFieldType, iCount);
						iCount = 0;
					}
				}
			}
			xdsDefStructDone(g_hXds, iStructType);

			g_szScope = szSaveScope;
		}

		return iStructType;
	}

	// handle simple instances of typedefs
	if(decl->Name)
	{
		struct xdsType_t *pType = (struct xdsType_t *)g_hXds->pTypeDbByName->Find((unsigned char *)decl->Name->pcName, strlen(decl->Name->pcName));
		if(pType)
		{
			return pType->iTypeId;
		}
	}

	// handle annoying instances of typedefs
	if(decl->ListDeclarator)
	{
		Declarator_t d = (Declarator_t)decl->ListDeclarator->ElemFirst;
		if(d && d->ListOper)
		{
			Oper_t o = (Oper_t)d->ListOper->ElemFirst;
			if(o && o->Name)
			{
				struct xdsType_t *pType = (struct xdsType_t *)g_hXds->pTypeDbByName->Find((unsigned char *)o->Name->pcName, strlen(o->Name->pcName));
				if(pType)
				{
					return pType->iTypeId;
				}
			}
		}
	}	

	// unknown type...
	return 0;
}


#define DECL_STATUS_UNKNOWN_SIZE_ARRAY   -1
#define DECL_STATUS_UNNAMED              -2
#define DECL_STATUS_UNKNOWN_TYPE_ARRAY   -3
#define DECL_STATUS_TYPE_REFERENCE       -4
#define DECL_STATUS_UNKNOWN              -5


Oper_t prevOper(Declarator_t d, Oper_t o)
{
	Oper_t po;

	for(po = (Oper_t)d->ListOper->ElemFirst; po; po = po->OperNext)
	{
		if(po->OperNext == o)
			return po;
	}
	return NULL;
}

// TODO: add support for pointer-to-pointer type declarations (e.g. char **psz)
//
static int decodeDeclaration1(Declaration_t decl, bool bValidate, unsigned short iBaseType, unsigned short &iDeclType, int &iCount, char **szName, int iDeclaratorIndex)
{
	iDeclType = iBaseType;
	bool bIsTypedef = (decl->storage_class == SN_TYPEDEF);

	if(decl->ListDeclarator)
	{
		// search forward to appropriate declarator index
		Declarator_t d = (Declarator_t)decl->ListDeclarator->ElemFirst;
		for(; iDeclaratorIndex && d; --iDeclaratorIndex, d = d->DeclaratorNext)
			;
		if(!d)
			return 0;

		if(d->Name)
			*szName = d->Name->pcName;
		else if(g_szScope)
			*szName = 0;
		else
			return DECL_STATUS_UNNAMED;

		if(d->ListOper)
		{
			int iStatus = 0;
//			for(Oper_t o = (Oper_t)d->ListOper->ElemFirst; o; o = o->OperNext)
			for(Oper_t o = (Oper_t)d->ListOper->ElemLast; o; o = prevOper(d, o))
			{
				switch(o->type)
				{
				case FUNCTION: 
					return FUNCTION;

				case ARRAY:
					if(!o->Expr || (!o->Expr->Type || !o->Expr->Type->s_int))
					{
						// unspecified size array
						if(g_szScope)
							return DECL_STATUS_UNKNOWN_SIZE_ARRAY;	// may be due to unexpanded macro...
						else
						{
//							if(o->OperNext)
							if(prevOper(d, o))
								iDeclType = findArrayType(iDeclType, 0);
							else
								iDeclType = xdsDefArrayType(g_hXds, *szName, iDeclType, 0, (unsigned char)g_iArrayLengthSize);
						}
					}
					else
					{
						if(g_szScope)
						{
//							if(o->OperNext)
							if(prevOper(d, o))
							{
								iDeclType = findArrayType(iDeclType, o->Expr->intValue);
							}
							else
								iCount = o->Expr->intValue;
						}
						else
						{
							// if it's an array of char *'s, change base type back
							if(iDeclType == XDS_TYPE_BCSTRING || iDeclType == XDS_TYPE_WCSTRING)
							{
								iDeclType = findPointerType(XDS_TYPE_CHAR);
							}
							iDeclType = findArrayType(iDeclType, o->Expr->intValue);
//							if(!o->OperNext)
							if(!prevOper(d, o))
							{
								if(bIsTypedef)
									renameType(iDeclType, *szName);
								else
									iDeclType = xdsDefElement(g_hXds, *szName, iDeclType, 0);
							}
						}
					}
					iStatus = ARRAY;
					break;

				case POINTER_STAR:
					if(g_szScope)
					{
						// inside of class/struct, treat as pointer to type
						iDeclType = findPointerType(iDeclType);
					}
					else
					{
						// outside of class/struct, treat as unspecified size array
						if(iDeclType == XDS_TYPE_CHAR)
						{
							if(g_iStringLengthSize == 1)
								iDeclType = XDS_TYPE_BCSTRING;
							else
								iDeclType = XDS_TYPE_WCSTRING;
						}
						else
						{
							iDeclType = findArrayType(iDeclType, 0);
						}
					}


					if(bIsTypedef)
					{
						// see if array type already exists
						if(iDeclType >= XDS_FIRST_DEFID)
						{
							renameType(iDeclType, *szName);
						}
						else
						{
//							fprintf(stderr, "%s: Fatal error - array of non-defined type\n", szProgName);
//							exit(1);
							return DECL_STATUS_UNKNOWN_TYPE_ARRAY;
						}
						iStatus = ARRAY;
					}
					else
					{
//						if(!g_szScope && !o->OperNext)
						if(!g_szScope && !prevOper(d, o))
						{
							iDeclType = xdsDefElement(g_hXds, *szName, iDeclType, 0);
						}
						else
						{
							iStatus = POINTER_STAR;
						}
					}
					if(!prevOper(d, o))
						return ARRAY;
					break;
				case POINTER_AMPERSAND: // (e.g. int &value; )
					return DECL_STATUS_TYPE_REFERENCE;
					break;
				}
			}
			return iStatus;
		}
		else
		{
			iDeclType = iBaseType;
			if(!g_szScope)
			{
				if(bIsTypedef)
				{
					if(iBaseType >= XDS_FIRST_DEFID)
					{
						struct xdsType_t *pType = xdsFindType(g_hXds, iBaseType);
						if(!memcmp(pType->szName, *szName, strlen(ANONYMOUS_TYPE_NAME)))
						{
							// (re)name existing anonymous types
							g_hXds->pTypeDbByName->Del((unsigned char*)pType->szName, strlen(pType->szName));
							XDS_FREE((void*)pType->szName);
							pType->szName = xdsStringDup(*szName);
							g_hXds->pTypeDbByName->Add(pType);
						}
						else
						{
							// create alias for existing user-named types
							iDeclType = aliasType(iBaseType, *szName);
						}
					}
					else
					{
						// create alias for built-in types
						iDeclType = aliasType(iBaseType, *szName);
					}
				}
				else
				{
					iDeclType = xdsDefElement(g_hXds, *szName, iBaseType, 0);
				}
			}
			return 100;
		}
	}

	return 0;
}


// build XDS type info for declaration
void decodeDeclaration(Declaration_t decl)
{
	unsigned short iBaseType;
	unsigned short iDeclType;
	int   iCount;
	char *szName;
	
	// register base type
	iBaseType = decodeBaseType(decl, true);
	if(!iBaseType)
		return;

	int iDeclaratorIndex = 0;
	do
	{
		iDeclType = iCount = 0;
		szName = NULL;
	}
	while(decodeDeclaration1(decl, true, iBaseType, iDeclType, iCount, &szName, iDeclaratorIndex++));
}
