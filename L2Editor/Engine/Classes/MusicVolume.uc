//=============================================================================
// MusicVolume:  
//=============================================================================
class MusicVolume extends Volume
	native
	nativereplication;

var(Music) int nMusicID;
var(Music) bool bForcePlayMusic;
var(Music) bool bLoopMusic;

simulated function PostBeginPlay()
{
	Super.PostBeginPlay();
}

defaultproperties
{
     nMusicID=-1
     DrawType=DT_MusicVolume
}
