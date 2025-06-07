//=============================================================================
// The EmitterLight class.
//=============================================================================
class EmitterLight extends Light
	native;

var(EmitterLight) enum EEmitterLightType
{	
	EEL_PawnOnly,
	EEL_WorldOnly,
	EEL_All,
	
} EmitterLightType;

defaultproperties
{
     bStatic=False
     bNoDelete=False
     bDynamicLight=True
}
