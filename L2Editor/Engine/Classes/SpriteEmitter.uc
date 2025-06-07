//=============================================================================
// Emitter: An Unreal Sprite Particle Emitter.
//=============================================================================
class SpriteEmitter extends ParticleEmitter
	native;


enum EParticleDirectionUsage
{
	PTDU_None,
	PTDU_Up,
	PTDU_Right,
	PTDU_Forward,
	PTDU_Normal,
	PTDU_UpAndNormal,
	PTDU_RightAndNormal,
	PTDU_Scale
};


var (Sprite)		EParticleDirectionUsage		UseDirectionAs;
var (Sprite)		vector						ProjectionNormal;
var (Sprite)		int							Refraction;
var (Sprite)		float						RefrUScale;
var (Sprite)		float						RefrVScale;
var transient		vector						RealProjectionNormal;

defaultproperties
{
     ProjectionNormal=(Z=1.000000)
     RefrUScale=0.060000
     RefrVScale=0.060000
}
