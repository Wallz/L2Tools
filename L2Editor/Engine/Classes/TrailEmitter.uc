//=============================================================================
// Emitter: An Unreal Trail Particle Emitter.
//=============================================================================
class TrailEmitter extends ParticleEmitter	
	native;

struct ParticleTrailData
{
	var vector	Location;
	var color	Color;
	var float	Size;
	var int		DoubleDummy1;
	var int		DoubleDummy2;
};

struct ParticleTrailInfo
{
	var int		TrailIndex;
	var int		NumPoints;	
	var vector	LastLocation;
	//__L2 kurt
	var int		PreDestroyTrailNum;
};

var (Trail)			int							MaxPointsPerTrail;


var	(Trail)			range							DistanceRange;	// __L2 by nonblock
// var (Trail)			float						DistanceThreshold;

var (Trail)			bool						UseCrossedSheets;
var (Trail)			int							MaxTrailTwistAngle;

var transient		array<ParticleTrailData>	TrailData;
var transient		array<ParticleTrailInfo>	TrailInfo;
var transient		vertexbuffer				VertexBuffer;
var transient		indexbuffer					IndexBuffer;
var transient		int							VerticesPerParticle;
var transient		int							IndicesPerParticle;
var transient		int							PrimitivesPerParticle;

defaultproperties
{
     MaxPointsPerTrail=50
     DistanceRange=(Min=1.000000,Max=100.000000)
     MaxTrailTwistAngle=16384
}
