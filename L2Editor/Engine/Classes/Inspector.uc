// ====================================================================
//  Class:  Engine.Inspector
//  Parent: Engine.Info
// ====================================================================

class Inspector extends Actor
		Native;

native function coords InspectorFunc001(mesh InMesh, int BoneIndex);
native function int InspectorFunc002(mesh InMesh);
native function int InspectorFunc003(mesh InMesh);
native function int InspectorFunc004(mesh InMesh);
native function int InspectorFunc005(mesh InMesh, int AnimSeqIndex);
native function InspectorFunc006(mesh InMesh, int AnimSeqIndex, int Frame);

event int Inspection(int param1, int param2, int param3, int param4);

defaultproperties
{
}
