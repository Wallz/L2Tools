//=============================================================================
// GameEngine: The game subsystem.
// This is a built-in Unreal class and it shouldn't be modified.
//=============================================================================
class GameEngine extends Engine
	native
	noexport
	transient;

// URL structure.
struct URL
{
	var string			Protocol,	// Protocol, i.e. "unreal" or "http".
						Host;		// Optional hostname, i.e. "204.157.115.40" or "unreal.epicgames.com", blank if local.
	var int				Port;		// Optional host port.
	var string			Map;		// Map name, i.e. "SkyCity", default is "Index".
	var array<string>	Op;			// Options.
	var string			Portal;		// Portal to enter through, default is "".
	var int 			Valid;
};

//#ifdef __L2
struct FWind
{
	var float LifeTime;
	var Box Area;
	var vector Direction;
	var float Force;
	var float Speed;
	var float AccelSpeed;
	var float DeltaSeconds;
};

//#endif

var Level			GLevel,
					GEntry;
//#ifdef __L2 // gigadeth
var Level			GSkyLevel;
//#endif
var PendingLevel	GPendingLevel;
var URL				LastURL;
var config array<string>	ServerActors,
					ServerPackages;

var array<object> DummyArray;	// Do not modify
var object        DummyObject;  // Do not modify
//#ifdef	__L2	Hunter
var int				LastHitObject;
var bool			bClicked;
var float			ClickedMouseX, ClickedMouseY;
var vector			ClickLocation;
var plane			ClickPlane;
var Actor			ClickActor;
var int				StartTempNpcID;
var int				EndTempNpcID;
var int				CurrentTempNpcID;	
var	float			AvgFPS;
var	float			LastDeltaTime;
var	float			LastDeltaSeconds[30];
var	int				CurTickIndex;
//#endif
//#ifdef    __L2 // gigadeth
var array<FWind>	Wind;
var array<int>		LoadingMap;
var array<int>		CommandMacro;
var int				LandMark;
//#endif
//#ifdef __L2 // zodiac

var bool			bUseUnderWaterEffect;
var float			FadeTime;
var bool			bFadeOut;
var float			FadeElaspedTime;
var bool 			bFadeIn;
var Color			FadeColor;
var int				nFadeType;
var bool			bPlayerObserverMode;
var vector			TeleportLoc; // for use Teleport


//#ifdef __L2 // zodiac
var bool			bTimeCheck;
var int				RequestedTime;
var int				ElapsedTimeCheck;
var float			MusicMaxTime;
var float			DefaultMusicMaxTime;
// nonblock
var float			BlackOutDuration;
var float			FadeInTime;
var	bool			bLazyMode;

var int				UnderWaterEffect;
var bool			bAttachedUnderWaterEffect;
//#endif

//#ifdef	__L2	hunter
var	int				InspectorCount;
var	int				InspectorParams;
var	Class			InspectorClass;
//#ifdef	__L2	hunter
var array<object>	CameraEffects;
//#endif
//#ifdef __L2 // giga
var int				AuthData[4];
var bool			bConnectionClosed;
//#endif
//#ifdef __L2 // yohan
//var float	fSkillHittime;
//var float	fSkillCurtime;
//var bool	bSkillHitting;
//#endif

defaultproperties
{
     ServerPackages(0)="AmbientCreatures"
     ServerPackages(1)="GamePlay"
     ServerPackages(2)="UnrealGame"
     MusicMaxTime=60.000000
     DefaultMusicMaxTime=30.000000
     CacheSizeMegs=128
}
