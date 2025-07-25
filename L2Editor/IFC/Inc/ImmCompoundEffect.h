/**********************************************************************
  IFC 2.3 Header

  Copyright (c) 1997 - 2002 Immersion Corporation

	Permission to use, copy, modify, distribute, and sell this
	software and its documentation may be granted without fee;
	interested parties are encouraged to request permission from
		Immersion Corporation
		801 Fox Lane
		San Jose, CA 95131
		408-467-1900

	IMMERSION DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
	INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
	IN NO EVENT SHALL IMMERSION BE LIABLE FOR ANY SPECIAL, INDIRECT OR
	CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
	LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
	NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
	CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

  FILE:		ImmCompoundEffect.h

  PURPOSE:	Manages Compound Effects for Force Foundation Classes

  STARTED:	2/24/99 by Jeff Mallett

  NOTES/REVISIONS:
     3/2/99 jrm (Jeff Mallett): Force-->Feel renaming
	 3/15/99 jrm: __declspec(dllimport/dllexport) the whole class

**********************************************************************/


#include "ImmBaseTypes.h"
#include "ImmEffect.h"

#include "ImmIFR.h"

#if !defined(__FEELCOMPOUNDEFFECT_H)
#define __FEELCOMPOUNDEFFECT_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef _IFCDLL_
#define DLLIFC __declspec(dllimport)
#else
#define DLLIFC __declspec(dllexport)
#endif

/*
**  IMM_FFE_FILEEFFECT - struct used by DX7 to read and write to FFE
**  files.  This struct is different from DIFILEEFFECT due to the use
**  of the non const LPDIEFFECT.  An LPDIEFFECT is needed to be able to
**  collect information from IFC class objects.  This should be defined
**  elsewhere, but no more appropriate header currently exists.
*/
typedef struct IMM_FFE_FILEEFFECT{
    DWORD       dwSize;
    GUID        GuidEffect;
    LPVOID      lpDiEffect;
    CHAR        szFriendlyName[MAX_PATH];                    
}IMM_FFE_FILEEFFECT, *LPIMM_FFE_FILEEFFECT;

//================================================================
// CImmCompoundEffect
//================================================================
// Represents a compound effect, such as might be created in
// Immersion Studio.  Contains an array of effect objects.
// Methods iterate over component effects, passing the message
// to each one.
// Also, has stuff for being used by CImmProject:
//   * next pointer so can be put on a linked list
//   * force name

//
// ------ PUBLIC INTERFACE ------ 
//

class DLLIFC CImmCompoundEffect
{
    //
    // CONSTRUCTOR/DESTRUCTOR
    //

public://###	protected:
	// Constructs a CImmCompoundEffect
	// Don't try to construct a CImmCompoundEffect yourself.
    // Instead let CImmProject construct it for you.
	CImmCompoundEffect(LPVOID pInternalData);

	public:

	~CImmCompoundEffect();


    //
    // ATTRIBUTES
    //

	public:

	const long
	GetNumberOfContainedEffects();
   
	const long
	GetNumberOfContainedSounds();

	const char *
	GetName(); 

	GENERIC_EFFECT_PTR
	GetContainedEffect(
		long index
		);

	GENERIC_EFFECT_PTR
	GetContainedEffect(
		LPCSTR lpszEffectName
		);

	LPCSTR
	GetContainedSound(
		long index
		);

	DWORD
	GetEffectType();

	LPCSTR
	GetTypeName();

	//
    // OPERATIONS
    //

	public:

	// Start all the contained effects
	BOOL Start(
		DWORD dwIterations = IMM_EFFECT_DONT_CHANGE,
		DWORD dwFlags = 0
		);

	// Stop all the contained effects
	BOOL Stop();


//
// ------ PRIVATE INTERFACE ------ 
//

    //
    // HELPERS
    //

    protected:

	BOOL
	get_ffe_object(LPIMM_FFE_FILEEFFECT pffeObject);


    //
    // FRIENDS
    //

	public:

	friend class CImmProject;


    //
    // INTERNAL DATA
    //

	protected:
	LPVOID m_pIFRObject;

	CImmCompoundEffect *m_pNext; // Next compound effect in the project
};

#endif
