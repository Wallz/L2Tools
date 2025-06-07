/*===============================================================================
	UnCache.h: Unreal fast memory cache support.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Tim Sweeney.
===============================================================================*/

/*-----------------------------------------------------------------------------
	FMemCache.
-----------------------------------------------------------------------------*/

//
// Memory cache.
//
class CORE_API FMemCache
{
public:
	// Information about a cache item (32 bytes).
	enum {COST_INFINITE=0x1000000};
	class CORE_API FCacheItem
	{
	public:
		friend class FMemCache;
		typedef _WORD TCacheTime;

		void Unlock();

		QWORD GetId();
		BYTE* GetData();
		INT GetSize();
		BYTE GetExtra();
		void SetExtra(BYTE B);
		INT GetCost();
		
		TCacheTime GetTime();

		// Implementation.
	private:
		QWORD		Id;				// This item's cache id, 0=unused.
		BYTE*		Data;			// Pointer to the item's data.
		TCacheTime	Time;			// Last Get() time.
		BYTE		Segment;		// Number of the segment this item resides in.
		BYTE		Extra;			// Extra space for use.
		INT			Cost;			// Cost to flush this item.
		FCacheItem*	LinearNext;		// Next cache item in linear list, or NULL if last.
		FCacheItem*	LinearPrev;		// Previous cache item in linear list, or NULL if first.
		FCacheItem*	HashNext;		// Next cache item in hash table, or NULL if last.
	};

	// FMemCache interface.
	FMemCache();

    void Init( INT BytesToAllocate, INT MaxItems, void* Start=NULL, INT SegSize=0 );
	void Exit( INT FreeMemory );
	void Flush( QWORD Id=0, DWORD Mask=~0, UBOOL IgnoreLocked=0 );
	BYTE* Create( QWORD Id, FCacheItem *&Item, INT CreateSize, INT Alignment=DEFAULT_ALIGNMENT, INT SafetyPad=0 );
	void Tick();
	void CheckState();
	UBOOL Exec( const TCHAR* Cmd, FOutputDevice& Ar=*GLog );
	void Status( TCHAR* Msg );
	INT GetTime();

	// Accessors.
	FCacheItem* First();
	FCacheItem* Last();
	FCacheItem* Next(FCacheItem* Item);
	DWORD GHash(DWORD Val);
	BYTE* Get(QWORD Id, FCacheItem*& Item, INT Alignment = DEFAULT_ALIGNMENT);

private:
	// Constants.
	enum {HASH_COUNT=16384};
	enum {IGNORE_SIZE=256};

	// Variables.
	UBOOL		Initialized;
	INT			Time;
	QWORD		MruId;
	FCacheItem* MruItem;
	FCacheItem* ItemMemory;
	FCacheItem* CacheItems;
	FCacheItem* LastItem;
	FCacheItem* UnusedItems;
	FCacheItem* HashItems[HASH_COUNT];
	BYTE*       CacheMemory;

	// Stats.
	INT			NumGets, NumCreates, CreateCycles, GetCycles, TickCycles;
	INT			ItemsFresh, ItemsStale, ItemsTotal, ItemGaps;
	INT			MemFresh, MemStale, MemTotal;

	// Internal functions.
	void CreateNewFreeSpace( BYTE* Start, BYTE* End, FCacheItem* Prev, FCacheItem* Next, INT Segment );
	void Unhash(QWORD Id);
	FCacheItem* MergeWithNext( FCacheItem* First );
	FCacheItem* FlushItem( FCacheItem* Item, UBOOL IgnoreLocked=0 );
	void ConditionalCheckState();

	// Friends.
	friend class FCacheItem;
};

// Global scope.
typedef FMemCache::FCacheItem FCacheItem;

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

