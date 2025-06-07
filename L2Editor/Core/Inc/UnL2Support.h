#pragma once

class CORE_API L2ThreadInterface {
public:
	virtual void Unk00() = 0;
};

class CORE_API L2ThreadBase {
public:
	L2ThreadBase(TCHAR*);
	L2ThreadBase(L2ThreadBase const &);

	virtual ~L2ThreadBase();
	virtual void Unk00() = 0;
	virtual void Unk01() = 0;
	virtual void Unk02() = 0;
	virtual void Unk03() = 0;
	virtual void Unk04() = 0;
	virtual void Unk05() = 0;
	virtual void Unk06() = 0;
	virtual void* GetHandle();
	virtual void* GetEventHandle();
	virtual unsigned long GetID();
	virtual int IsRunning();
};

class CORE_API L2ThreadUtil {
public:
	L2ThreadUtil();
	L2ThreadUtil(class L2ThreadUtil const &);

	virtual ~L2ThreadUtil();

	void StaticInit();
	
	class L2ThreadStats* GetCurrentThreadStats();
	class L2ThreadStats* GetThreadStats(INT);
	INT GetCurrentThreadIndex();
	INT GetThreadCounter();
	INT GetThreadIndex(unsigned long);
	INT IsCurrentThread(enum L2ThreadFunction);
	INT IsRunning(enum L2ThreadFunction);
	INT KillAllThread();
	unsigned long GetCurrentThreadID();
	unsigned long GetThreadID(enum L2ThreadFunction);
	
	void* GetThreadSafeParam(void *, INT);
	void AddEtcThread();
	void AddMaINThread();
	void AddThread(enum L2ThreadFunction, class L2ThreadBase *);
	void ConditionalThreadAffinityMask(INT);
	void DeleteThreadSafeParam(void *);
	void SetThreadAffinityMask(INT);
};

class CORE_API L2ThreadStats {
public:
	L2ThreadStats();

	void Clear();
	void LockEnd();
	void LockStart();
	void SleepEnd();
	void SleepStart();
};

class CORE_API L2ParamStack {
	L2ParamStack(int);
	L2ParamStack(class L2ParamStack &);
	~L2ParamStack();

	int GetBufferSize();
	int GetTotalBufferSize();
	int PushBack(void*);
	void** GetBuffer();
	void* Top();
	void Clear();
	void Restart();
};

class CORE_API UParamStack : public UObject
{
public:
	INT stack;

public:
	UParamStack(UParamStack const &);

	void SetParamStack(L2ParamStack*);
	
	DECLARE_FUNCTION(execPushString);
	DECLARE_FUNCTION(execPushInt);
	DECLARE_FUNCTION(execGetFloat);
	DECLARE_FUNCTION(execGetInt);
	DECLARE_FUNCTION(execGetString);

	DECLARE_CLASS(UParamStack, UObject, 0, Core)
	NO_DEFAULT_CONSTRUCTOR(UParamStack)
};

class CORE_API FArchiveDummySave : public FArchive {
public:
	FArchiveDummySave();
	FArchiveDummySave(FArchiveDummySave const&);
	virtual ~FArchiveDummySave();
};

class CORE_API FBlowFish {
public:
	FBlowFish();

	short InitializeBlowfish(unsigned char* const, short);
	void ResetMemberVariable();

	void BlowfishDecrypt(unsigned char*, INT);
	void BlowfishEncrypt(unsigned char*, INT);
	void Blowfish_decipher(DWORD*, DWORD*);
	void Blowfish_encipher(DWORD*, DWORD*);
};

class CORE_API FMoverHit {
public:
	FMoverHit();
	FMoverHit(class AMover*, INT);

	FMoverHit& operator=(FMoverHit const&);
	FMoverHit& operator==(FMoverHit const&);
	FMoverHit& operator!=(FMoverHit const&);
};

class CORE_API FNpcPosInfoHit {
public:
	FNpcPosInfoHit();
	FNpcPosInfoHit(class ANpcPosInfo*, INT);

	FNpcPosInfoHit& operator=(FNpcPosInfoHit const&);
	FNpcPosInfoHit& operator==(FNpcPosInfoHit const&);
	FNpcPosInfoHit& operator!=(FNpcPosInfoHit const&);
};

class CORE_API FSuperPointInfoHit {
public:
	FSuperPointInfoHit();
	FSuperPointInfoHit(class ASuperPointInfo*, INT);

	FSuperPointInfoHit& operator=(FSuperPointInfoHit const&);
	FSuperPointInfoHit& operator==(FSuperPointInfoHit const&);
	FSuperPointInfoHit& operator!=(FSuperPointInfoHit const&);
};

class CORE_API FTerritoryInfoHit {
public:
	FTerritoryInfoHit();
	FTerritoryInfoHit(class ATerritoryInfo*, INT);

	FTerritoryInfoHit& operator=(FTerritoryInfoHit const&);
	FTerritoryInfoHit& operator==(FTerritoryInfoHit const&);
	FTerritoryInfoHit& operator!=(FTerritoryInfoHit const&);
};

class CORE_API FVehicleRoutePointHit {
public:
	FVehicleRoutePointHit();
	FVehicleRoutePointHit(class AVehicleRoutePoint*, INT);

	FVehicleRoutePointHit& operator=(FVehicleRoutePointHit const&);
	FVehicleRoutePointHit& operator==(FVehicleRoutePointHit const&);
	FVehicleRoutePointHit& operator!=(FVehicleRoutePointHit const&);
};


class CORE_API FSharedMemory {
public:
	FSharedMemory();
	~FSharedMemory();

	INT Create(TCHAR*, INT);
	void* GetData();
};
		
class CORE_API UTextBufferFactory : public UFactory
{
	DECLARE_CLASS(UTextBufferFactory, UFactory, 0, Core)
	UTextBufferFactory();
	void StaticConstructor();
	UObject* FactoryCreateText(ULevel* InLevel, UClass* Class, UObject* InParent, FName Name, DWORD Flags, UObject* Context, const TCHAR* Type, const TCHAR*& Buffer, const TCHAR* BufferEnd, FFeedbackContext* Warn);
};
