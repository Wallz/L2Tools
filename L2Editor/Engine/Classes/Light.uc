//=============================================================================
// The light class.
//=============================================================================
class Light extends Actor
	placeable
	native;

#exec Texture Import File=Textures\S_Light.pcx  Name=S_Light Mips=Off MASKED=1

// Light modulation.
var(Lighting) enum ELightType
{
	LT_None,
	LT_Steady,
	LT_Pulse,
	LT_Blink,
	LT_Flicker,
	LT_Strobe,
	LT_BackdropLight,
	LT_SubtlePulse,
	LT_TexturePaletteOnce,
	LT_TexturePaletteLoop,
	LT_FadeOut,
	LT_Fade
} LightType;

// Spatial light effect to use.
var(Lighting) enum ELightEffect
{
	LE_None,
	LE_TorchWaver,
	LE_FireWaver,
	LE_WateryShimmer,
	LE_Searchlight,
	LE_SlowWave,
	LE_FastWave,
	LE_CloudCast,
	LE_StaticSpot,
	LE_Shock,
	LE_Disco,
	LE_Warp,
	LE_Spotlight,
	LE_NonIncidence,
	LE_Shell,
	LE_OmniBumpMap,
	LE_Interference,
	LE_Cylinder,
	LE_Rotor,
	LE_Sunlight,
	LE_QuadraticNonIncidence
} LightEffect;

// Lighting info.
var(LightColor) float LightBrightness;
var(Lighting) float LightRadius;
var(LightColor) byte LightHue, LightSaturation;
var(Lighting) byte LightPeriod, LightPhase, LightCone;

// __L2 gigadeth
var	bool bSunlightColor;
var(Lighting) bool bTimeLight;
var(Lighting) float LightOnTime;
var(Lighting) float LightOffTime;
var float LightPrevTime;
var float LightLifeTime;

var (Corona)	float	MinCoronaSize;
var (Corona)	float	MaxCoronaSize;
var (Corona)	float	CoronaRotation;
var (Corona)	float	CoronaRotationOffset;
var (Corona)	bool	UseOwnFinalBlend;

defaultproperties
{
     LightType=LT_Steady
     LightBrightness=64.000000
     LightRadius=64.000000
     LightSaturation=255
     LightPeriod=32
     LightCone=128
     MaxCoronaSize=1000.000000
     bStatic=True
     bHidden=True
     bNoDelete=True
     Texture=Texture'Engine.S_Light'
     bMovable=False
     CollisionRadius=24.000000
     CollisionHeight=24.000000
}
