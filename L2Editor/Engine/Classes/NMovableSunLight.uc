//=============================================================================
// Movable Directional sunlight
//=============================================================================
class NMovableSunlight extends Light
	placeable
	native;

#exec Texture Import File=Textures\SunIcon.pcx  Name=SunIcon Mips=Off MASKED=1

defaultproperties
{
     LightEffect=LE_Sunlight
     bSunlightColor=True
     bStatic=False
     Texture=Texture'Engine.SunIcon'
     bIgnoredRange=True
     bMovable=True
     bDirectional=True
}
