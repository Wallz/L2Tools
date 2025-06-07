/*=============================================================================
	UnCanvas.h: Unreal canvas definition.
	Copyright 2001 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Andrew Scheidecker
=============================================================================*/

class FCanvasUtil; // sjs

/*
	UCanvas
	A high-level rendering interface used to render objects on the HUD.
*/

class ENGINE_API UCanvas : public UObject
{
	DECLARE_CLASS(UCanvas,UObject,CLASS_Transient,Engine);
	NO_DEFAULT_CONSTRUCTOR(UCanvas);
public:

	// Variables.
	UFont*			Font;
    //FLOAT           FontScaleX, FontScaleY; // gam
	FLOAT			SpaceX, SpaceY;
	FLOAT			OrgX, OrgY;
	FLOAT			ClipX, ClipY;
	FLOAT			CurX, CurY;
	FLOAT			Z;
	BYTE			Style;
	FLOAT			CurYL;
	FColor			Color;
	BITFIELD		bCenter:1;
	BITFIELD		bNoSmooth:1;
	INT				SizeX, SizeY;
    //FPlane          ColorModulate;  // sjs - modulate all colors by this before rendering
    //BITFIELD        bRenderLevel;   // gam - Will render the level if enabled.
	UFont			*TinyFont, *SmallFont, *MedFont;
	FStringNoInit	TinyFontName, SmallFontName, MedFontName;
	UViewport*		Viewport;
    FCanvasUtil*    pCanvasUtil; // sjs - shared canvas util for batching successive canvas rendering funcs

	class UTexture* pNormalLeftTex;
	class UTexture* pNormalRightTex;
	class UTexture* pTargetLeftTex;
	class UTexture* pTargetRightTex;
	class UTexture* pAttackLeftTex;
	class UTexture* pAttackRightTex;
	class UTexture* p2DQuestMarkTex;
	class UTexture* pBlueTex;
	class UTexture* pBlueCrossTex;
	class UTexture* pBlueShieldTex;
	class UTexture* pBlueSwordTex;
	class UTexture* pRedTex;
	class UTexture* pRedCrossTex;
	class UTexture* pRedShieldTex;
	class UTexture* pRedSwordTex;
	class UTexture* pBlueAttLeaderTex;
	class UTexture* pBlueLeaderTex;
	class UTexture* pRedAttLeaderTex;
	class UTexture* pRedLeaderTex;
	class UTexture* pRadarTex;
	class UTexture* pMyLocTex;
	class UTexture* pPartyLocTex;
	class UTexture* pTargetLocTex;
	class UTexture* pTutorialLocTex1;
	class UTexture* pTutorialLocTex2;
	class UTexture* pRadarNorthTex;
	class UTexture* pRadarSouthTex;
	class UTexture* pRadarEastTex;
	class UTexture* pRadarWestTex;
	BITFIELD m_bForeTutorial : 1 GCC_PACK(4);
	BITFIELD m_bBroadcastObserverMode : 1;
	BITFIELD m_bShowBroadcastObserverTargetName : 1;
	BITFIELD m_IsClipped : 1;
	FLOAT m_OldCurX GCC_PACK(4);
	FLOAT m_OldCurY;
	FLOAT m_OldOrgX;
	FLOAT m_OldOrgY;
	FLOAT m_OldClipX;
	FLOAT m_OldClipY;
	class UFont* m_L2Font[3];
	class UTexture* pChatBack1_1;
	class UTexture* pChatBack1_2;
	class UTexture* pChatBack1_3;
	class UTexture* pChatBack1_4;
	class UTexture* pChatBack2_1;
	class UTexture* pChatBack2_2;
	class UTexture* pChatBack2_3;
	class UTexture* pChatBack2_4;
	class UTexture* m_pBroadcastObserverNumberTexRed[9];
	class UTexture* m_pBroadcastObserverNumberTexBlue[9];
	INT m_pViewportWindowRenderTarget;
	FLOAT m_fRecordMarkTimer;
	class UTexture* m_pReplayRecordTimerTex;
	FLOAT m_FontScaleModifier;
	FLOAT m_fTimePerFont;
	FLOAT m_fStartTime;
	INT m_nTextIndex;
	BITFIELD m_bTextAnim : 1 GCC_PACK(4);
	BITFIELD m_bTextEnd : 1;
	BITFIELD m_bFirstTextAnim : 1;

	// UCanvas interface.
	virtual void Init( UViewport* InViewport );
	virtual void Update();
	virtual void DrawTile( UMaterial* Material, FLOAT X, FLOAT Y, FLOAT XL, FLOAT YL, FLOAT U, FLOAT V, FLOAT UL, FLOAT VL, FLOAT Z, FPlane Color, FPlane Fog );
	virtual void DrawIcon( UMaterial* Material, FLOAT ScreenX, FLOAT ScreenY, FLOAT XSize, FLOAT YSize, FLOAT Z, FPlane Color, FPlane Fog );
	virtual void DrawPattern( UMaterial* Material, FLOAT X, FLOAT Y, FLOAT XL, FLOAT YL, FLOAT Scale, FLOAT OrgX, FLOAT OrgY, FLOAT Z, FPlane Color, FPlane Fog );
	virtual void VARARGS WrappedStrLenf( UFont* Font, INT& XL, INT& YL, const TCHAR* Fmt, ... );
	virtual void VARARGS WrappedStrLenf(UFont* Font, FLOAT ScaleX, FLOAT ScaleY, INT& XL, INT& YL, const TCHAR* Fmt, ...);
	virtual void VARARGS WrappedPrintf(UFont* Font, UBOOL Center, const TCHAR* Fmt, ...);
	virtual void VARARGS WrappedPrintf(UFont* Font, FLOAT ScaleX, FLOAT ScaleY, UBOOL Center, const TCHAR* Fmt, ...);

	virtual void LoadFont(INT, TCHAR*, TCHAR*);
	virtual void DestroyFont(INT);

	virtual void ReleaseClip();
	virtual void DrawStretchedTex(INT, INT, INT, INT, UTexture *, bool, UBOOL);
	virtual void DrawStretchedTex(INT, INT, INT, INT, INT, INT, INT, INT, UTexture *, UBOOL);
	virtual void DrawTexture(INT, INT, INT, INT, FLOAT, FLOAT, FLOAT, FLOAT, UTexture *, UBOOL, bool);
	virtual void DrawLine(INT, INT, INT, INT, DWORD, INT);
	virtual void DrawActor(INT, INT, AActor *, FLOAT);
	virtual void DrawRect(INT, INT, INT, INT, UTexture *, DWORD);
	virtual DWORD DrawHtmlText(INT, INT, DWORD, const TCHAR*, DWORD, DWORD, DWORD, FLOAT, INT, L2FontType, INT, INT, DWORD, INT);
	virtual void DrawBrightenTile(UMaterial *, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FPlane, FPlane);
	virtual void DrawBrightenTexture(INT, INT, INT, INT, FLOAT, FLOAT, FLOAT, FLOAT, UTexture *, DWORD);
	virtual DWORD DrawTextToCanvas(INT, INT, DWORD, const TCHAR*, DWORD, DWORD, DWORD, FLOAT, INT, L2FontType, INT, INT, DWORD, INT, INT, INT, TArray<FL2ColorFontInfo *> *, unsigned short, EFontExceptionType, INT);
	virtual DWORD DrawThaiText(INT, INT, DWORD, TArray<ThaiCharacter>, DWORD, DWORD, DWORD, FLOAT, INT, L2FontType, INT, INT, DWORD, INT, INT, INT, TArray<FL2ColorFontInfo *> *, unsigned short, EFontExceptionType);
	virtual DWORD DrawNormalText(INT, INT, DWORD, const TCHAR*, DWORD, DWORD, DWORD, FLOAT, INT, L2FontType, INT, INT, DWORD, INT, INT, INT, TArray<FL2ColorFontInfo *> *, unsigned short, EFontExceptionType, INT);
	virtual void Draw3DCoordText(FVector, DWORD, TCHAR*, UTexture *, INT, INT, L2FontType, INT);
	virtual void Draw3DCoordText(FLevelSceneNode *, FRenderInterface *, FVector, DWORD, TCHAR*, UTexture *, INT, INT, L2FontType, INT);
	virtual DWORD GetTextExtent(const TCHAR*, size_t*, DWORD, DWORD, INT, L2FontType, INT, DWORD);
	virtual DWORD GetTextSize(const TCHAR*, size_t*, DWORD, DWORD, INT, L2FontType, INT, DWORD);
	virtual DWORD GetThaiTextSize(TArray<ThaiCharacter>, size_t*, DWORD, DWORD, INT, L2FontType, INT, DWORD);
	virtual void DrawTargetName(FLevelSceneNode *, FRenderInterface *, FVector, DWORD, User *, TargetRenderType, L2FontType, DWORD);
	virtual void DrawTargetOptionName(FLevelSceneNode *, FRenderInterface *, FVector, DWORD, User *, TargetRenderType, L2FontType);
	virtual void DrawTargetTex(FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, UTexture *);
	virtual void DrawDepthBar(FLevelSceneNode *, FRenderInterface *, FVector, DWORD, DWORD, INT, FLOAT, bool);
	virtual DWORD DrawSpecialDigit(INT, INT, INT, INT, DWORD, const TCHAR*, UTexture *, UBOOL);
	virtual void DrawCameraSceneNode();
	virtual void DrawCameraSceneNode(INT, INT, INT, INT, INT);
	virtual UMaterial * GetViewportSceneTexture(INT);

	virtual void WrapStringToArray(const TCHAR* Text, TArray<FString> *OutArray, FLOAT Width, UFont *Font = NULL, const TCHAR EOL = '\n');
	virtual void ClippedStrLen(UFont* Font, FLOAT ScaleX, FLOAT ScaleY, INT& XL, INT& YL, const TCHAR* Text);
	virtual void ClippedPrINT(UFont* Font, FLOAT ScaleX, FLOAT ScaleY, UBOOL Center, const TCHAR* Text);

	void virtual DrawTileStretched(UMaterial* Mat, FLOAT Left, FLOAT Top, FLOAT AWidth, FLOAT AHeight);
	void virtual DrawTileScaled(UMaterial* Mat, FLOAT Left, FLOAT Top, FLOAT NewXScale, FLOAT NewYScale);
	void virtual DrawTileBound(UMaterial* Mat, FLOAT Left, FLOAT Top, FLOAT Width, FLOAT Height);
	void virtual DrawTileJustified(UMaterial* Mat, FLOAT Left, FLOAT Top, FLOAT Width, FLOAT Height, BYTE Justification);
	void virtual DrawTileScaleBound(UMaterial* Mat, FLOAT Left, FLOAT Top, FLOAT Width, FLOAT Height);
	void virtual VARARGS DrawTextJustified(BYTE Justification, FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, const TCHAR* Fmt, ...);

	virtual void SetClip(INT X, INT Y, INT XL, INT YL);

	virtual DWORD DrawTrueFontText(INT, INT, DWORD, const TCHAR*, DWORD, DWORD, DWORD, FLOAT, INT, L2FontType, INT, INT, DWORD);
	virtual DWORD GetTrueFontTextSize(const TCHAR*, size_t*, DWORD, DWORD, INT, L2FontType, INT, DWORD);
	virtual DWORD DrawTrueFontSingleLineText(INT, INT, DWORD, const TCHAR*, FLOAT, L2FontType);
	virtual DWORD DrawTrueFontHtmlText(INT, INT, DWORD, const TCHAR*, DWORD, DWORD, DWORD, INT, L2FontType, INT, INT, DWORD, INT, INT, TArray<FL2ColorFontInfo*>*);
	virtual void DrawChatting(FVector, DWORD, TCHAR*, INT, INT, L2FontType, INT, INT, DWORD);

	virtual void StartBroadcastObserverMode();
	virtual void FinishBroadcastObserverMode();
	virtual void ToggleBroadcastObserverTargetName();

	virtual void SetFontScale(FLOAT);

	virtual void DrawRecordMark();

	// Natives.
	DECLARE_FUNCTION(execDrawActor);
	DECLARE_FUNCTION(execDrawPortal);
	DECLARE_FUNCTION(execDrawText);
	DECLARE_FUNCTION(execDrawTextClipped);
	DECLARE_FUNCTION(execDrawTextJustified);
	DECLARE_FUNCTION(execDrawTile);
	DECLARE_FUNCTION(execDrawTileClipped);
	DECLARE_FUNCTION(execDrawTileJustified);
	DECLARE_FUNCTION(execDrawTileScaled);
	DECLARE_FUNCTION(execDrawTileStretched);
	DECLARE_FUNCTION(execStrLen);
	DECLARE_FUNCTION(execTextSize);
	DECLARE_FUNCTION(execWrapStringToArray);

    void eventReset()
    {
        ProcessEvent(FindFunctionChecked(TEXT("Reset")),NULL);
    }

private:
	// Internal functions.
	void WrappedPrint( ERenderStyle Style, INT& XL, INT& YL, UFont* Font, FLOAT ScaleX, FLOAT ScaleY, UBOOL Center, const TCHAR* Text ); // gam
};
