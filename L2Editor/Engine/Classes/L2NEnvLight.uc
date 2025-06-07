class L2NEnvLight extends L2NTimeLight
	native;

var array<NTimeColor>	SkyColor;
var array<int> HazeColorIndex;
var array<NTimeColor>	HazeColor;
var array<int> CloudColorIndex;
var array<NTimeColor>	CloudColor;
var array<NTimeColor>	StarColor;
var array<NTimeColor>	SunColor;
var array<NTimeColor>	MoonColor;

var array<NTimeColor>	TerrainAmbient;
var array<NTimeColor>	ActorAmbient;
var array<NTimeColor>	StaticMeshAmbient;
var array<NTimeColor>	BSPAmbient;

var array<NTimeScale>	SunScale;
var array<NTimeScale>	MoonScale;

var int					EnvType;

defaultproperties
{
}
