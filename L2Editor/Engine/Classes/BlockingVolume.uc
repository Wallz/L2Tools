//=============================================================================
// BlockingVolume:  a bounding volume
// used to block certain classes of actors
// primary use is to provide collision for non-zero extent traces around static meshes 

//=============================================================================

//#ifdef __L2 // zodiac
class BlockingVolume extends Volume
	native
	nativereplication;
//#else
//class BlockingVolume extends Volume
//	native;
//#endif


var() bool bClampFluid;

defaultproperties
{
     bClampFluid=True
     bWorldGeometry=True
     bBlockActors=True
     bBlockPlayers=True
     bBlockZeroExtentTraces=False
     bBlockKarma=True
}
