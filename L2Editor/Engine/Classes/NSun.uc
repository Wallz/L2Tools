//=============================================================================
// Sun
// 
//=============================================================================
class NSun extends Actor
	placeable
	native;

var(Display)		float	Radius;
var					float	Latitude;
var					float	Longitude;
var(Display)		float	LimitMaxRadius;
var(Display)		float	SunScale;
var					bool	bMakeLightmap;
var					vector  Position;
//#exec Texture Import File=Textures\sun.pcx Name=moon Mips=Off MASKED=1
//#exec Texture Import File=Textures\ZoneInfo.pcx Name=S_ZoneInfo Mips=Off MASKED=1
//#exec OBJ LOAD FILE=..\Textures\Skies.utx PACKAGE=Skies

simulated function PostBeginPlay()
{
	//local	mesh	temp;

	Super.PostBeginPlay();

	//texture = texture(DynamicLoadObject("Skies.CogSky.Sun", class'texture'));
	//texture = TexScaler(DynamicLoadObject("Skies.CogSky.SunFlare", class'TexScaler'));
}

defaultproperties
{
     Radius=32768.000000
     LimitMaxRadius=32768.000000
     SunScale=1.000000
     DrawType=DT_Sun
     bAcceptsProjectors=False
     bNetTemporary=True
     RemoteRole=ROLE_None
     bIgnoredRange=True
     bGameRelevant=True
}
