class ParamStack extends object
	native
	noexport;
	
var int stack; //실제로는 l2paramstack 포인터를 사용하지만 4byte 사이즈를 맞추기 위해 int로...
	
native function string GetString();
native function int	GetInt();
native function float GetFloat();

//2006.6 ttmayrin
native function PushInt(int item);
native function PushString(string item);

defaultproperties
{
}
