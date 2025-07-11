//=============================================================================
// ParticleEmitter: Base class for sub- emitters.
//
// make sure to keep structs in sync in UnParticleSystem.h
//=============================================================================

class ParticleEmitter extends Object
	abstract
	editinlinenew
	native;

enum EBlendMode
{
	BM_MODULATE,
	BM_MODULATE2X,
	BM_MODULATE4X,
	BM_ADD,
	BM_ADDSIGNED,
	BM_ADDSIGNED2X,
	BM_SUBTRACT,
	BM_ADDSMOOTH,
	BM_BLENDDIFFUSEALPHA,
	BM_BLENDTEXTUREALPHA,
	BM_BLENDFACTORALPHA,
	BM_BLENDTEXTUREALPHAPM,
	BM_BLENDCURRENTALPHA,
	BM_PREMODULATE,
	BM_MODULATEALPHA_ADDCOLOR,
	BM_MODULATEINVALPHA_ADDCOLOR,
	BM_MODULATEINVCOLOR_ADDALPHA,
	BM_HACK	
};

enum EParticleDrawStyle
{
	PTDS_Regular,
	PTDS_AlphaBlend,
	PTDS_Modulated,
	PTDS_Translucent,
	PTDS_AlphaModulate_MightNotFogCorrectly,
	PTDS_Darken,
	PTDS_Brighten
};

enum EParticleCoordinateSystem
{
	PTCS_Independent,
	PTCS_Relative,
	PTCS_Absolute,
//#ifdef __L2 //kurt
	PTCS_RelativeRotation,
	// __L2 // by nonblock
	PTCS_Spray
//#endif
};

//solasys
enum EParticleParticleChoice
{
	PTWC_MaxParticle,
	PTWC_ParticlePerSecond
};

enum EParticleRotationSource
{
	PTRS_None,
	PTRS_Actor,
	PTRS_Offset,
	PTRS_Normal
};

enum EParticleVelocityDirection
{
	PTVD_None,
	PTVD_StartPositionAndOwner,
	PTVD_OwnerAndStartPosition,
	PTVD_AddRadial
};

enum EParticleStartLocationShape
{
	PTLS_Box,
	PTLS_Sphere,
	PTLS_Polar,
	PTLS_All
};

enum EParticleEffectAxis
{
	PTEA_NegativeX,
	PTEA_PositiveZ
};

enum EParticleCollisionSound
{
	PTSC_None,
	PTSC_LinearGlobal,
	PTSC_LinearLocal,
	PTSC_Random
};

enum EParticleMeshSpawning
{
	PTMS_None,
	PTMS_Linear,
	PTMS_Random
};

enum ESkelLocationUpdate
{
	PTSU_None,
	PTSU_SpawnOffset,
	PTSU_Location
};

struct ParticleTimeScale
{
	var () float	RelativeTime;		// always in range [0..1]
	var () float	RelativeSize;
};

struct ParticleRevolutionScale
{
	var () float	RelativeTime;		// always in range [0..1]
	var () vector	RelativeRevolution;
};

struct ParticleColorScale
{
	var () float	RelativeTime;		// always in range [0..1]
	var () color	Color;
};

struct ParticleVelocityScale
{
	var () float	RelativeTime;		// always in range [0..1]
	var () vector	RelativeVelocity;
};

struct Particle
{
	var vector	Location;
	var vector	OldLocation;
	var vector	Velocity;
	var vector	StartSize;
	var vector	SpinsPerSecond;
	var vector	StartSpin;
	var vector  RevolutionCenter;
	var vector  RevolutionsPerSecond;
	var vector	RevolutionsMultiplier;
	var vector	Size;
	var vector  StartLocation;
	var vector  ColorMultiplier;
	var vector	VelocityMultiplier;
	var vector	OldMeshLocation;
	var color	Color;
	var float	Time;
	var float	MaxLifetime;
	var float	Mass;
	var int		HitCount;
	var int		Flags;
	var int		Subdivision;
	var int 	BoneIndex;
//#ifdef __L2 //kurt
	var Rotator	Rotation;
	var vector	Acceleration;
//#endif
};

struct ParticleSound
{
	var () sound	Sound;
	var () range	Radius;
	var () range	Pitch;
	var () int		Weight;
	var () range	Volume;
	var () range	Probability;
};

var (Acceleration)	vector						Acceleration;

//#ifdef __L2 // by nonblock
var (Acceleration)	bool						IndependentSprayAccel;	// if PTCS_Spray and true, VelocityLoss and Accel are 'PTCS_Independent' value.
//#endif

var (Collision)		bool						UseCollision;
var (Collision)		vector						ExtentMultiplier;
var (Collision)		rangevector					DampingFactorRange;
var (Collision)		bool						UseCollisionPlanes;
var (Collision)		array<plane>				CollisionPlanes;
var	(Collision)		bool						UseMaxCollisions;
var (Collision)		range						MaxCollisions;
var (Collision)		int							SpawnFromOtherEmitter;
var (Collision)		int							SpawnAmount;
var (Collision)		rangevector					SpawnedVelocityScaleRange;
var (Collision)		bool						UseSpawnedVelocityScale;

var (Color)			bool						UseColorScale;
var (Color)			array<ParticleColorScale>	ColorScale;
var (Color)			float						ColorScaleRepeats;
var (Color)			rangevector					ColorMultiplierRange;
var (Color)			float						Opacity; // Added by Demiurge (PSE)
// #ifdef __L2 nonblock
var (Color)			transient float				OpacityRatio;
// #endif

var (Fading)		plane						FadeOutFactor;
var (Fading)		float						FadeOutStartTime;
var (Fading)		bool						FadeOut;
var (Fading)		plane						FadeInFactor;
var (Fading)		float						FadeInEndTime;
var (Fading)		bool						FadeIn;

var (Force)			bool						UseActorForces;

var (General)		EParticleCoordinateSystem	CoordinateSystem;
var (General)		const int					MaxParticles;
var (General)		bool						ResetAfterChange;
var (General)		EParticleEffectAxis			EffectAxis;

//solasys
var (General)		const int					LevelOfWeather;
var (General)		const int					CheckLevelOfWeather;
var (General)		EParticleParticleChoice		ParticleChoice;
var (General)		bool						WeatherEffect;
var (General)		array<range>				WeatherRangeTime;
var (General)		bool						WeatherSoundCheck;

//#ifdef __L2//kurt
var (General)		bool						ForcedLifeTime;
var (General)		bool						ForcedFade;
var (General)		bool						ForcedMaxParticles;
//#endif

var (Local)			bool						RespawnDeadParticles;
var (Local)			bool						AutoDestroy;
var (Local)			bool						AutoReset;
var (Local)			bool						Disabled;
var (Local)			bool						DisableFogging;
var (Local)			range						AutoResetTimeRange;
var (Local)			string						Name;

var (Location)		vector						StartLocationOffset;
var (Location)		rangevector					StartLocationRange;
var (Location)		int							AddLocationFromOtherEmitter;
var (Location)		EParticleStartLocationShape StartLocationShape;
var (Location)		range						SphereRadiusRange;
var (Location)		rangevector					StartLocationPolarRange;

var (Mass)			range						StartMassRange;

var (MeshSpawning)	staticmesh					MeshSpawningStaticMesh;
var (MeshSpawning)	EParticleMeshSpawning		MeshSpawning;
var (MeshSpawning)	bool						VelocityFromMesh;
var (MeshSpawning)	rangevector					VelocityScaleRange;
var (MeshSpawning)	rangevector					MeshScaleRange;
var (MeshSpawning)	bool						UniformMeshScale;
var (MeshSpawning)	bool						UniformVelocityScale;
var (MeshSpawning)	bool						UseColorFromMesh;
var (MeshSpawning)	bool						SpawnOnlyInDirectionOfNormal;
var (MeshSpawning)	vector						MeshNormal;
var (MeshSpawning)	range						MeshNormalThresholdRange;

var (Rendering)		int							AlphaRef;
var (Rendering)		bool						AlphaTest;
var (Rendering)		bool						AcceptsProjectors;
var (Rendering)		bool						ZTest;
var (Rendering)		bool						ZWrite;

var (Revolution)	bool						UseRevolution;
var (Revolution)	rangevector					RevolutionCenterOffsetRange;
var (Revolution)	rangevector					RevolutionsPerSecondRange;
var (Revolution)	bool						UseRevolutionScale;
var (Revolution)	array<ParticleRevolutionScale> RevolutionScale;
var (Revolution)	float						RevolutionScaleRepeats;

var (Rotation)		EParticleRotationSource		UseRotationFrom;
var (Rotation)		bool						SpinParticles;
var (Rotation)		rotator						RotationOffset;
var (Rotation)		vector						SpinCCWorCW;
var (Rotation)		rangevector					SpinsPerSecondRange;
var (Rotation)		rangevector					StartSpinRange;
var (Rotation)		bool						DampRotation;
var (Rotation)		rangevector					RotationDampingFactorRange;
var (Rotation)		vector						RotationNormal;

var (Size)			bool						UseSizeScale;
var (Size)			bool						UseRegularSizeScale;
var (Size)			array<ParticleTimeScale>	SizeScale;
var (Size)			float						SizeScaleRepeats;
var (Size)			rangevector					StartSizeRange;
var (Size)			bool						UniformSize;

var (SkeletalMesh)	ESkelLocationUpdate			UseSkeletalLocationAs;
var	(SkeletalMesh)	actor						SkeletalMeshActor;
var (SkeletalMesh)	vector						SkeletalScale;
var (SkeletalMesh)	range						RelativeBoneIndexRange;

var (Sound)			array<ParticleSound>		Sounds;
var (Sound)			EParticleCollisionSound		SpawningSound;
var (Sound)			range						SpawningSoundIndex;
var (Sound)			range						SpawningSoundProbability;
var (Sound)			EParticleCollisionSound		CollisionSound;
var (Sound)			range						CollisionSoundIndex;
var (Sound)			range						CollisionSoundProbability;

var (Spawning)		float						ParticlesPerSecond;
var (Spawning)		float						InitialParticlesPerSecond;
var (Spawning)		bool						AutomaticInitialSpawning;
	
var (Texture)		EParticleDrawStyle			DrawStyle;
var (Texture)		texture						Texture;
var (Texture)		int							TextureUSubdivisions;
var (Texture)		int							TextureVSubdivisions;
var (Texture)		bool						BlendBetweenSubdivisions;
var	(Texture)		bool						UseSubdivisionScale;
var (Texture)		array<float>				SubdivisionScale;
var (Texture)		int							SubdivisionStart;
var (Texture)		int							SubdivisionEnd;
var (Texture)		bool						UseRandomSubdivision;

var (Tick)			float						SecondsBeforeInactive;
var (Tick)			float						MinSquaredVelocity;

var	(Time)			range						InitialTimeRange;
var (Time)			range						LifetimeRange;
var (Time)			range						InitialDelayRange;

var (Trigger)		bool						TriggerDisabled;
var (Trigger)		bool						ResetOnTrigger;
var (Trigger)		range						SpawnOnTriggerRange;
var (Trigger)		float						SpawnOnTriggerPPS;

var (Velocity)		rangevector					StartVelocityRange;
var (Velocity)		range						StartVelocityRadialRange;
var (Velocity)		vector						MaxAbsVelocity;
var (Velocity)		rangevector					VelocityLossRange;
var (Velocity)		int							AddVelocityFromOtherEmitter;
var (Velocity)		rangevector					AddVelocityMultiplierRange;
var (Velocity)		EParticleVelocityDirection	GetVelocityDirectionFrom;
var (Velocity)		bool						UseVelocityScale;
var (Velocity)		array<ParticleVelocityScale> VelocityScale;
var (Velocity)		float						VelocityScaleRepeats;

var (Warmup)		float						WarmupTicksPerSecond;
var (Warmup)		float						RelativeWarmupTime;

var transient		emitter						Owner;
var	transient		bool						Initialized;
var transient		bool						Inactive;
var transient		float						InactiveTime;
var transient		array<Particle>				Particles;
var transient		int							ParticleIndex;			// index into circular list of particles
var transient		int							ActiveParticles;		// currently active particles
var transient		float						PPSFraction;			// used to keep track of fractional PPTick
var transient		box							BoundingBox;

var transient		vector						RealExtentMultiplier;
var	transient		bool						RealDisableFogging;
var transient		bool						AllParticlesDead;
var transient		bool						WarmedUp;
var	transient		int							OtherIndex;
var transient		float						InitialDelay;
var transient		vector						GlobalOffset;
var transient		float						TimeTillReset;
var transient		int							PS2Data;
var transient		int							MaxActiveParticles;
var transient		int							CurrentCollisionSoundIndex;
var transient		int							CurrentSpawningSoundIndex;
var transient		int							CurrentMeshSpawningIndex;
var transient		float						MaxSizeScale;
var transient		int							KillPending;
var transient		int							DeferredParticles;
var transient		vector						RealMeshNormal;
var transient		array<vector>				MeshVertsAndNormals;
var transient		int							CurrentSpawnOnTrigger;

//#ifdef __L2 //kurt
var	transient		bool						bOwnerTracking;
var	transient		float						CurLifeTime;
var	transient		bool						bNotifyPreDestroy;
//#endif

native function SpawnParticle( int Amount );
native function Trigger();

defaultproperties
{
     ExtentMultiplier=(X=1.000000,Y=1.000000,Z=1.000000)
     DampingFactorRange=(X=(Min=1.000000,Max=1.000000),Y=(Min=1.000000,Max=1.000000),Z=(Min=1.000000,Max=1.000000))
     SpawnFromOtherEmitter=-1
     ColorMultiplierRange=(X=(Min=1.000000,Max=1.000000),Y=(Min=1.000000,Max=1.000000),Z=(Min=1.000000,Max=1.000000))
     Opacity=1.000000
     FadeOutFactor=(W=1.000000,X=1.000000,Y=1.000000,Z=1.000000)
     FadeInFactor=(W=1.000000,X=1.000000,Y=1.000000,Z=1.000000)
     CoordinateSystem=PTCS_Relative
     MaxParticles=10
     LevelOfWeather=2
     RespawnDeadParticles=True
     AddLocationFromOtherEmitter=-1
     StartMassRange=(Min=1.000000,Max=1.000000)
     VelocityScaleRange=(X=(Min=1.000000,Max=1.000000),Y=(Min=1.000000,Max=1.000000),Z=(Min=1.000000,Max=1.000000))
     MeshScaleRange=(X=(Min=1.000000,Max=1.000000),Y=(Min=1.000000,Max=1.000000),Z=(Min=1.000000,Max=1.000000))
     UniformMeshScale=True
     UniformVelocityScale=True
     MeshNormal=(Z=1.000000)
     AlphaTest=True
     ZTest=True
     SpinCCWorCW=(X=0.500000,Y=0.500000,Z=0.500000)
     UseRegularSizeScale=True
     StartSizeRange=(X=(Min=100.000000,Max=100.000000),Y=(Min=100.000000,Max=100.000000),Z=(Min=100.000000,Max=100.000000))
     SkeletalScale=(X=1.000000,Y=1.000000,Z=1.000000)
     RelativeBoneIndexRange=(Max=1.000000)
     AutomaticInitialSpawning=True
     DrawStyle=PTDS_Translucent
     Texture=Texture'Engine.S_Emitter'
     LifetimeRange=(Min=4.000000,Max=4.000000)
     TriggerDisabled=True
     MaxAbsVelocity=(X=10000.000000,Y=10000.000000,Z=10000.000000)
     AddVelocityFromOtherEmitter=-1
     AddVelocityMultiplierRange=(X=(Min=1.000000,Max=1.000000),Y=(Min=1.000000,Max=1.000000),Z=(Min=1.000000,Max=1.000000))
}
