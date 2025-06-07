/*=============================================================================
	Editor.cpp: Unreal editor package.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.
=============================================================================*/

#include "EditorPrivate.h"

/*-----------------------------------------------------------------------------
	Globals.
-----------------------------------------------------------------------------*/

// Global variables.
EDITOR_API FGlobalTopicTable GTopics;

// Register things.
#define NAMES_ONLY
#define AUTOGENERATE_NAME(name) EDITOR_API FName EDITOR_##name;
#define AUTOGENERATE_FUNCTION(cls,idx,name) IMPLEMENT_FUNCTION(cls,idx,name)
#include "EditorClasses.h"
#undef AUTOGENERATE_FUNCTION
#undef AUTOGENERATE_NAME
#undef NAMES_ONLY

// sjs --- import natives
#define NATIVES_ONLY
#define NAMES_ONLY
#define AUTOGENERATE_NAME(name)
#define AUTOGENERATE_FUNCTION(cls,idx,name)
#include "EditorClasses.h"
#undef AUTOGENERATE_FUNCTION
#undef AUTOGENERATE_NAME
#undef NATIVES_ONLY
#undef NAMES_ONLY
// --- sjs

// Package implementation.
IMPLEMENT_PACKAGE(Editor);

/*-----------------------------------------------------------------------------
	Misc
-----------------------------------------------------------------------------*/

INT appStrPrefix(const TCHAR* Str, const TCHAR* Prefix)
{
	for (;;)
	{
		if (!*Prefix)
			return 0;

		TCHAR A = appToUpper(*Str);
		TCHAR B = appToUpper(*Prefix);

		if (A != B)
			return(B - A);

		Str++;
		Prefix++;
	}
}

/*-----------------------------------------------------------------------------
	The end.
-----------------------------------------------------------------------------*/
