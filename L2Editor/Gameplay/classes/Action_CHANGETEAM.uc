class ACTION_ChangeTeam extends ScriptedAction;

var(Action) int Team;

function bool InitActionFor(ScriptedController C)
{

	local PlayerReplicationInfo P;
	
	if (C.PlayerReplicationInfo==None)
	{
		P = c.spawn(class'PlayerReplicationInfo',C,,C.Pawn.Location, C.Pawn.Rotation);
		if (P==None)
		{
			log("#### FUCKED STILL");
			return false;
		}
		
		C.PlayerReplicationInfo = P;
		C.Pawn.PlayerReplicationInfo=P;
		P = None;
	}
	
	C.Level.Game.GameReplicationInfo.Teams[Team].AddToTeam(c);
	C.bIsPlayer=true;
	
	return false;	
}

defaultproperties
{
	ActionString="Change Team"
	Team=0
}