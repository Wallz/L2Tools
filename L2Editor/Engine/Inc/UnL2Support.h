
class ENGINE_API UL2NEnvManager : public UObject
{
	DECLARE_CLASS(UL2NEnvManager, UObject, 0, Engine)
};

class ENGINE_API UL2NetHandler : public UObject
{
	DECLARE_CLASS(UL2NetHandler, UObject, 0, Engine)
};

class ENGINE_API UL2ConsoleWnd : public UObject
{
	DECLARE_CLASS(UL2ConsoleWnd, UObject, 0, Engine)
};

struct ENGINE_API FL2Event
{
	INT EventID;
	BYTE EventCmd;
};

struct ENGINE_API FServerPointStruct
{
	FStringNoInit Name;
	FVector point;
	FPlane Color;
};

struct ENGINE_API FServerLineStruct
{
	FStringNoInit Name;
	FVector Start;
	FVector End;
	FPlane Color;
};

struct ENGINE_API FL2RotatorTime
{
	FLOAT PitchTime;
	FLOAT RollTime;
	FLOAT YawTime;
};

struct ENGINE_API FNMoverPtr
{
	INT Ptr;
};

struct ENGINE_API FNBoneScalerStatPtr
{
	INT Ptr;
};

struct ENGINE_API FNAppendixEffectPtr
{
	INT Ptr;
};

struct ENGINE_API FNWeaponEffectPtr
{
	INT Ptr;
};

struct ENGINE_API FNCursedWeaponEffectPtr
{
	INT Ptr;
};

class ENGINE_API FNViewShake
{
	FNViewShake(AActor*);
	FNViewShake(FNViewShake const&);
	virtual ~FNViewShake();

	void Init(unsigned char, FLOAT);
	INT CheckShake(FLOAT&, FLOAT&, FLOAT&, FLOAT&, FLOAT);
	INT Update(FLOAT);
};

struct ENGINE_API FNViewShakePtr
{
	INT Ptr;
};

struct ENGINE_API FNViewShakeMgrPtr
{
	INT Ptr;
};

struct ENGINE_API FNMagicInfo
{
	class USkillVisualEffect* Agent;
	INT DummyPtr;
	INT MagicID;
	INT LevelID;
	INT AniIndex;
	INT FlexibleAniIndex;
	INT StageShot;
	INT StagePreshot;
	FLOAT SkillHitTime;
	FLOAT ShotTime;
	FLOAT TweenTime;
	FLOAT ActiveTime;
	class AActor* TargetPawn;
	FLOAT MagicSpeed;
	INT MagicAniStatus;
	INT MagicType;
	BITFIELD bTargetExcepted : 1 GCC_PACK(4);
	INT nSlice GCC_PACK(4);
	FRotator NormalRotationRate;
	FRotator SkillRotationRate;
	TArrayNoInit<class AActor*> AssociatedActor;
	TArrayNoInit<class AActor*> EffectActor;
	TArrayNoInit<INT> EffectID;
	TArrayNoInit<FVector> LocLIst;
	FName Anis[3];
	FLOAT AniDues[3];
	FName LastShotName;
	INT PendingNotify;
	INT PendingPreshotNotify;
	INT PendingChannelingNotify;
};

struct ENGINE_API FTextureModifyinfo
{
	BITFIELD bUseModify : 1 GCC_PACK(4);
	BITFIELD bTwoSide : 1;
	BITFIELD bAlphaBlend : 1;
	BITFIELD bDummy : 1;
	FColor Color GCC_PACK(4);
	INT AlphaOp;
	INT ColorOp;
};

struct ENGINE_API FNpcPos
{
	FVector Delta;
	INT Yaw;
	INT Percent;
};

struct ENGINE_API FNpcPrivate
{
	FName Name;
	FName ai;
	FLOAT Num;
};

struct ENGINE_API FWhenExtinctionCreate
{
	FStringNoInit respawn;
	FStringNoInit Name;
};

struct ENGINE_API FWayPoint
{
	FVector point;
	FStringNoInit Delay;
};

struct ENGINE_API FAnywhereNpc
{
	FName NpcName;
	INT	total;
	FStringNoInit respawn;
	FStringNoInit nickname;
	FName ai;
	TArrayNoInit<FNpcPrivate> Privates;
	FWhenExtinctionCreate when_extinction_create;
	TArrayNoInit<FWayPoint> WayPoints;
	BITFIELD bWayPointsShow:1;
};

enum InitalSpawn;

struct ENGINE_API FAnywhereNpcMaker
{
	InitalSpawn inital_spawn;
	INT maximum_npc;
	TArrayNoInit<FAnywhereNpc> Npc;
	BITFIELD bGroup:1;
};

struct ENGINE_API FNActionPtr
{
	INT Ptr;
};

struct ENGINE_API FNPawnLightPtr
{
	INT Ptr;
};
struct ENGINE_API FNAbnormalStatPtr
{
	INT Ptr;
};

struct ENGINE_API FNMoverTarget
{
	INT bTarget;
	INT bOwnedTarget;
	FVector Loc;
	AActor* Target;
};

struct ENGINE_API FNTimeHSV
{
	INT Time;
	BYTE Hue;
	BYTE Sat;
	FLOAT Bri;
};

struct ENGINE_API FNTimeColor
{
	INT Time;
	BYTE R;
	BYTE G;
	BYTE B;
};

struct ENGINE_API FFWind
{
	FLOAT LifeTime;
	FBox Area;
	FVector Direction;
	FLOAT Force;
	FLOAT Speed;
	FLOAT AccelSpeed;
	FLOAT DeltaSeconds;
};

struct ENGINE_API FTerrainIntensityMap
{
	FLOAT Time;
	TArrayNoInit<BYTE> Intensity;
};

struct ENGINE_API FL2EnvironmentColorInfo
{
	FLOAT Time;
	FColor FogColor;
	FColor SkyColor;
	FColor CloudColor;
	FColor HazeRingColor;
};

struct ENGINE_API FSkillSlice
{
	FName AnimName;
	FLOAT EndTIme;
};

struct ENGINE_API FNTimeScale
{
	FLOAT Time;
	FLOAT Scale;
};

/*
class ENGINE_API AL2FogInfo : public AInfo
{
public:
	FRange AffectRange;
	FRange FogRange1;
	FRange FogRange2;
	FRange FogRange3;
	FRange FogRange4;
	FRange FogRange5;
	FLOAT TextureDistance;
	TArrayNoInit<FL2EnvironmentColorInfo> Colors;
	class UMaterial* CloudTexture;
	DECLARE_CLASS(AL2FogInfo, AInfo, 0, Engine)
	NO_DEFAULT_CONSTRUCTOR(AL2FogInfo)
};*/

enum SEAMLESS_RESULT {};

enum ColosseumFenceState {};

struct RecommandedDataStr {};

struct Item {};

struct ItemInfo {};

struct FServerStaticObject {};

struct FVehicle {};

struct PledgeInfo {};

struct MacroInfo {};

struct HennaInfo {};

struct ColosseumFence {};

enum ItemSlotType
{
	ITST_MAX
};

struct ENGINE_API User
{
public:
	User();
	FName GetClassNameW();
	FName GetDamageEffect();
	FName GetNpcMeshName();
	FName GetNpcTexName(int);
	FName GetPcExMeshName(int);
	FName GetPcExTexName(int);
	FName GetPcMeshName(int);
	FName GetPcTexName(int, int);
	int GetAnimType(class APawn*);
	int GetArrowItemID();
	int GetCriminalRate();
	int GetGuiltyStatus();
	int GetHairMeshType(enum EPawnSubMeshStyle);
	int GetItemClassID(int);
	int GetMeshType();
	int GetNpcTexNum();
	int GetPcTexNum(int);
	int GetPetID();
	int GetPledgeID();
	int GetPrivateStoreState();
	int GetSummonedID();
	int GetSurrenderWarID();
	int HasBootsSound(int);
	int HaveItem(enum ItemSlotType);
	int IsMyPartyMember();
	int IsMyPledgeMember();
	int IsPartyMaster();
	int IsPartyMember();
	int IsPledgeMaster();
	unsigned long GetNameColor(bool);
	unsigned long GetNickColor();
	unsigned long GetUniqueNameColor();
	TCHAR* GetClassNamePointer();

	TCHAR* GetName();
	void SetName(TCHAR*);

	TCHAR* GetNickName();
	void SetNickName(TCHAR*);

	void IsMatchedStoreMsg(TCHAR*);
	void SetAttackItemVariationOption();
	void SetEventmatchEffect(int);
	void SetItemSlotByItemClassID();
	
	
	void SetPawnResource();
	void SetPledgePower(unsigned int);
};

enum L2FontType {};

enum EFontExceptionType {};

enum TargetRenderType {};

struct ThaiCharacter {};

class FL2ColorFontInfo {};

enum ELightColorType {};

class FNPawnLight {};