class RibbonEmitter extends ParticleEmitter
	native;

struct native RibbonPoint
{
	//#ifdef __L2
	var() vector Location;
	var() vector LocationUnder;
	var() vector MovingDirection;	
	var() vector MovingDirectionUnder;	
	var() float Width;
	//#endif

	//var() vector Location;	
	//var() vector AxisNormal;
	//var() float Width;
};

enum EGetPointAxis
{
	PAXIS_OwnerX, // owners X axis based on rotation
	PAXIS_OwnerY, // owners Y axis based on rotation
	PAXIS_OwnerZ, // owners Z axis based on rotation
	PAXIS_BoneNormal, // (end - start) or start bone direction if no end bone found
	PAXIS_StartBoneDirection, // start bones direction
	PAXIS_AxisNormal // specified normal
};

// main vars
var(Ribbon) float SampleRate;
var(Ribbon) float DecayRate;	// unused
var(Ribbon) int NumPoints;

//#ifdef __L2. nonblock. accelerative dropping
enum EAccDrop
{
	ADRP_NONE,		// not using accelerative drop. (rely on decaying )
	ADRP_BYPOINT,	// start acc drop, when RibbonPoints.Num() exceeds NumPoints.
	ADRP_BYTIME,	// start acc drop, when Time > ThresTime
	ADRP_BYTIME_DUAL // start acc drop, and process, then drop again.
};

var(RibbonAccDrop) EAccDrop AccDrop;
var(RibbonAccDrop) int PointsDropRate;	// number of dying points a tick.
var(RibbonAccDrop) float ThresholdTime;	// ADRP_BYTIME, ADRP_BYTIME_DUAL
var(RibbonAccDrop) int MinPoints;		// ADRP_BYPOINT, ADRP_BYTIME, ADRP_BYTIME_DUAL
var(RibbonAccDrop) float ThresholdTime2;	// ADRP_BYTIME_DUAL
//#endif

var(Ribbon) float RibbonWidth;
var(Ribbon) EGetPointAxis GetPointAxisFrom;
var(Ribbon) vector AxisNormal; // used for PAXIS_AxisNormal
var(Ribbon) float MinSampleDist;
var(Ribbon) float MinSampleDot;
var(Ribbon) float PointOriginOffset;

// texture UV scaling
var(RibbonTexture) float RibbonTextureUScale;
var(RibbonTexture) float RibbonTextureVScale;

// axis rotated sheets
var(RibbonSheets) int NumSheets; // number of sheets used
var(RibbonSheets) array<float> SheetScale;

// bone vars (emitter must have an actor with a skeletal mesh as its owner)
var(RibbonBones) vector StartBoneOffset;
var(RibbonBones) vector EndBoneOffset;
var(RibbonBones) name BoneNameStart;
var(RibbonBones) name BoneNameEnd;

// ribbon point array
var(Ribbon) array<RibbonPoint> RibbonPoints;

//#ifdef __L2. nonblock. interpolation
var(RibbonInterpolation) bool bUseInterpolation;
var(RibbonInterpolation) int CntrPoints;
var(RibbonInterpolation) float ScaleRatio;
// #endif

// flags
var(Ribbon) bool bSamplePoints;
var(Ribbon) bool bDecayPoints;
var(Ribbon) bool bDecayPointsWhenStopped;
var(Ribbon) bool bSyncDecayWhenKilled;
var(RibbonTexture) bool bLengthBasedTextureU;
var(RibbonSheets) bool bUseSheetScale;
var(RibbonBones) bool bUseBones;
var(RibbonBones) bool bUseBoneDistance; // get width from distance between start and end bones

// internal vars
var transient float SampleTimer; // sample timer (samples point at SampleTimer >= SampleRate)
var transient float DecayTimer;
var transient float RealSampleRate;
var transient float RealDecayRate;
var transient int SheetsUsed;
var transient RibbonPoint LastSampledPoint;

var transient bool bKilled; // used to init vars when particle emitter is killed
var transient bool bDecaying;

defaultproperties
{
     SampleRate=0.050000
     NumPoints=20
     RibbonWidth=20.000000
     GetPointAxisFrom=PAXIS_AxisNormal
     AxisNormal=(Z=1.000000)
     MinSampleDist=1.000000
     MinSampleDot=0.995000
     PointOriginOffset=0.500000
     RibbonTextureUScale=1.000000
     RibbonTextureVScale=1.000000
     CntrPoints=5
     ScaleRatio=3.000000
     bSamplePoints=True
     bDecayPoints=True
     MaxParticles=1
     UseRegularSizeScale=False
     StartSizeRange=(X=(Min=1.000000,Max=1.000000),Y=(Min=1.000000,Max=1.000000),Z=(Min=1.000000,Max=1.000000))
     InitialParticlesPerSecond=10000.000000
     AutomaticInitialSpawning=False
}
