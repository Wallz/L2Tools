/*=============================================================================
	UnMem.h: FMemStack class, ultra-fast temporary memory allocation
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Tim Sweeney
=============================================================================*/

/*-----------------------------------------------------------------------------
	Globals.
-----------------------------------------------------------------------------*/

// Enums for specifying memory allocation type.
enum EMemZeroed {MEM_Zeroed=1};
enum EMemOned   {MEM_Oned  =1};

/*-----------------------------------------------------------------------------
	FMemStack.
-----------------------------------------------------------------------------*/

//
// Simple linear-allocation memory stack.
// Items are allocated via PushBytes() or the specialized operator new()s.
// Items are freed en masse by using FMemMark to Pop() them.
//
class CORE_API FMemStack
{
public:
	// Get bytes.
	BYTE* PushBytes(INT AllocSize, INT Align);

	// Main functions.
	void Init( INT DefaultChunkSize );
	void Exit();
	void Tick();
	INT GetByteCount();

	// Friends.
	friend class FMemMark;
	friend void* operator new( size_t Size, FMemStack& Mem, INT Count=1, INT Align=DEFAULT_ALIGNMENT );
	friend void* operator new( size_t Size, FMemStack& Mem, EMemZeroed Tag, INT Count=1, INT Align=DEFAULT_ALIGNMENT );
	friend void* operator new( size_t Size, FMemStack& Mem, EMemOned Tag, INT Count=1, INT Align=DEFAULT_ALIGNMENT );

	// Types.
	struct FTaggedMemory
	{
		FTaggedMemory* Next;
		INT DataSize;
		BYTE Data[1];
	};

private:
	// Constants.
	enum {MAX_CHUNKS=1024};

	// Variables.
	BYTE*			Top;				// Top of current chunk (Top<=End).
	BYTE*			End;				// End of current chunk.
	INT				DefaultChunkSize;	// Maximum chunk size to allocate.
	FTaggedMemory*	TopChunk;			// Only chunks 0..ActiveChunks-1 are valid.

	// Static.
	static FTaggedMemory* UnusedChunks;

	// Functions.
	class FMemStackFrame& GetThreadSafeMemStack();
};

/*-----------------------------------------------------------------------------
	FMemStack templates.
-----------------------------------------------------------------------------*/

// Operator new for typesafe memory stack allocation.
template <class T> inline T* New( FMemStack& Mem, INT Count=1, INT Align=DEFAULT_ALIGNMENT )
{
	guardSlow(FMemStack::New);
	return (T*)Mem.PushBytes( Count*sizeof(T), Align );
	unguardSlow;
}
template <class T> inline T* NewZeroed( FMemStack& Mem, INT Count=1, INT Align=DEFAULT_ALIGNMENT )
{
	guardSlow(FMemStack::New);
	BYTE* Result = Mem.PushBytes( Count*sizeof(T), Align );
	appMemzero( Result, Count*sizeof(T) );
	return (T*)Result;
	unguardSlow;
}
template <class T> inline T* NewOned( FMemStack& Mem, INT Count=1, INT Align=DEFAULT_ALIGNMENT )
{
	guardSlow(FMemStack::New);
	BYTE* Result = Mem.PushBytes( Count*sizeof(T), Align );
	appMemset( Result, 0xff, Count*sizeof(T) );
	return (T*)Result;
	unguardSlow;
}

/*-----------------------------------------------------------------------------
	FMemStack operator new's.
-----------------------------------------------------------------------------*/

// Operator new for typesafe memory stack allocation.
inline void* operator new( size_t Size, FMemStack& Mem, INT Count, INT Align )
{
	// Get uninitialized memory.
	guardSlow(FMemStack::New1);
	return Mem.PushBytes( Size*Count, Align );
	unguardSlow;
}
inline void* operator new( size_t Size, FMemStack& Mem, EMemZeroed Tag, INT Count, INT Align )
{
	// Get zero-filled memory.
	guardSlow(FMemStack::New2);
	BYTE* Result = Mem.PushBytes( Size*Count, Align );
	appMemzero( Result, Size*Count );
	return Result;
	unguardSlow;
}
inline void* operator new( size_t Size, FMemStack& Mem, EMemOned Tag, INT Count, INT Align )
{
	// Get one-filled memory.
	guardSlow(FMemStack::New3);
	BYTE* Result = Mem.PushBytes( Size*Count, Align );
	appMemset( Result, 0xff, Size*Count );
	return Result;
	unguardSlow;
}

class CORE_API FMemStackFrame : public FMemStack {
	~FMemStackFrame();

	BYTE* AllocateNewChunk(INT);
	BYTE* PushBytes(INT AllocSize, INT Align);
	void FreeChunks(FTaggedMemory*);

	// Main functions.
	void Init(INT DefaultChunkSize);
	void Exit();
	void Tick();
	INT GetByteCount();
};

class CORE_API FMemStackStats{
};

/*-----------------------------------------------------------------------------
	FMemMark.
-----------------------------------------------------------------------------*/

//
// FMemMark marks a top-of-stack position in the memory stack.
// When the marker is constructed or initialized with a particular memory 
// stack, it saves the stack's current position. When marker is popped, it
// pops all items that were added to the stack subsequent to initialization.
//
class CORE_API FMemMark
{
public:
	// Constructors.
	FMemMark();
	FMemMark(FMemStack& InMem);

	// FMemMark interface.
	void Pop();

private:
	// Implementation variables.
	FMemStack* Mem;
	BYTE* Top;
	FMemStack::FTaggedMemory* SavedChunk;
};

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

