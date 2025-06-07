class ActionMessage extends CriticalEventPlus;

var localized string Messages[32];

static function string GetString(
	optional int Switch,
	optional PlayerReplicationInfo RelatedPRI_1, 
	optional PlayerReplicationInfo RelatedPRI_2,
	optional Object OptionalObject
	)
{
	return Default.Messages[Switch];
}

defaultproperties
{
    PosY=0.85
    FontSize=0
    Lifetime=8
   	DrawColor=(R=255,G=255,B=0,A=255)
}