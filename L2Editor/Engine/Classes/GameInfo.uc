//=============================================================================
// GameInfo.
//
// The GameInfo defines the game being played: the game rules, scoring, what actors 
// are allowed to exist in this game type, and who may enter the game.  While the 
// GameInfo class is the public interface, much of this functionality is delegated 
// to several classes to allow easy modification of specific game components.  These 
// classes include GameInfo, AccessControl, Mutator, BroadcastHandler, and GameRules.  
// A GameInfo actor is instantiated when the level is initialized for gameplay (in 
// C++ UGameEngine::LoadMap() ).  The class of this GameInfo actor is determined by 
// (in order) either the DefaultGameType if specified in the LevelInfo, or the 
// DefaultGame entry in the game's .ini file (in the Engine.Engine section), unless 
// its a network game in which case the DefaultServerGame entry is used.  
//
//=============================================================================
class GameInfo extends Info
	native;

//-----------------------------------------------------------------------------
// Variables.

var bool				      bRestartLevel;			// Level should be restarted when player dies
var bool				      bPauseable;				// Whether the game is pauseable.
var config bool					bWeaponStay;              // Whether or not weapons stay when picked up.
var	bool				      bCanChangeSkin;			// Allow player to change skins in game.
var bool				      bTeamGame;				// This is a team game.
var	bool					  bGameEnded;				// set when game ends
var	bool					  bOverTime;
var localized bool			  bAlternateMode;
var	bool					  bCanViewOthers;
var bool					  bDelayedStart;
var bool					  bWaitingToStartMatch;
var globalconfig bool		  bChangeLevels;
var		bool				  bAlreadyChanged;
var bool						bLoggingGame;           // Does this gametype log?
var globalconfig bool			bEnableStatLogging;		// If True, games will log
var config bool					bAllowWeaponThrowing;

var globalconfig float        GameDifficulty;
var	  globalconfig int		  GoreLevel;				// 0=Normal, increasing values=less gore
var   globalconfig float	  AutoAim;					// How much autoaiming to do (1 = none, 0 = always).
														// (cosine of max error to correct)
var   globalconfig float	  GameSpeed;				// Scale applied to game rate.
var   float                   StartTime;

var   string				  DefaultPlayerClassName;

// user interface
var   string                  ScoreBoardType;           // Type of class<Menu> to use for scoreboards. (gam)
var   string			      BotMenuType;				// Type of bot menu to display.
var   string			      RulesMenuType;			// Type of rules menu to display.
var   string				  SettingsMenuType;			// Type of settings menu to display.
var   string				  GameUMenuType;			// Type of Game dropdown to display.
var   string				  MultiplayerUMenuType;		// Type of Multiplayer dropdown to display.
var   string				  GameOptionsMenuType;		// Type of options dropdown to display.
var	  string				  HUDType;					// HUD class this game uses.
var   string				  MapListType;				// Maplist this game uses.
var   string			      MapPrefix;				// Prefix characters for names of maps for this game type.
var   string			      BeaconName;				// Identifying string used for finding LAN servers.

var   globalconfig int	      MaxSpectators;			// Maximum number of spectators.
var	  int					  NumSpectators;			// Current number of spectators.
var   globalconfig int		  MaxPlayers; 
var   int					  NumPlayers;				// number of human players
var	  int					  NumBots;					// number of non-human players (AI controlled but participating as a player)
var   int					  CurrentID;
var localized string	      DefaultPlayerName;
var localized string	      GameName;
var float					  FearCostFallOff;			// how fast the FearCost in NavigationPoints falls off

var config int                GoalScore;                // what score is needed to end the match
var config int                MaxLives;	                // max number of lives for match, unless overruled by level's GameDetails
var config int                TimeLimit;                // time limit in minutes

// Message classes.
var class<LocalMessage>		  DeathMessageClass;
var class<GameMessage>		  GameMessageClass;
var	name					  OtherMesgGroup;

//-------------------------------------
// GameInfo components
var string MutatorClass;
var Mutator BaseMutator;				// linked list of Mutators (for modifying actors as they enter the game)
var globalconfig string AccessControlClass;
var AccessControl AccessControl;		// AccessControl controls whether players can enter and/or become admins
var GameRules GameRulesModifiers;		// linked list of modifier classes which affect game rules
var string BroadcastHandlerClass;
var BroadcastHandler BroadcastHandler;	// handles message (text and localized) broadcasts

var class<PlayerController> PlayerControllerClass;	// type of player controller to spawn for players logging in
var string PlayerControllerClassName;

// ReplicationInfo
var() class<GameReplicationInfo> GameReplicationInfoClass;
var GameReplicationInfo GameReplicationInfo;
var bool bWelcomePending;

// Stats - jmw
var GameStats                   GameStats;				// Holds the GameStats actor
var class<GameStats>			GameStatsClass;			// Type of GameStats actor to spawn

// Voice chat
struct VoiceChatterInfo
{
	var controller			Controller;
	var int					IpAddr;
	var int					Handle;
};
var array<VoiceChatterInfo>		VoiceChatters;

// Cheat Protection
var class<Security> 			SecurityClass;	

var() String ScreenShotName;
var() String DecoTextName;
var() String Acronym;

// localized PlayInfo descriptions & extra info
var private localized string GIPropsDisplayText[11];
var private localized string GIPropsExtras[2];

//------------------------------------------------------------------------------
// Engine notifications.

function PreBeginPlay()
{
	StartTime = 0;
	SetGameSpeed(GameSpeed);
	GameReplicationInfo = Spawn(GameReplicationInfoClass);
	InitGameReplicationInfo();
	// Create stat logging actor.
    InitLogging();
}

function UpdatePrecacheMaterials()
{
	PrecacheGameTextures(Level);
}

function UpdatePrecacheStaticMeshes()
{
	PrecacheGameStaticMeshes(Level);
}

static function PrecacheGameTextures(LevelInfo myLevel);
static function PrecacheGameStaticMeshes(LevelInfo myLevel);

function string FindPlayerByID( int PlayerID )
{
    local int i;

    for( i=0; i<GameReplicationInfo.PRIArray.Length; i++ )
    {
        if( GameReplicationInfo.PRIArray[i].PlayerID == PlayerID )
            return GameReplicationInfo.PRIArray[i].PlayerName;
    }
    return "";
}

static function bool UseLowGore()
{
	return ( Default.bAlternateMode || (Default.GoreLevel > 0) );
}
		
function PostBeginPlay()
{
	if (GameStats!=None)
	{
		GameStats.NewGame();
		GameStats.ServerInfo();
	}
}

/* Reset() 
reset actor to initial state - used when restarting level without reloading.
*/
function Reset()
{
	Super.Reset();
	bGameEnded = false;
	bOverTime = false;
	bWaitingToStartMatch = true;
	InitGameReplicationInfo();
}

/* InitLogging()
Set up statistics logging
*/
function InitLogging()
{

    if ( !bEnableStatLogging || !bLoggingGame || (Level.NetMode == NM_Standalone) || (Level.NetMode == NM_ListenServer) )
		return;

	GameStats = spawn(GameStatsClass);
}

function Timer()
{
	local NavigationPoint N;
	local int i;

    // If we are a server, broadcast a welcome message.
    if( bWelcomePending )
    {
		bWelcomePending = false;
		if ( Level.NetMode != NM_Standalone )
		{
			for ( i=0; i<GameReplicationInfo.PRIArray.Length; i++ )
				if ( (GameReplicationInfo.PRIArray[i] != None)
					&& !GameReplicationInfo.PRIArray[i].bWelcomed )
				{
					GameReplicationInfo.PRIArray[i].bWelcomed = true;
					if ( !GameReplicationInfo.PRIArray[i].bOnlySpectator )
						BroadcastLocalizedMessage(GameMessageClass, 1, GameReplicationInfo.PRIArray[i]);
				}
		}
	}

	BroadcastHandler.UpdateSentText();
    for ( N=Level.NavigationPointList; N!=None; N=N.NextNavigationPoint )
		N.FearCost *= FearCostFallOff;
}

// Called when game shutsdown.
event GameEnding()
{
	EndLogging("serverquit");
}

//------------------------------------------------------------------------------
// Replication

function InitGameReplicationInfo()
{
	GameReplicationInfo.bTeamGame = bTeamGame;
	GameReplicationInfo.GameName = GameName;
	GameReplicationInfo.GameClass = string(Class);
    GameReplicationInfo.MaxLives = MaxLives;
}

native function string GetNetworkNumber();

//------------------------------------------------------------------------------
// Server/Game Querying.

function GetServerInfo( out ServerResponseLine ServerState )
{
	ServerState.ServerName		= GameReplicationInfo.ServerName;
	ServerState.MapName			= Left(string(Level), InStr(string(Level), "."));
	ServerState.GameType		= Mid( string(Class), InStr(string(Class), ".")+1);
	ServerState.CurrentPlayers	= GetNumPlayers();
	ServerState.MaxPlayers		= MaxPlayers;
	ServerState.IP				= ""; // filled in at the other end.
	ServerState.Port			= GetServerPort();

	ServerState.ServerInfo.Length = 0;
	ServerState.PlayerInfo.Length = 0;
}

function int GetNumPlayers()
{
	return NumPlayers;
}

function GetServerDetails( out ServerResponseLine ServerState )
{
	local int i;
	local Mutator M;

	i = ServerState.ServerInfo.Length;

	// servermode
	ServerState.ServerInfo.Length = i+1;
	ServerState.ServerInfo[i].Key = "servermode";
	if( Level.NetMode==NM_ListenServer )
		ServerState.ServerInfo[i++].Value = "non-dedicated";
    else
		ServerState.ServerInfo[i++].Value = "dedicated";

	// adminname
	ServerState.ServerInfo.Length = i+1;
	ServerState.ServerInfo[i].Key = "adminname";
	ServerState.ServerInfo[i++].Value = GameReplicationInfo.AdminName;
	
	// adminemail
	ServerState.ServerInfo.Length = i+1;
	ServerState.ServerInfo[i].Key = "adminemail";
	ServerState.ServerInfo[i++].Value = GameReplicationInfo.AdminEmail;

	// has password
	if( AccessControl.RequiresPassword() )
		{
		ServerState.ServerInfo.Length = i+1;
		ServerState.ServerInfo[i].Key = "password";
		ServerState.ServerInfo[i++].Value = "true";
	}

	// has stats enabled
	ServerState.ServerInfo.Length = i+1;
	ServerState.ServerInfo[i].Key = "gamestats";
	if( GameStats!=None )
		ServerState.ServerInfo[i++].Value = "true";
	else
		ServerState.ServerInfo[i++].Value = "false";

	// game speed
	if( GameSpeed != 1.0 )
			{
		ServerState.ServerInfo.Length = i+1;
		ServerState.ServerInfo[i].Key = "gamespeed";
		ServerState.ServerInfo[i++].Value = string( int(GameSpeed*100)/100.0 );
	}

	// Ask the mutators if they have anything to add.
	for (M = BaseMutator.NextMutator; M != None; M = M.NextMutator)
		M.GetServerDetails(ServerState);
}
			
function GetServerPlayers( out ServerResponseLine ServerState )
{
    local Mutator M;
	local Controller C;
	local PlayerReplicationInfo PRI;
	local int i;

	i = ServerState.PlayerInfo.Length;

	for( C=Level.ControllerList;C!=None;C=C.NextController )
        {
			PRI = C.PlayerReplicationInfo;
			if( (PRI != None) && !PRI.bBot && MessagingSpectator(C) == None )
            {
			ServerState.PlayerInfo.Length = i+1;
			ServerState.PlayerInfo[i].PlayerNum  = C.PlayerNum;		
			ServerState.PlayerInfo[i].PlayerName = PRI.PlayerName;
			ServerState.PlayerInfo[i].Score		 = PRI.Score;			
			ServerState.PlayerInfo[i].Ping		 = PRI.Ping;
			i++;
		}
	}

	// Ask the mutators if they have anything to add.
	for (M = BaseMutator.NextMutator; M != None; M = M.NextMutator)
		M.GetServerPlayers(ServerState);
}

//------------------------------------------------------------------------------
// Misc.

// Return the server's port number.
function int GetServerPort()
{
	local string S;
	local int i;

	// Figure out the server's port.
	S = Level.GetAddressURL();
	i = InStr( S, ":" );
	assert(i>=0);
	return int(Mid(S,i+1));
}

function bool SetPause( BOOL bPause, PlayerController P )
{
    if( bPauseable || P.IsA('Admin') || P.PlayerReplicationInfo.bAdmin || Level.Netmode==NM_Standalone )
	{
		if( bPause )
			Level.Pauser=P.PlayerReplicationInfo;
		else
			Level.Pauser=None;
		return True;
	}
	else return False;
}

//------------------------------------------------------------------------------
// Voice chat.
function ChangeVoiceChatter( Controller Client, int IpAddr, int Handle, bool Add )
{
	local int Index;
	local PlayerController P;
		
	if( Add )
	{
		Index = VoiceChatters.length;
		VoiceChatters.Insert(Index,1);
		VoiceChatters[Index].Controller	= Client;
		VoiceChatters[Index].IpAddr		= IpAddr;
		VoiceChatters[Index].Handle		= Handle;
	}
	else
	{
		for( Index=0; Index<VoiceChatters.Length; Index++ )
		{
			if( (VoiceChatters[Index].IpAddr == IpAddr) && (VoiceChatters[Index].Handle == Handle) )
				VoiceChatters.Remove(Index,1);
		}
	}
	
	foreach DynamicActors( class'PlayerController', P )
	{
		if( P != Client )
		{
			P.ClientChangeVoiceChatter( IpAddr, Handle, Add );
		}
	}
}

//------------------------------------------------------------------------------
// Game parameters.

//
// Set gameplay speed.
//
function SetGameSpeed( Float T )
{
	local float OldSpeed;

	OldSpeed = GameSpeed;
	GameSpeed = FMax(T, 0.1);
	Level.TimeDilation = GameSpeed;
	if ( GameSpeed != OldSpeed )
		SaveConfig();
	SetTimer(Level.TimeDilation, true);
}

//
// Called after setting low or high detail mode.
//
event DetailChange()
{
	local actor A;
	local zoneinfo Z;

    if( Level.DetailMode == DM_Low )
	{
		foreach DynamicActors(class'Actor', A)
		{
            if( (A.bHighDetail || A.bSuperHighDetail) && !A.bGameRelevant )
                A.Destroy();
        }
    }
    else if( Level.DetailMode == DM_High )
    {
        foreach DynamicActors(class'Actor', A)
        {
            if( A.bSuperHighDetail && !A.bGameRelevant )
				A.Destroy();
		}
	}
	foreach AllActors(class'ZoneInfo', Z)
		Z.LinkToSkybox();
}

//------------------------------------------------------------------------------
// Player start functions

//
// Grab the next option from a string.
//
function bool GrabOption( out string Options, out string Result )
{
	if( Left(Options,1)=="?" )
	{
		// Get result.
		Result = Mid(Options,1);
		if( InStr(Result,"?")>=0 )
			Result = Left( Result, InStr(Result,"?") );

		// Update options.
		Options = Mid(Options,1);
		if( InStr(Options,"?")>=0 )
			Options = Mid( Options, InStr(Options,"?") );
		else
			Options = "";

		return true;
	}
	else return false;
}

//
// Break up a key=value pair into its key and value.
//
function GetKeyValue( string Pair, out string Key, out string Value )
{
	if( InStr(Pair,"=")>=0 )
	{
		Key   = Left(Pair,InStr(Pair,"="));
		Value = Mid(Pair,InStr(Pair,"=")+1);
	}
	else
	{
		Key   = Pair;
		Value = "";
	}
}

/* ParseOption()
 Find an option in the options string and return it.
*/
function string ParseOption( string Options, string InKey )
{
	local string Pair, Key, Value;
	while( GrabOption( Options, Pair ) )
	{
		GetKeyValue( Pair, Key, Value );
		if( Key ~= InKey )
			return Value;
	}
	return "";
}

/* Initialize the game.
 The GameInfo's InitGame() function is called before any other scripts (including 
 PreBeginPlay() ), and is used by the GameInfo to initialize parameters and spawn 
 its helper classes.
 Warning: this is called before actors' PreBeginPlay.
*/
event InitGame( string Options, out string Error )
{
	local string InOpt, LeftOpt;
	local int pos;
	local class<AccessControl> ACClass;
	local class<GameRules> GRClass;
	local class<BroadcastHandler> BHClass;

	log( "InitGame:" @ Options );

    MaxPlayers = Clamp(GetIntOption( Options, "MaxPlayers", MaxPlayers ),0,32);
    GameDifficulty = FMax(0,GetIntOption(Options, "Difficulty", GameDifficulty));

	InOpt = ParseOption( Options, "GameSpeed");
	if( InOpt != "" )
	{
		log("GameSpeed"@InOpt);
		SetGameSpeed(float(InOpt));
	}

    AddMutator(MutatorClass); 
	
	BHClass = class<BroadcastHandler>(DynamicLoadObject(BroadcastHandlerClass,Class'Class'));
	BroadcastHandler = spawn(BHClass);

	InOpt = ParseOption( Options, "AccessControl");
	if( InOpt != "" )
		ACClass = class<AccessControl>(DynamicLoadObject(InOpt, class'Class'));
    if ( ACClass == None )
	{
		ACClass = class<AccessControl>(DynamicLoadObject(AccessControlClass, class'Class'));
		if (ACClass == None)
			ACClass = class'Engine.AccessControl';
	}

	LeftOpt = ParseOption( Options, "AdminName" );
	InOpt = ParseOption( Options, "AdminPassword");
	if( LeftOpt!="" && InOpt!="" )
		ACClass.default.bDontAddDefaultAdmin = true;

	AccessControl = Spawn(ACClass);
//FIXME_MERGE	if (AccessControl != None && LeftOpt!="" && InOpt!="" )
//        AccessControl.SetAdminFromURL(LeftOpt, InOpt);

	InOpt = ParseOption( Options, "GameRules");
	if ( InOpt != "" )
	{
		log("Game Rules"@InOpt);
		while ( InOpt != "" )
		{
			pos = InStr(InOpt,",");
			if ( pos > 0 )
			{
				LeftOpt = Left(InOpt, pos);
				InOpt = Right(InOpt, Len(InOpt) - pos - 1);
			}
			else
			{
				LeftOpt = InOpt;
				InOpt = "";
			}
			log("Add game rules "$LeftOpt);
			GRClass = class<GameRules>(DynamicLoadObject(LeftOpt, class'Class'));
			if ( GRClass != None )
			{
				if ( GameRulesModifiers == None )
					GameRulesModifiers = Spawn(GRClass);
				else	
					GameRulesModifiers.AddGameRules(Spawn(GRClass));
			}
		}
	}

	log("Base Mutator is "$BaseMutator);
    
	InOpt = ParseOption( Options, "Mutator");
	if ( InOpt != "" )
	{
		log("Mutators"@InOpt);
		while ( InOpt != "" )
		{
			pos = InStr(InOpt,",");
			if ( pos > 0 )
			{
				LeftOpt = Left(InOpt, pos);
				InOpt = Right(InOpt, Len(InOpt) - pos - 1);
			}
			else
			{
				LeftOpt = InOpt;
				InOpt = "";
			}
			log("Add mutator "$LeftOpt);
            AddMutator(LeftOpt, true); 
		}
	}

	InOpt = ParseOption( Options, "GamePassword");
	if( InOpt != "" )
	{
		AccessControl.SetGamePassWord(InOpt);
		log( "GamePassword" @ InOpt );
	}

    InOpt = ParseOption( Options,"AllowThrowing");
    if ( InOpt != "" )
    	bAllowWeaponThrowing = bool (InOpt);

	InOpt = ParseOption(Options, "GameStats");
	if ( InOpt != "")
		bEnableStatLogging = bool(InOpt);
	else
		bEnableStatLogging = false;
	
	log("GameInfo::InitGame : bEnableStatLogging"@bEnableStatLogging);
	 
	
}

function AddMutator(string mutname, optional bool bUserAdded)
{
    local class<Mutator> mutClass;
    local Mutator mut;

    mutClass = class<Mutator>(DynamicLoadObject(mutname, class'Class'));
    if (mutClass == None)
        return;
	
	if ( (mutClass.Default.GroupName != "") && (BaseMutator != None) )
	{
		// make sure no mutators with same groupname
		for ( mut=BaseMutator; mut!=None; mut=mut.NextMutator )
			if ( mut.GroupName == mutClass.Default.GroupName )
				return;
	}
	 
    mut = Spawn(mutClass);
	// mc, beware of mut being none
	if (mut == None)
		return;

	// Meant to verify if this mutator was from Command Line parameters or added from other Actors
	mut.bUserAdded = bUserAdded;

    if (BaseMutator == None)
        BaseMutator = mut;
    else
        BaseMutator.AddMutator(mut);
}

//
// Return beacon text for serverbeacon.
//
event string GetBeaconText()
{	
	return
		Level.ComputerName
    $   " "
    $   Left(Level.Title,24) 
    $   "\\t"
    $   BeaconName
    $   "\\t"
    $   GetNumPlayers()
	$	"/"
	$	MaxPlayers;
}

/* ProcessServerTravel()
 Optional handling of ServerTravel for network games.
*/
function ProcessServerTravel( string URL, bool bItems )
{
	local playercontroller P, LocalPlayer;

	EndLogging("mapchange");

	// Notify clients we're switching level and give them time to receive.
	// We call PreClientTravel directly on any local PlayerPawns (ie listen server)
	log("ProcessServerTravel:"@URL);
	foreach DynamicActors( class'PlayerController', P )
		if( NetConnection( P.Player)!=None )
			P.ClientTravel( URL, TRAVEL_Relative, bItems );
		else
		{	
			LocalPlayer = P;
			P.PreClientTravel();
		}

	if ( (Level.NetMode == NM_ListenServer) && (LocalPlayer != None) )
        Level.NextURL = Level.NextURL
					 $"?Team="$LocalPlayer.GetDefaultURL("Team")
					 $"?Name="$LocalPlayer.GetDefaultURL("Name")
                     $"?Class="$LocalPlayer.GetDefaultURL("Class")
                     $"?Character="$LocalPlayer.GetDefaultURL("Character"); 

	// Switch immediately if not networking.
	if( Level.NetMode!=NM_DedicatedServer && Level.NetMode!=NM_ListenServer )
		Level.NextSwitchCountdown = 0.0;
}

//
// Accept or reject a player on the server.
// Fails login if you set the Error to a non-empty string.
//
event PreLogin
(
	string Options,
	string Address,
	out string Error,
	out string FailCode
)
{
	local bool bSpectator;

    bSpectator = ( ParseOption( Options, "SpectatorOnly" ) ~= "true" );
	AccessControl.PreLogin(Options, Address, Error, FailCode, bSpectator);
}

function int GetIntOption( string Options, string ParseString, int CurrentValue)
{
	local string InOpt;

	InOpt = ParseOption( Options, ParseString );
	if ( InOpt != "" )
	{
		log(ParseString@InOpt);
		return int(InOpt);
	}	
	return CurrentValue;
}

function bool AtCapacity(bool bSpectator)
{
	if ( Level.NetMode == NM_Standalone )
		return false;

	if ( bSpectator )
		return ( (NumSpectators >= MaxSpectators)
			&& ((Level.NetMode != NM_ListenServer) || (NumPlayers > 0)) );
	else
		return ( (MaxPlayers>0) && (NumPlayers>=MaxPlayers) );
}

//
// Log a player in.
// Fails login if you set the Error string.
// PreLogin is called before Login, but significant game time may pass before
// Login is called, especially if content is downloaded.
//
event PlayerController Login
(
	string Portal,
	string Options,
	out string Error
)
{
	local NavigationPoint StartSpot;
	local PlayerController NewPlayer;
	local Pawn      TestPawn;
    local string          InName, InAdminName, InPassword, InChecksum, InClass, InCharacter; 
	local byte            InTeam;
    local bool bSpectator, bAdmin;

    bSpectator = ( ParseOption( Options, "SpectatorOnly" ) ~= "true" );
//FIXME_MERGE	bAdmin = AccessControl.CheckOptionsAdmin(Options);

    // Make sure there is capacity except for admins. (This might have changed since the PreLogin call).
    if ( !bAdmin && AtCapacity(bSpectator) )
	{
		Error=GameMessageClass.Default.MaxedOutMessage;
		return None;
	}

	// If admin, force spectate mode if the server already full of reg. players
	if ( bAdmin && AtCapacity(false))
		bSpectator = true;
	
	BaseMutator.ModifyLogin(Portal, Options);

	// Get URL options.
	InName     = Left(ParseOption ( Options, "Name"), 20);
	InTeam     = GetIntOption( Options, "Team", 255 ); // default to "no team"
    InAdminName= ParseOption ( Options, "AdminName");
	InPassword = ParseOption ( Options, "Password" );
	InChecksum = ParseOption ( Options, "Checksum" );

	log( "Login:" @ InName );
	if( InPassword != "" )
		log( "Password"@InPassword );
	
	// Pick a team (if need teams)
    InTeam = PickTeam(InTeam,None);
		 
	// Find a start spot.
	StartSpot = FindPlayerStart( None, InTeam, Portal );

	if( StartSpot == None )
	{
		Error = GameMessageClass.Default.FailedPlaceMessage;
		return None;
	}

		if ( PlayerControllerClass == None )
//#ifdef	__L2	Hunter
			PlayerControllerClass = class<PlayerController>(DynamicLoadObject("ini:Engine.Engine.DefaultPlayerController", class'Class'));
//#else
			//PlayerControllerClass = class<PlayerController>(DynamicLoadObject(PlayerControllerClassName, class'Class'));
//endif

		NewPlayer = spawn(PlayerControllerClass,,,StartSpot.Location,StartSpot.Rotation);

	// Handle spawn failure.
	if( NewPlayer == None )
	{
		log("Couldn't spawn player controller of class "$PlayerControllerClass);
		Error = GameMessageClass.Default.FailedSpawnMessage;
		return None;
	}

	NewPlayer.StartSpot = StartSpot;

    // Init player's replication info
    NewPlayer.GameReplicationInfo = GameReplicationInfo;
	NewPlayer.GotoState('Spectating');

	// Apply security to this controller
	
	NewPlayer.PlayerSecurity = spawn(SecurityClass,self);
	if (NewPlayer.PlayerSecurity==None)
	{
		log("Could not spawn security for player "$NewPlayer,'Security');
	}
	
	if ( Level.NetMode == NM_Standalone )
		NewPlayer.GotoState('AttractMode');
	else
		NewPlayer.GotoState('Spectating');

	// Init player's name
	if( InName=="" )
		InName=DefaultPlayerName;
	if( Level.NetMode!=NM_Standalone || NewPlayer.PlayerReplicationInfo.PlayerName==DefaultPlayerName )
		ChangeName( NewPlayer, InName, false );

    if ( bSpectator || NewPlayer.PlayerReplicationInfo.bOnlySpectator )
	{
        NewPlayer.PlayerReplicationInfo.bOnlySpectator = true;
		NewPlayer.PlayerReplicationInfo.bIsSpectator = true;
		NumSpectators++;
		return NewPlayer;
	}

	// Change player's team.
    if ( !ChangeTeam(newPlayer, InTeam, false) )
	{
		Error = GameMessageClass.Default.FailedTeamMessage;
		return None;
	}
	newPlayer.StartSpot = StartSpot;
		
    // Init player's administrative privileges and log it
/*FIXME_MERGE    if (AccessControl.AdminLogin(NewPlayer, InAdminName, InPassword))
    {
		AccessControl.AdminEntered(NewPlayer, InAdminName);
    }
*/
	// Set the player's ID.
	NewPlayer.PlayerReplicationInfo.PlayerID = CurrentID++;

	InClass = ParseOption( Options, "Class" );

    if (InClass == "")
        InClass = DefaultPlayerClassName;
    InCharacter = ParseOption(Options, "Character");
    NewPlayer.SetPawnClass(InClass, InCharacter);

	NumPlayers++;
    bWelcomePending = true;

	// if delayed start, don't give a pawn to the player yet
	// Normal for multiplayer games
	if ( bDelayedStart )
	{
		NewPlayer.GotoState('PlayerWaiting');
		return NewPlayer;	
	}

	// Try to match up to existing unoccupied player in level,
	// for savegames and coop level switching.
	ForEach DynamicActors(class'Pawn', TestPawn )
	{
		if ( (TestPawn!=None) && (PlayerController(TestPawn.Controller)!=None) && (PlayerController(TestPawn.Controller).Player==None) && (TestPawn.Health > 0)
			&&  (TestPawn.OwnerName~=InName) )
		{
			NewPlayer.Destroy();
			TestPawn.SetRotation(TestPawn.Controller.Rotation);
			TestPawn.bInitializeAnimation = false; // FIXME - temporary workaround for lack of meshinstance serialization
			TestPawn.PlayWaiting();
			return PlayerController(TestPawn.Controller);
		}
	}
	return newPlayer;
}	

// __L2 kurt 여기 고쳐주세요 L2NetLogin
event PlayerController L2NetLogin
(
	string Portal,
	string Options,
	out string Error
)
{
	local PlayerController NewPlayer;
	local NavigationPoint StartSpot;

//	local class<Pawn> DesiredPawnClass;
	local Pawn      TestPawn;
	local string          InName, InPassword, InChecksum, InClass;
	local byte            InTeam;
	local bool bSpectator;

	bSpectator = ( ParseOption( Options, "SpectatorOnly" ) != "" );

	// Make sure there is capacity. (This might have changed since the PreLogin call).
	if ( AtCapacity(bSpectator) )
	{
		Error=GameMessageClass.Default.MaxedOutMessage;
		return None;
	}

	BaseMutator.ModifyLogin(Portal, Options);

	// Get URL options.
	InName     = Left(ParseOption ( Options, "Name"), 20);
	InTeam     = GetIntOption( Options, "Team", 255 ); // default to "no team"
	InPassword = ParseOption ( Options, "Password" );
	InChecksum = ParseOption ( Options, "Checksum" );
	
	// Pick a team (if need teams)
	InTeam = PickTeam(InTeam, None);
		 
	// Find a start spot.
	StartSpot = FindPlayerStart( None, InTeam, Portal );

	if( StartSpot == None )
	{
		Error = GameMessageClass.Default.FailedPlaceMessage;
		return None;
	}

	// Init player's administrative privileges
	if ( AccessControl.AdminLogin(NewPlayer, InPassword) )
	{
		NewPlayer = spawn(AccessControl.AdminClass,,,StartSpot.Location,StartSpot.Rotation);
		bSpectator = true;
	}
	else
	{
		if ( PlayerControllerClass == None )
//#ifdef	__L2	Hunter
			PlayerControllerClass = class<PlayerController>(DynamicLoadObject("ini:Engine.Engine.DefaultPlayerController", class'Class'));
//#else
			//PlayerControllerClass = class<PlayerController>(DynamicLoadObject(PlayerControllerClassName, class'Class'));
//endif
		NewPlayer = spawn(PlayerControllerClass,,,StartSpot.Location,StartSpot.Rotation);
	}

	// Handle spawn failure.
	if( NewPlayer == None )
	{
		log("Couldn't spawn player controller of class "$PlayerControllerClass);
		Error = GameMessageClass.Default.FailedSpawnMessage;
		return None;
	}

	NewPlayer.StartSpot = StartSpot;

	// Init player's name
	if( InName=="" )
		InName=DefaultPlayerName;
	if( Level.NetMode!=NM_Standalone
		|| ( None != NewPlayer.PlayerReplicationInfo && NewPlayer.PlayerReplicationInfo.PlayerName==DefaultPlayerName ) )
		ChangeName( NewPlayer, InName, false );

	// Init player's replication info
	NewPlayer.GameReplicationInfo = GameReplicationInfo;

	NewPlayer.GotoState('Spectating');

	if ( bSpectator )
	{
		NewPlayer.PlayerReplicationInfo.bOnlySpectator = true;
		NumSpectators++;
		return NewPlayer;
	}

	// Set the player's ID.
	if( None != NewPlayer.PlayerReplicationInfo )
		NewPlayer.PlayerReplicationInfo.PlayerID = CurrentID++;

	InClass = ParseOption( Options, "Class" );
	/*if ( InClass != "" )
	{
		DesiredPawnClass = class<Pawn>(DynamicLoadObject(InClass, class'Class'));
		if ( DesiredPawnClass != None )
			NewPlayer.PawnClass = DesiredPawnClass;
	}*/

	// If we are a server, broadcast a welcome message.
	if( Level.NetMode==NM_DedicatedServer || Level.NetMode==NM_ListenServer )
		BroadcastLocalizedMessage(GameMessageClass, 1, NewPlayer.PlayerReplicationInfo);

	// if delayed start, don't give a pawn to the player yet
	// Normal for multiplayer games
	if ( bDelayedStart )
	{
		NewPlayer.GotoState('PlayerWaiting');
		return NewPlayer;	
	}

	// Try to match up to existing unoccupied player in level,
	// for savegames and coop level switching.
	ForEach DynamicActors(class'Pawn', TestPawn )
	{
		if ( (TestPawn!=None) && (PlayerController(TestPawn.Controller)!=None) && (PlayerController(TestPawn.Controller).Player==None) && (TestPawn.Health > 0)
			&&  (TestPawn.OwnerName~=InName) )
		{
			TestPawn.Controller.Destroy();
			NewPlayer.Possess(TestPawn);
			return NewPlayer;
		}
	}

	// start match, or let player enter, immediately
	bRestartLevel = false;	// let player spawn once in levels that must be restarted after every death
	if ( bWaitingToStartMatch )
		L2NetStartMatch();
	else
		RestartPlayer(newPlayer);

	bRestartLevel = Default.bRestartLevel;
	return newPlayer;
}

/* StartMatch()
Start the game - inform all actors that the match is starting, and spawn player pawns
*/
function StartMatch()
{	
	local Controller P;
	local Actor A; 

	if (GameStats!=None)
		GameStats.StartGame();

	// tell all actors the game is starting
	ForEach AllActors(class'Actor', A)
		A.MatchStarting();

	// start human players first
	for ( P = Level.ControllerList; P!=None; P=P.nextController )
		if ( P.IsA('PlayerController') && (P.Pawn == None) )
		{
            if ( bGameEnded ) 
                return; // telefrag ended the game with ridiculous frag limit
            else if ( PlayerController(P).CanRestartPlayer()  ) 
				RestartPlayer(P);
		}

	// start AI players
	for ( P = Level.ControllerList; P!=None; P=P.nextController )
		if ( P.bIsPlayer && !P.IsA('PlayerController') )
        {
			if ( Level.NetMode == NM_Standalone )
			RestartPlayer(P);
        	else
				P.GotoState('Dead','MPStart');
		}

	bWaitingToStartMatch = false;
	GameReplicationInfo.bMatchHasBegun = true;
}

// __L2 Hunter 
function L2NetStartMatch()
{	
	local Controller P;
	local Actor A; 

	if (GameStats!=None)
		GameStats.StartGame();

	ForEach AllActors(class'Actor', A)
		A.MatchStarting();

	// start players first
	for ( P = Level.ControllerList; P!=None; P=P.nextController )
		if ( P.IsA('PlayerController') && (P.Pawn == None) )
		{
			if ( bGameEnded ) return; // telefrag ended the game with ridiculous frag limit
		}

	for ( P = Level.ControllerList; P!=None; P=P.nextController )
		if ( P.bIsPlayer && !P.IsA('PlayerController') )
			RestartPlayer(P);

	bWaitingToStartMatch = false;
}
//Hunter End

event SpawnDefaultPlayerPawn( Controller aPlayer )	
{
	local NavigationPoint startSpot;
	local class<Pawn> DefaultPlayerClass;

	startSpot = FindPlayerStart(aPlayer, 255);
	if( startSpot == None )
	{
		log(" Player start not found!!!");
		return;
	}	
	
	if ( aPlayer.PawnClass != None )
		aPlayer.Pawn = Spawn(aPlayer.PawnClass,,,StartSpot.Location,StartSpot.Rotation);

	if( aPlayer.Pawn==None )
	{
        DefaultPlayerClass = GetDefaultPlayerClass(aPlayer);
		aPlayer.Pawn = Spawn(DefaultPlayerClass,,,StartSpot.Location,StartSpot.Rotation);
	}
	if ( aPlayer.Pawn == None )
	{
		log("Couldn't spawn player of type "$aPlayer.PawnClass$" at "$StartSpot);
		aPlayer.GotoState('Dead');
		return;
	}
	aPlayer.Possess(aPlayer.Pawn);
	aPlayer.PawnClass = aPlayer.Pawn.Class;

    aPlayer.Pawn.PlayTeleportEffect(true, true);
	aPlayer.ClientSetRotation(aPlayer.Pawn.Rotation);
	AddDefaultInventory(aPlayer.Pawn);
}

// __L2 Hunter
event SpawnPlayerPawn( Controller aPlayer, string PawnClassName, vector StartLocation, rotator StartRotation )	
{
//	local bool foundStart;
	local int TeamNum;
	local class<Pawn> DefaultPlayerClass;

	//if( bRestartLevel && Level.NetMode!=NM_DedicatedServer && Level.NetMode!=NM_ListenServer )
	//	return;

	if ( (aPlayer.PlayerReplicationInfo == None) || (aPlayer.PlayerReplicationInfo.Team == None) )
		TeamNum = 255;
	else
		TeamNum = aPlayer.PlayerReplicationInfo.Team.TeamIndex;

	if( aPlayer.Pawn==None )
	{
		DefaultPlayerClass = class<Pawn>(DynamicLoadObject(PawnClassName, class'Class'));
		aPlayer.Pawn = Spawn(DefaultPlayerClass,,,StartLocation,StartRotation,true);
	}
	if ( aPlayer.Pawn == None )
	{
		log("Couldn't spawn player of type");
		aPlayer.GotoState('Dead');
		return;
	}

	aPlayer.Possess(aPlayer.Pawn);
	aPlayer.PawnClass = aPlayer.Pawn.Class;


	//PlayTeleportEffect(aPlayer, true, true);
	aPlayer.PlayTeleportEffect(true, true);
	aPlayer.ClientSetRotation(aPlayer.Pawn.Rotation);
	AddDefaultInventory(aPlayer.Pawn);
}
//Hunter End

//
// Restart a player.
//
function RestartPlayer( Controller aPlayer )	
{
	local NavigationPoint startSpot;
	local int TeamNum;
	local class<Pawn> DefaultPlayerClass;

	if( bRestartLevel && Level.NetMode!=NM_DedicatedServer && Level.NetMode!=NM_ListenServer )
		return;

	if ( (aPlayer.PlayerReplicationInfo == None) || (aPlayer.PlayerReplicationInfo.Team == None) )
		TeamNum = 255;
	else
		TeamNum = aPlayer.PlayerReplicationInfo.Team.TeamIndex;

	startSpot = FindPlayerStart(aPlayer, TeamNum);
	if( startSpot == None )
	{
		log(" Player start not found!!!");
		return;
	}	
	
	if (aPlayer.PreviousPawnClass!=None && aPlayer.PawnClass != aPlayer.PreviousPawnClass)
		BaseMutator.PlayerChangedClass(aPlayer);			
			
	if ( aPlayer.PawnClass != None )
		aPlayer.Pawn = Spawn(aPlayer.PawnClass,,,StartSpot.Location,StartSpot.Rotation);

	if( aPlayer.Pawn==None )
	{
        DefaultPlayerClass = GetDefaultPlayerClass(aPlayer);
		aPlayer.Pawn = Spawn(DefaultPlayerClass,,,StartSpot.Location,StartSpot.Rotation);
	}
	if ( aPlayer.Pawn == None )
	{
		log("Couldn't spawn player of type "$aPlayer.PawnClass$" at "$StartSpot);
		aPlayer.GotoState('Dead');
		return;
	}
    aPlayer.Pawn.Anchor = startSpot;
	aPlayer.Pawn.LastStartSpot = PlayerStart(startSpot);
	aPlayer.Pawn.LastStartTime = Level.TimeSeconds;
	aPlayer.PreviousPawnClass = aPlayer.Pawn.Class;

	aPlayer.Possess(aPlayer.Pawn);
	aPlayer.PawnClass = aPlayer.Pawn.Class;

    aPlayer.Pawn.PlayTeleportEffect(true, true);
	aPlayer.ClientSetRotation(aPlayer.Pawn.Rotation);
	AddDefaultInventory(aPlayer.Pawn);
	TriggerEvent( StartSpot.Event, StartSpot, aPlayer.Pawn);
}

function class<Pawn> GetDefaultPlayerClass(Controller C)
{
    local PlayerController PC;
    local String PawnClassName;
    local class<Pawn> PawnClass;

    PC = PlayerController( C );

    if( PC != None )
{
        PawnClassName = PC.GetDefaultURL( "Class" );
        PawnClass = class<Pawn>( DynamicLoadObject( PawnClassName, class'Class') );

        if( PawnClass != None )
            return( PawnClass );
}

    return( class<Pawn>( DynamicLoadObject( DefaultPlayerClassName, class'Class' ) ) );
}

//
// Called after a successful login. This is the first place
// it is safe to call replicated functions on the PlayerPawn.
//
event PostLogin( PlayerController NewPlayer )
{
    local class<HUD> HudClass;
    local class<Scoreboard> ScoreboardClass;
    local String SongName;

    // Log player's login.
	if (GameStats!=None)
	{
		GameStats.ConnectEvent(NewPlayer.PlayerReplicationInfo);
		GameStats.GameEvent("NameChange",NewPlayer.PlayerReplicationInfo.playername,NewPlayer.PlayerReplicationInfo);		
	}

	if ( !bDelayedStart )
	{
		// start match, or let player enter, immediately
		bRestartLevel = false;	// let player spawn once in levels that must be restarted after every death
		if ( bWaitingToStartMatch )
			StartMatch();
		else
			RestartPlayer(newPlayer);
		bRestartLevel = Default.bRestartLevel;
	}

	// Start player's music.
    SongName = Level.Song;
    if( SongName != "" && SongName != "None" )
        NewPlayer.ClientSetMusic( SongName, MTRAN_Fade );
	
	// tell client what hud and scoreboard to use

    if( HUDType != "" )
        HudClass = class<HUD>(DynamicLoadObject(HUDType, class'Class'));

    if( ScoreBoardType != "" )
        ScoreboardClass = class<Scoreboard>(DynamicLoadObject(ScoreBoardType, class'Class'));
    NewPlayer.ClientSetHUD( HudClass, ScoreboardClass );

	if ( NewPlayer.Pawn != None )
		NewPlayer.Pawn.ClientSetRotation(NewPlayer.Pawn.Rotation);
}

event L2NetPostLogin( PlayerController NewPlayer )
{
	if ( !bDelayedStart )
	{
		// start match, or let player enter, immediately
		bRestartLevel = false;	// let player spawn once in levels that must be restarted after every death
		if ( bWaitingToStartMatch )
			L2NetStartMatch();
		bRestartLevel = Default.bRestartLevel;
	}

	// Start player's music.
	NewPlayer.ClientSetMusic( Level.Song, MTRAN_Fade );
	
	if ( NewPlayer.Pawn != None )
		NewPlayer.Pawn.ClientSetRotation(NewPlayer.Pawn.Rotation);
}

//
// Player exits.
//
function Logout( Controller Exiting )
{
	local bool bMessage;
	local int Index;

	bMessage = true;
	if ( PlayerController(Exiting) != None )
	{
//FIXME_MERGE		if ( AccessControl.AdminLogout( PlayerController(Exiting) ) )
//			AccessControl.AdminExited( PlayerController(Exiting) );

        if ( PlayerController(Exiting).PlayerReplicationInfo.bOnlySpectator )
		{
			bMessage = false;
				NumSpectators--;
		}
		else
        {
			NumPlayers--;
        }
			
		if( Level.NetMode != NM_Standalone )
		{
			for( Index=0; Index<VoiceChatters.Length; Index++ )
			{
				if( VoiceChatters[Index].Controller == Exiting )
				{
					ChangeVoiceChatter( PlayerController(Exiting), VoiceChatters[Index].IpAddr, VoiceChatters[Index].Handle, false );
				}
			}
		}

		if( Level.NetMode == NM_Client )
		{
			PlayerController(Exiting).ClientLeaveVoiceChat();
		}
	}
	if( bMessage && (Level.NetMode==NM_DedicatedServer || Level.NetMode==NM_ListenServer) )
		BroadcastLocalizedMessage(GameMessageClass, 4, Exiting.PlayerReplicationInfo);

	if ( GameStats!=None)
		GameStats.DisconnectEvent(Exiting.PlayerReplicationInfo);
}

//
// Examine the passed player's inventory, and accept or discard each item.
// AcceptInventory needs to gracefully handle the case of some inventory
// being accepted but other inventory not being accepted (such as the default
// weapon).  There are several things that can go wrong: A weapon's
// AmmoType not being accepted but the weapon being accepted -- the weapon
// should be killed off. Or the player's selected inventory item, active
// weapon, etc. not being accepted, leaving the player weaponless or leaving
// the HUD inventory rendering messed up (AcceptInventory should pick another
// applicable weapon/item as current).
//
event AcceptInventory(pawn PlayerPawn)
{
	//default accept all inventory except default weapon (spawned explicitly)
}

function AddGameSpecificInventory(Pawn p)
{
    local Weapon newWeapon;
    local class<Weapon> WeapClass;
    local Inventory Inv;

    // Spawn default weapon.
    WeapClass = BaseMutator.GetDefaultWeapon();
    if( (WeapClass!=None) && (p.FindInventoryType(WeapClass)==None) )
    {
        newWeapon = Spawn(WeapClass,,,p.Location);
        if( newWeapon != None )
        {
            Inv = None;
            // search pawn's inventory for a bCanThrowWeapon==false, if we find one, don't call Bringup
            for( Inv=Inventory; Inv!=None; Inv=Inv.Inventory )
            {
                if ( Inv.IsA('Weapon') && Weapon(Inv).bCanThrow==false )
                    break;
            }
            newWeapon.GiveTo(p);
            newWeapon.bCanThrow = false; // don't allow default weapon to be thrown out
        }
    }
}

//
// Spawn any default inventory for the player.
//
function AddDefaultInventory( pawn PlayerPawn )
{
	local Weapon newWeapon;
	local class<Weapon> WeapClass;

	// Spawn default weapon.
	WeapClass = BaseMutator.GetDefaultWeapon();
	if( (WeapClass!=None) && (PlayerPawn.FindInventoryType(WeapClass)==None) )
	{
		newWeapon = Spawn(WeapClass,,,PlayerPawn.Location);
		if( newWeapon != None )
		{
			newWeapon.GiveTo(PlayerPawn);
            //newWeapon.BringUp();
			newWeapon.bCanThrow = false; // don't allow default weapon to be thrown out
		}
	}
	SetPlayerDefaults(PlayerPawn);
}

/* SetPlayerDefaults()
 first make sure pawn properties are back to default, then give mutators an opportunity
 to modify them
*/
function SetPlayerDefaults(Pawn PlayerPawn)
{
	PlayerPawn.AirControl = PlayerPawn.Default.AirControl;
    PlayerPawn.GroundSpeed = PlayerPawn.Default.GroundSpeed;
    PlayerPawn.WaterSpeed = PlayerPawn.Default.WaterSpeed;
    PlayerPawn.AirSpeed = PlayerPawn.Default.AirSpeed;
    PlayerPawn.Acceleration = PlayerPawn.Default.Acceleration;
    PlayerPawn.JumpZ = PlayerPawn.Default.JumpZ;
	BaseMutator.ModifyPlayer(PlayerPawn);
}

function NotifyKilled(Controller Killer, Controller Killed, Pawn KilledPawn )
{
}

function Killed( Controller Killer, Controller Killed, Pawn KilledPawn, class<DamageType> damageType )
{
    if ( (Killed != None) && Killed.bIsPlayer )
	{
		Killed.PlayerReplicationInfo.Deaths += 1;
		BroadcastDeathMessage(Killer, Killed, damageType);

		if (GameStats!=None)
		{
			if ( (Killer == Killed) || (Killer == None) )
			{
				if ( Killer == None )
					GameStats.KillEvent("K", None, Killed.PlayerReplicationInfo, DamageType);	//"Kill"
			else
					GameStats.KillEvent("K", Killer.PlayerReplicationInfo, Killed.PlayerReplicationInfo, DamageType);	//"Kill"
			}
			else
			{
				if ( bTeamGame && Killer.PlayerReplicationInfo.Team == Killed.PlayerReplicationInfo.Team )
					GameStats.KillEvent("TK", Killer.PlayerReplicationInfo, Killed.PlayerReplicationInfo, DamageType);	//"Teamkill"
				else
					GameStats.KillEvent("K", Killer.PlayerReplicationInfo, Killed.PlayerReplicationInfo, DamageType);	//"Kill"
			}
		}
	}
    if ( Killed != None )
	ScoreKill(Killer, Killed);
	DiscardInventory(KilledPawn);
    NotifyKilled(Killer,Killed,KilledPawn);
}

function bool PreventDeath(Pawn Killed, Controller Killer, class<DamageType> damageType, vector HitLocation)
{
	if ( GameRulesModifiers == None )
		return false;
	return GameRulesModifiers.PreventDeath(Killed,Killer, damageType,HitLocation);
}

function BroadcastDeathMessage(Controller Killer, Controller Other, class<DamageType> damageType)
{
	if ( (Killer == Other) || (Killer == None) )
        BroadcastLocalized(self,DeathMessageClass, 1, None, Other.PlayerReplicationInfo, damageType);
	else 
        BroadcastLocalized(self,DeathMessageClass, 0, Killer.PlayerReplicationInfo, Other.PlayerReplicationInfo, damageType);
}


// %k = Owner's PlayerName (Killer)
// %o = Other's PlayerName (Victim)
// %w = Owner's Weapon ItemName
static native function string ParseKillMessage( string KillerName, string VictimName, string DeathMessage );

function Kick( string S )
{
	AccessControl.Kick(S);
}

function KickBan( string S )
{
	AccessControl.KickBan(S);
}

function bool IsOnTeam(Controller Other, int TeamNum)
{
    if ( bTeamGame && (Other != None) && Other.bIsPlayer
		&& (Other.PlayerReplicationInfo.Team != None)
		&& (Other.PlayerReplicationInfo.Team.TeamIndex == TeamNum) )
		return true;
	return false;
}

//-------------------------------------------------------------------------------------
// Level gameplay modification.

//
// Return whether Viewer is allowed to spectate from the
// point of view of ViewTarget.
//
function bool CanSpectate( PlayerController Viewer, bool bOnlySpectator, actor ViewTarget )
{
	return true;
}

/* Use reduce damage for teamplay modifications, etc.
*/
function int ReduceDamage( int Damage, pawn injured, pawn instigatedBy, vector HitLocation, vector Momentum, class<DamageType> DamageType )
{
	// 2006/07/27 Removed by NeverDie
	/*
	local int OriginalDamage;
	local armor FirstArmor;

	OriginalDamage = Damage;

	if( injured.PhysicsVolume.bNeutralZone )
		Damage = 0;
	else if ( injured.InGodMode() ) // God mode
		return 0;
	else if ( (injured.Inventory != None) && (damage > 0) ) //then check if carrying armor
	{
		FirstArmor = injured.inventory.PrioritizeArmor(Damage, DamageType, HitLocation);
		while( (FirstArmor != None) && (Damage > 0) )
		{
			Damage = FirstArmor.ArmorAbsorbDamage(Damage, DamageType, HitLocation);
			FirstArmor = FirstArmor.nextArmor;
		} 
	}

	if ( GameRulesModifiers != None )
		return GameRulesModifiers.NetDamage( OriginalDamage, Damage,injured,instigatedBy,HitLocation,Momentum,DamageType );

	return Damage;
	*/
	return 0;
}

//
// Return whether an item should respawn.
//
function bool ShouldRespawn( Pickup Other )
{
	if( Level.NetMode == NM_StandAlone )
		return false;

	return Other.ReSpawnTime!=0.0;
}

/* Called when pawn has a chance to pick Item up (i.e. when 
   the pawn touches a weapon pickup). Should return true if 
   he wants to pick it up, false if he does not want it.
*/
function bool PickupQuery( Pawn Other, Pickup item )
{
	local byte bAllowPickup;

	if ( (GameRulesModifiers != None) && GameRulesModifiers.OverridePickupQuery(Other, item, bAllowPickup) )
		return (bAllowPickup == 1);

	if ( Other.Inventory == None )
		return true;
	else
		return !Other.Inventory.HandlePickupQuery(Item);
}
		
/* Discard a player's inventory after he dies.
*/
function DiscardInventory( Pawn Other )
{
	Other.Weapon = None;
	Other.SelectedItem = None;
    while ( Other.Inventory != None )
        Other.Inventory.Destroy();
}

/* Try to change a player's name.
*/	
function ChangeName( Controller Other, coerce string S, bool bNameChange )
{
	local Controller C;
	
	if( S == "" )
		return;
		
	Other.PlayerReplicationInfo.SetPlayerName(S);
    // notify local players
    if ( bNameChange )
		for ( C=Level.ControllerList; C!=None; C=C.NextController )
			if ( (PlayerController(C) != None) && (Viewport(PlayerController(C).Player) != None) )
				PlayerController(C).ReceiveLocalizedMessage( class'GameMessage', 2, Other.PlayerReplicationInfo );          
}

/* Return whether a team change is allowed.
*/
function bool ChangeTeam(Controller Other, int N, bool bNewTeam)
{
	return true;
}

/* Return a picked team number if none was specified
*/
function byte PickTeam(byte Current, Controller C)
{
	return Current;
}

/* Send a player to a URL.
*/
function SendPlayer( PlayerController aPlayer, string URL )
{
	aPlayer.ClientTravel( URL, TRAVEL_Relative, true );
}

/* Restart the game.
*/
function RestartGame()
{
	local string NextMap;
    local MapList MyList;

	if ( (GameRulesModifiers != None) && GameRulesModifiers.HandleRestartGame() )
		return;

	// these server travels should all be relative to the current URL
	if ( bChangeLevels && !bAlreadyChanged && (MapListType != "") )
	{
		// open a the nextmap actor for this game type and get the next map
		bAlreadyChanged = true;
        MyList = GetMapList(MapListType);
		if (MyList != None)
		{
			NextMap = MyList.GetNextMap();
			MyList.Destroy();
		}
		if ( NextMap == "" )
			NextMap = GetMapName(MapPrefix, NextMap,1);

		if ( NextMap != "" )
		{
			Level.ServerTravel(NextMap, false);
			return;
		}
	}

	Level.ServerTravel( "?Restart", false );
}

function MapList GetMapList(string MapListType)
{
local class<MapList> MapListClass;

	if (MapListType != "")
	{
        MapListClass = class<MapList>(DynamicLoadObject(MapListType, class'Class'));
		if (MapListClass != None)
			return Spawn(MapListClass);
	}
	return None;
}

//==========================================================================
// Message broadcasting functions (handled by the BroadCastHandler)

event Broadcast( Actor Sender, coerce string Msg, optional name Type )
{
	BroadcastHandler.Broadcast(Sender,Msg,Type);
}

function BroadcastTeam( Controller Sender, coerce string Msg, optional name Type )
{
	BroadcastHandler.BroadcastTeam(Sender,Msg,Type);
}

/*
 Broadcast a localized message to all players.
 Most message deal with 0 to 2 related PRIs.
 The LocalMessage class defines how the PRI's and optional actor are used.
*/
event BroadcastLocalized( actor Sender, class<LocalMessage> Message, optional int Switch, optional PlayerReplicationInfo RelatedPRI_1, optional PlayerReplicationInfo RelatedPRI_2, optional Object OptionalObject )
{
	BroadcastHandler.AllowBroadcastLocalized(Sender,Message,Switch,RelatedPRI_1,RelatedPRI_2,OptionalObject);
}

//==========================================================================
	
function bool CheckEndGame(PlayerReplicationInfo Winner, string Reason)
{
	local Controller P;

	if ( (GameRulesModifiers != None) && !GameRulesModifiers.CheckEndGame(Winner, Reason) )
		return false;

	// all player cameras focus on winner or final scene (picked by gamerules)
	for ( P=Level.ControllerList; P!=None; P=P.NextController )
	{
		P.ClientGameEnded();
        P.GameHasEnded();
	}	
	return true;
}

/* End of game.
*/
function EndGame( PlayerReplicationInfo Winner, string Reason )
{
	// don't end game if not really ready
	if ( !CheckEndGame(Winner, Reason) )
	{
		bOverTime = true;
		return;
	}

	bGameEnded = true;
	TriggerEvent('EndGame', self, None);
	EndLogging(Reason);
}

function EndLogging(string Reason)
{

	if (GameStats == None)
		return;
		
	GameStats.EndGame(Reason);
	GameStats.Destroy();
	GameStats = None;
}

/* Return the 'best' player start for this player to start from.
 */
function NavigationPoint FindPlayerStart( Controller Player, optional byte InTeam, optional string incomingName )
{
	local NavigationPoint N, BestStart;
	local Teleporter Tel;
	local float BestRating, NewRating;
	local byte Team;

	// always pick StartSpot at start of match
    if ( (Player != None) && (Player.StartSpot != None) && (Level.NetMode == NM_Standalone)
		&& (bWaitingToStartMatch || ((Player.PlayerReplicationInfo != None) && Player.PlayerReplicationInfo.bWaitingPlayer))  )
	{
		return Player.StartSpot;
	}	

	if ( GameRulesModifiers != None )
	{
		N = GameRulesModifiers.FindPlayerStart(Player,InTeam,incomingName);
		if ( N != None )
		    return N;
	}

	// if incoming start is specified, then just use it
	if( incomingName!="" )
		foreach AllActors( class 'Teleporter', Tel )
			if( string(Tel.Tag)~=incomingName )
				return Tel;

	// use InTeam if player doesn't have a team yet
	if ( (Player != None) && (Player.PlayerReplicationInfo != None) )
	{
		if ( Player.PlayerReplicationInfo.Team != None )
			Team = Player.PlayerReplicationInfo.Team.TeamIndex;
		else
			Team = 0;
	}
	else
		Team = InTeam;

	for ( N=Level.NavigationPointList; N!=None; N=N.NextNavigationPoint )
	{
		NewRating = RatePlayerStart(N,InTeam,Player);
		if ( NewRating > BestRating )
		{
			BestRating = NewRating;
			BestStart = N;
		}
	}
	
	if ( BestStart == None )
	{
		log("Warning - PATHS NOT DEFINED or NO PLAYERSTART");			
		foreach AllActors( class 'NavigationPoint', N )
		{
			NewRating = RatePlayerStart(N,0,Player);
			if ( NewRating > BestRating )
			{
				BestRating = NewRating;
				BestStart = N;	
			}
		}
	}

	return BestStart;
}

/* Rate whether player should choose this NavigationPoint as its start
default implementation is for single player game
*/
function float RatePlayerStart(NavigationPoint N, byte Team, Controller Player)
{
	local PlayerStart P;

	P = PlayerStart(N);
	if ( P != None )
	{
		if ( P.bSinglePlayerStart )
		{
			if ( P.bEnabled )
				return 1000;
			return 20;
		}
		return 10;
	}
	return 0;
}

function ScoreObjective(PlayerReplicationInfo Scorer, Int Score)
{
	if ( Scorer != None )
	{
		Scorer.Score += Score;
        /*
		if ( Scorer.Team != None )
			Scorer.Team.Score += Score;
        */
	}
	if ( GameRulesModifiers != None )
		GameRulesModifiers.ScoreObjective(Scorer,Score);

	CheckScore(Scorer);
}

/* CheckScore()
see if this score means the game ends
*/
function CheckScore(PlayerReplicationInfo Scorer)
{
	if ( (GameRulesModifiers != None) && GameRulesModifiers.CheckScore(Scorer) )
		return;
}
	
function ScoreKill(Controller Killer, Controller Other)
{
	if( (killer == Other) || (killer == None) )
	{
		Other.PlayerReplicationInfo.Score -= 1;
		if (GameStats!=None)
			GameStats.ScoreEvent(Other.PlayerReplicationInfo,-1,"self_frag");
	}
	else if ( killer.PlayerReplicationInfo != None )
	{
		Killer.PlayerReplicationInfo.Score += 1;
		Killer.PlayerReplicationInfo.Kills++;
		if (GameStats!=None)
			GameStats.ScoreEvent(Killer.PlayerReplicationInfo,1,"frag");
	}

	if ( GameRulesModifiers != None )
		GameRulesModifiers.ScoreKill(Killer, Other);

    if ( (Killer != None) || (MaxLives > 0) )
	CheckScore(Killer.PlayerReplicationInfo);
}

function bool TooManyBots() 
{
	return false;
}

static function string FindTeamDesignation(GameReplicationInfo GRI, actor A)	// Should be subclassed in various team games
{
	return "";
}

// - Parse out % vars for various messages

static function string ParseMessageString(Mutator BaseMutator, Controller Who, String Message)
{
	return Message;
}

function ReviewJumpSpots(name TestLabel);

function TeamInfo OtherTeam(TeamInfo Requester)
{
	return None;
}

exec function KillBots(int num);

exec function AdminSay(string Msg)
{
	local controller C;

	for( C=Level.ControllerList; C!=None; C=C.nextController )
		if( C.IsA('PlayerController') )
		{
			PlayerController(C).ClearProgressMessages();
			PlayerController(C).SetProgressTime(6);
			PlayerController(C).SetProgressMessage(0, Msg, class'Canvas'.Static.MakeColor(255,255,255));
		}
}

defaultproperties
{
     bRestartLevel=True
     bPauseable=True
     bCanChangeSkin=True
     bCanViewOthers=True
     bDelayedStart=True
     bChangeLevels=True
     bAllowWeaponThrowing=True
     GameDifficulty=1.000000
     AutoAim=0.930000
     GameSpeed=1.000000
     HUDType="Engine.HUD"
     MaxSpectators=2
     MaxPlayers=16
     DefaultPlayerName="Player"
     GameName="Game"
     FearCostFallOff=0.950000
     DeathMessageClass=Class'Engine.LocalMessage'
     GameMessageClass=Class'Engine.GameMessage'
     MutatorClass="Engine.Mutator"
     AccessControlClass="Engine.AccessControl"
     BroadcastHandlerClass="Engine.BroadcastHandler"
     PlayerControllerClassName="Engine.PlayerController"
     GameReplicationInfoClass=Class'Engine.GameReplicationInfo'
     GameStatsClass=Class'Engine.GameStats'
     SecurityClass=Class'Engine.Security'
     Acronym="???"
}
