////////////////////////////////////////////
// AirVolume
////////////////////////////////////////////
//#ifdef __L2 // by nonblock
class AirVolume extends Volume
	native
	nativereplication;

var(AirVolume) name EffectName;
var(AirVolume) float FullFadeSeconds;
var(AirVolume) float RelativeOffset;


simulated function PostBeginPlay()
{
	Super.PostBeginPlay();
}


//#endif

defaultproperties
{
     FullFadeSeconds=60.000000
     DrawType=DT_MusicVolume
}
