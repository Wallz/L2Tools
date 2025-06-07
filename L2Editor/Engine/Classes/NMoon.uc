//=============================================================================
// Moon
// 
//=============================================================================
class NMoon extends Actor
	placeable
	native;

var(Display)		float	Radius;
var					float	Latitude;
var					float	Longitude;
var(Display)		float	LimitMaxRadius;
var(Display)		float	MoonScale;
var					bool	bMakeLightmap;
var					vector  Position;
var(Display)		bool    bMoonLight;
var(Display)		int		EnvType;
var(Display)		texture	Flame[12];
var(LightColor)		byte	LightHue, LightSaturation;
var(LightColor)		float	LightBrightness;

defaultproperties
{
     Radius=32768.000000
     LimitMaxRadius=32768.000000
     MoonScale=1.000000
     DrawType=DT_Sun
     bAcceptsProjectors=False
     bNetTemporary=True
     RemoteRole=ROLE_None
     bIgnoredRange=True
     bGameRelevant=True
     bDirectional=True
}
