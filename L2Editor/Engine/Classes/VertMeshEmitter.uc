//=============================================================================
// Emitter: An Unreal VertMesh Particle Emitter.
//=============================================================================
class VertMeshEmitter extends ParticleEmitter	
	noexport
	native;

var	transient array<int>							VertexStreams;
var	transient array<int>							IndexBuffer;
var transient array<float>							AnimFrame;	

var (VertMesh) array<float>				AnimRate; //���߿� framerate�� ���� �����Ҷ��...
var (VertMesh) array<float>				StartAnimFrame;
var	(VertMesh) VertMesh					VertexMesh;

var (VertMesh)		bool				UseMeshBlendMode;
var (VertMesh)		bool				RenderTwoSided;
var (VertMesh)		bool				UseParticleColor;

var	transient		vector				MeshExtent;

defaultproperties
{
     UseMeshBlendMode=True
     RenderTwoSided=True
     StartSizeRange=(X=(Min=1.000000,Max=1.000000),Y=(Min=1.000000,Max=1.000000),Z=(Min=1.000000,Max=1.000000))
}
