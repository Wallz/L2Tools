//=============================================================================
// Ambient sound, sits there and emits its sound.  This class is no different 
// than placing any other actor in a level and setting its ambient sound.
//=============================================================================
class AmbientSoundObject extends LevelObject
	placeable
	native;

// Import the sprite.
#exec Texture Import File=Textures\Ambient.pcx Name=S_Ambient Mips=Off MASKED=1

//#ifdef	__L2	Hunter
enum	ASType1{
	AST1_Always,
	AST1_Day,
	AST1_Night,
	AST1_Water};
var(Sound)	ASType1		AmbientSoundType;
var(Sound)	int			AmbientRandom;	 // Radius of ambient sound.
var			float		AmbientSoundStartTime;	//Sound Run-Time Variable
//#endif

// Ambient sound.
var(Sound) sound		AmbientSound;			// Ambient sound effect.
var(Sound) float        SoundRadius;			// Radius of ambient sound.
var(Sound) byte         SoundVolume;			// Volume of ambient sound.
var(Sound) byte         SoundPitch;				// Sound pitch shift, 64.0=none.

defaultproperties
{
     AmbientRandom=100
     SoundRadius=64.000000
     SoundVolume=190
     SoundPitch=64
     Texture=Texture'Engine.S_Ambient'
}
