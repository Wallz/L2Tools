class L2NTimeLight extends Object;

struct NTimeColor
{
	var int Time;
	var byte R, G, B;
};

struct NTimeHSV
{
	var int Time;
	var byte Hue;
	var byte Sat;
	var float Bri;
};

struct NTimeScale
{
	var float Time;
	var float Scale;
};

var bool				bLoaded;
var array<NTimeHSV>		TerrainLight;
var array<NTimeHSV>		ActorLight;
var array<NTimeHSV>		StaticMeshLight;
var array<NTimeHSV>		BSPLight;

defaultproperties
{
}
