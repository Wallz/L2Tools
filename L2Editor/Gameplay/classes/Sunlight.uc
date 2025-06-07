//=============================================================================
// Directional sunlight
//=============================================================================
class Sunlight extends Light;

// merge_hack missing #exec Texture Import File=Textures\SunIcon.pcx  Name=SunIcon Mips=Off MASKED=1

defaultproperties
{
	Texture=S_Actor // merge_hack - missing this resource SunIcon
	bDirectional=True
	LightEffect=LE_Sunlight
}
