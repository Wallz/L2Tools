/*=============================================================================
	BuildPropSheet : Property sheet for map rebuilding.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Warren Marshall

    Work-in-progress todo's:

=============================================================================*/

#include "UnrealEd.h"


// --------------------------------------------------------------
//
// WPageOptions
//
// --------------------------------------------------------------

// Structors.
WPageOptions::WPageOptions ( WWindow* InOwnerWindow )
:	WPropertyPage( InOwnerWindow )
{
	GeomCheck = BuildOnlyVisibleCheck = BSPCheck
		= LightingCheck
		= DefinePathsCheck = OptimalButton =
		DitherLightmapsCheck = ReportErrorsCheck = SaveLightmapsCheck = NULL;
	GeomBox = BSPBox = OptimizationBox = LightingBox = DefinePathsBox = NULL;
	BuildPathsButton = PathsChangedButton = BuildButton = LameButton = GoodButton = SaveButton = DeleteButton = NULL;
	BalanceBar = PortalBiasBar = NULL;
	BalanceLabel = PortalBiasLabel = NULL;
	ConfigCombo = LightmapCompressionCombo = NULL;
}

void WPageOptions::OpenWindow( INT InDlgId, HMODULE InHMOD )
{
	guard(WPageOptions::OpenWindow);
	WPropertyPage::OpenWindow( InDlgId, InHMOD );

	// Create child controls and let the base class determine their proper positions.
	GeomBox = new WGroupBox( this, IDGP_GEOM );
	GeomBox->OpenWindow( 1, 0 );
	BSPBox = new WGroupBox( this, IDGP_BSP );
	BSPBox->OpenWindow( 1, 0 );
	OptimizationBox = new WGroupBox( this, IDGP_OPTIMIZATION );
	OptimizationBox->OpenWindow( 1, 0 );
	LightingBox = new WGroupBox( this, IDGP_LIGHTING );
	LightingBox->OpenWindow( 1, 0 );
	DefinePathsBox = new WGroupBox( this, IDGP_PATHS );
	DefinePathsBox->OpenWindow( 1, 0 );
	GeomCheck = new WCheckBox( this, IDCK_GEOMETRY );
	GeomCheck->OpenWindow( 1, 0, 0, 0, 0, TEXT("") );
	BuildOnlyVisibleCheck = new WCheckBox( this, IDCK_ONLY_VISIBLE );
	BuildOnlyVisibleCheck->OpenWindow( 1, 0, 0, 0, 0, TEXT("") );
	BSPCheck = new WCheckBox( this, IDCK_BSP );
	BSPCheck->OpenWindow( 1, 0, 0, 0, 0, TEXT("") );
	LightingCheck = new WCheckBox( this, IDCK_LIGHTING );
	LightingCheck->OpenWindow( 1, 0, 0, 0, 0, TEXT("") );
	DefinePathsCheck = new WCheckBox( this, IDCK_PATH_DEFINE );
	DefinePathsCheck->OpenWindow( 1, 0, 0, 0, 0, TEXT("") );
	OptimalButton = new WCheckBox( this, IDRB_OPTIMAL );
	OptimalButton->OpenWindow( 1, 0, 0, 0, 0, TEXT("") );
	BuildPathsButton = new WButton( this, IDPB_BUILD_PATHS );
	BuildPathsButton->OpenWindow( 1, 0, 0, 0, 0, TEXT("") );
	PathsChangedButton = new WButton( this, IDPB_PATHS_CHANGED );
	PathsChangedButton->OpenWindow( 1, 0, 0, 0, 0, TEXT("") );
	BuildButton = new WButton( this, IDPB_BUILD );
	BuildButton->OpenWindow( 1, 0, 0, 0, 0, TEXT("") );
	LameButton = new WButton( this, IDRB_LAME );
	LameButton->OpenWindow( 1, 0, 0, 0, 0, TEXT("") );
	GoodButton = new WButton( this, IDRB_GOOD );
	GoodButton->OpenWindow( 1, 0, 0, 0, 0, TEXT("") );
	BalanceBar = new WTrackBar( this, IDSL_BALANCE );
	BalanceBar->OpenWindow( 1, 0 );
	PortalBiasBar = new WTrackBar( this, IDSL_PORTALBIAS );
	PortalBiasBar->OpenWindow( 1, 0 );
	BalanceLabel = new WLabel( this, IDSC_BALANCE );
	BalanceLabel->OpenWindow( 1, 0 );
	PortalBiasLabel = new WLabel( this, IDSC_PORTALBIAS );
	PortalBiasLabel->OpenWindow( 1, 0 );
	ConfigCombo = new WComboBox( this, IDCB_CONFIGS );
	ConfigCombo->OpenWindow( 1, 1, CBS_DROPDOWN );
	LightmapCompressionCombo = new WComboBox( this, IDCB_LIGHTMAP_FORMAT );
	LightmapCompressionCombo->OpenWindow( 1, 1, CBS_DROPDOWNLIST );
	DitherLightmapsCheck = new WCheckBox( this, IDCK_DITHER_LIGHTMAPS );
	DitherLightmapsCheck->OpenWindow( 1, 0, 0, 0, 0, TEXT("") );
	SaveLightmapsCheck = new WCheckBox( this, IDCK_SAVE_LIGHTMAPS );
	SaveLightmapsCheck->OpenWindow( 1, 0, 0, 0, 0, TEXT("") );
	ReportErrorsCheck = new WCheckBox( this, IDCK_REPORT_ERRORS );
	ReportErrorsCheck->OpenWindow( 1, 0, 0, 0, 0, TEXT("") );
	SaveButton = new WButton( this, IDPB_SAVE );
	SaveButton->OpenWindow( 1, 0, 0, 0, 0, TEXT("") );
	DeleteButton = new WButton( this, IDPB_DELETE );
	DeleteButton->OpenWindow( 1, 0, 0, 0, 0, TEXT("") );

	PlaceControl( GeomBox );
	PlaceControl( BSPBox );
	PlaceControl( OptimizationBox );
	PlaceControl( LightingBox );
	PlaceControl( DefinePathsBox );
	PlaceControl( GeomCheck );
	PlaceControl( BuildOnlyVisibleCheck );
	PlaceControl( BSPCheck );
	PlaceControl( LightingCheck );
	PlaceControl( DefinePathsCheck );
	PlaceControl( OptimalButton );
	PlaceControl( BuildPathsButton );
	PlaceControl( PathsChangedButton );
	PlaceControl( BuildButton );
	PlaceControl( LameButton );
	PlaceControl( GoodButton );
	PlaceControl( BalanceBar );
	PlaceControl( PortalBiasBar );
	PlaceControl( BalanceLabel );
	PlaceControl( PortalBiasLabel );
	PlaceControl( ConfigCombo );
	PlaceControl( LightmapCompressionCombo );
	PlaceControl( DitherLightmapsCheck );
	PlaceControl( SaveLightmapsCheck );
	PlaceControl( ReportErrorsCheck );
	PlaceControl( SaveButton );
	PlaceControl( DeleteButton );

	Finalize();

	// Delegates.
	GeomCheck->ClickDelegate = FDelegate(this, (TDelegate)&WPageOptions::OnGeomClick);
	BSPCheck->ClickDelegate = FDelegate(this, (TDelegate)&WPageOptions::OnBSPClick);
	LightingCheck->ClickDelegate = FDelegate(this, (TDelegate)&WPageOptions::OnLightingClick);
	DefinePathsCheck->ClickDelegate = FDelegate(this, (TDelegate)&WPageOptions::OnDefinePathsClick);
	BuildOnlyVisibleCheck->ClickDelegate = FDelegate(this, (TDelegate)&WPageOptions::OnBuildOnlyVisibleClick);
	BuildButton->ClickDelegate = FDelegate(this, (TDelegate)&WPageOptions::OnBuildClick);
	BuildPathsButton->ClickDelegate = FDelegate(this, (TDelegate)&WPageOptions::OnBuildPathsClick);
	PathsChangedButton->ClickDelegate = FDelegate(this, (TDelegate)&WPageOptions::OnPathsChangedClick);
	BalanceBar->ThumbTrackDelegate = FDelegate(this, (TDelegate)&WPageOptions::OnBalanceBarMove);
	BalanceBar->ThumbPositionDelegate = FDelegate(this, (TDelegate)&WPageOptions::OnBalanceBarMove);
	PortalBiasBar->ThumbTrackDelegate = FDelegate(this, (TDelegate)&WPageOptions::OnPortalBiasBarMove);
	PortalBiasBar->ThumbPositionDelegate = FDelegate(this, (TDelegate)&WPageOptions::OnPortalBiasBarMove);
	SaveButton->ClickDelegate = FDelegate(this, (TDelegate)&WPageOptions::OnSaveClick);
	DeleteButton->ClickDelegate = FDelegate(this, (TDelegate)&WPageOptions::OnDeleteClick);
	ConfigCombo->SelectionChangeDelegate = FDelegate(this, (TDelegate)&WPageOptions::OnConfigComboSelChange);
	LightmapCompressionCombo->SelectionChangeDelegate = FDelegate(this, (TDelegate)&WPageOptions::OnLightmapCompressionComboSelChange);
	DitherLightmapsCheck->ClickDelegate = FDelegate(this, (TDelegate)&WPageOptions::OnDitherLightmaps);
	SaveLightmapsCheck->ClickDelegate = FDelegate(this, (TDelegate)&WPageOptions::OnSaveLightmaps);
	LameButton->ClickDelegate = FDelegate(this, (TDelegate)&WPageOptions::OnLameClicked);
	GoodButton->ClickDelegate = FDelegate(this, (TDelegate)&WPageOptions::OnGoodClicked);
	OptimalButton->ClickDelegate = FDelegate(this, (TDelegate)&WPageOptions::OnOptimalClicked);
	ReportErrorsCheck->ClickDelegate = FDelegate(this, (TDelegate)&WPageOptions::OnReportErrors);

	BalanceBar->SetRange( 1, 100 );
	BalanceBar->SetTicFreq( 10 );

	PortalBiasBar->SetRange( 1, 100 );
	PortalBiasBar->SetTicFreq( 5 );

	RefreshConfigCombo();

	LightmapCompressionCombo->AddString( TEXT("DXT1") );
	LightmapCompressionCombo->AddString( TEXT("DXT3") );
	LightmapCompressionCombo->AddString( TEXT("RGB8") );

	// Initialize controls.
	InitFromCurrentConfig();

	unguard;
}
void WPageOptions::OnDestroy()
{
	guard(WPageOptions::OnDestroy);
	WPropertyPage::OnDestroy();

	::DestroyWindow( GeomBox->hWnd );
	::DestroyWindow( BSPBox->hWnd );
	::DestroyWindow( OptimizationBox->hWnd );
	::DestroyWindow( LightingBox->hWnd );
	::DestroyWindow( DefinePathsBox->hWnd );
	::DestroyWindow( GeomCheck->hWnd );
	::DestroyWindow( BuildOnlyVisibleCheck->hWnd );
	::DestroyWindow( BSPCheck->hWnd );
	::DestroyWindow( LightingCheck->hWnd );
	::DestroyWindow( DefinePathsCheck->hWnd );
	::DestroyWindow( BuildPathsButton->hWnd );
	::DestroyWindow( PathsChangedButton->hWnd );
	::DestroyWindow( BuildButton->hWnd );
	::DestroyWindow( LameButton->hWnd );
	::DestroyWindow( GoodButton->hWnd );
	::DestroyWindow( OptimalButton->hWnd );
	::DestroyWindow( BalanceBar->hWnd );
	::DestroyWindow( PortalBiasBar->hWnd );
	::DestroyWindow( BalanceLabel->hWnd );
	::DestroyWindow( PortalBiasLabel->hWnd );
	::DestroyWindow( ConfigCombo->hWnd );
	::DestroyWindow( LightmapCompressionCombo->hWnd );
	::DestroyWindow( DitherLightmapsCheck->hWnd );
	::DestroyWindow( SaveLightmapsCheck->hWnd );
	::DestroyWindow( ReportErrorsCheck->hWnd );
	::DestroyWindow( SaveButton->hWnd );
	::DestroyWindow( DeleteButton->hWnd );

	delete GeomBox;
	delete BSPBox;
	delete OptimizationBox;
	delete LightingBox;
	delete DefinePathsBox;
	delete GeomCheck;
	delete BuildOnlyVisibleCheck;
	delete BSPCheck;
	delete LightingCheck;
	delete DefinePathsCheck;
	delete BuildPathsButton;
	delete PathsChangedButton;
	delete BuildButton;
	delete LameButton;
	delete GoodButton;
	delete OptimalButton;
	delete BalanceBar;
	delete PortalBiasBar;
	delete BalanceLabel;
	delete PortalBiasLabel;
	delete ConfigCombo;
	delete LightmapCompressionCombo;
	delete DitherLightmapsCheck;
	delete SaveLightmapsCheck;
	delete ReportErrorsCheck;
	delete SaveButton;
	delete DeleteButton;

	unguard;
}
void WPageOptions::RefreshConfigCombo()
{
	guard(WPageOptions::RefreshConfigCombo);
	INT Sel = ConfigCombo->GetCurrent();
	ConfigCombo->Empty();
	for( INT x = 0 ; x < GRebuildTools.Options.Num() ; x++ )
		ConfigCombo->AddString( *GRebuildTools.Options(x).GetName() );
	ConfigCombo->SetCurrent( Sel == CB_ERR ? 0 : Sel );
	unguard;
}
void WPageOptions::Refresh()
{
	guard(WPageOptions::Refresh);

	BalanceLabel->SetText( *FString::Printf(TEXT("%d"), BalanceBar->GetPos() ) );
	PortalBiasLabel->SetText( *FString::Printf(TEXT("%d"), PortalBiasBar->GetPos() ) );

	UBOOL Checked;

	Checked = GRebuildTools.GetCurrent()->Options & REBUILD_Geometry;
	EnableWindow( BuildOnlyVisibleCheck->hWnd, Checked );

	Checked = GRebuildTools.GetCurrent()->Options & REBUILD_BSP;
	EnableWindow( OptimizationBox->hWnd, Checked );
	EnableWindow( LameButton->hWnd, Checked );
	EnableWindow( GoodButton->hWnd, Checked );
	EnableWindow( OptimalButton->hWnd, Checked );
	EnableWindow( PortalBiasBar->hWnd, Checked );
	EnableWindow( PortalBiasLabel->hWnd, Checked );
	EnableWindow( BalanceBar->hWnd, Checked );
	EnableWindow( BalanceLabel->hWnd, Checked );
	EnableWindow( GetDlgItem( hWnd, IDSC_BSP_1 ), Checked );
	EnableWindow( GetDlgItem( hWnd, IDSC_BSP_2 ), Checked );
	EnableWindow( GetDlgItem( hWnd, IDSC_BSP_3 ), Checked );
	EnableWindow( GetDlgItem( hWnd, IDSC_BSP_4 ), Checked );

	Checked = GRebuildTools.GetCurrent()->Options & REBUILD_Lighting;
	EnableWindow( LightmapCompressionCombo->hWnd, Checked );
	EnableWindow( GetDlgItem( hWnd, IDSC_LIGHTMAP_FORMAT ), Checked );

	Checked = GRebuildTools.GetCurrent()->Options & REBUILD_Paths;
	EnableWindow( PathsChangedButton->hWnd, Checked );

	InitFromCurrentConfig();

	unguard;
}

// Updates the controls with the values in the current config
void WPageOptions::InitFromCurrentConfig()
{
	guard(WPageOptions::InitFromCurrentConfig);

	FRebuildOptions* RO = GRebuildTools.GetCurrent();

	GeomCheck->SetCheck( RO->Options & REBUILD_Geometry ? BST_CHECKED : BST_UNCHECKED );
	BSPCheck->SetCheck( RO->Options & REBUILD_BSP ? BST_CHECKED : BST_UNCHECKED );
	LightingCheck->SetCheck( RO->Options & REBUILD_Lighting ? BST_CHECKED : BST_UNCHECKED );
	DefinePathsCheck->SetCheck( RO->Options & REBUILD_Paths ? BST_CHECKED : BST_UNCHECKED );
	BuildOnlyVisibleCheck->SetCheck( RO->Options & REBUILD_OnlyVisible ? BST_CHECKED : BST_UNCHECKED );

	LameButton->SetCheck( RO->BspOpt == BSP_Lame ? BST_CHECKED : BST_UNCHECKED );
	GoodButton->SetCheck( RO->BspOpt == BSP_Good ? BST_CHECKED : BST_UNCHECKED );
	OptimalButton->SetCheck( RO->BspOpt == BSP_Optimal ? BST_CHECKED : BST_UNCHECKED );

	BalanceBar->SetPos( RO->Balance );
	PortalBiasBar->SetPos( RO->PortalBias );

	if( RO->LightmapFormat == TEXF_DXT1 )
		LightmapCompressionCombo->SetCurrent( TEXT("DXT1") );
	else if( RO->LightmapFormat == TEXF_DXT3 )
		LightmapCompressionCombo->SetCurrent( TEXT("DXT3") );
	else if( RO->LightmapFormat == TEXF_RGBA8 )
		LightmapCompressionCombo->SetCurrent( TEXT("RGB8") );
	DitherLightmapsCheck->SetCheck( RO->DitherLightmaps ? BST_CHECKED : BST_UNCHECKED );
	SaveLightmapsCheck->SetCheck( RO->SaveOutLightmaps ? BST_CHECKED : BST_UNCHECKED );
	ReportErrorsCheck->SetCheck( RO->ReportErrors ? BST_CHECKED : BST_UNCHECKED );

	unguard;
}
// Sends the values in the controls to the current config
void WPageOptions::UpdateCurrentConfig()
{
	guard(WPageOptions::UpdateCurrentConfig);

	FRebuildOptions* RO = GRebuildTools.GetCurrent();

	RO->Options = 0;
	if( GeomCheck->IsChecked() ) RO->Options |= REBUILD_Geometry;
	if( BSPCheck->IsChecked() ) RO->Options |= REBUILD_BSP;
	if( LightingCheck->IsChecked() ) RO->Options |= REBUILD_Lighting;
	if( DefinePathsCheck->IsChecked() ) RO->Options |= REBUILD_Paths;
	if( BuildOnlyVisibleCheck->IsChecked() ) RO->Options |= REBUILD_OnlyVisible;

	if( LameButton->IsChecked() ) RO->BspOpt = BSP_Lame;
	if( GoodButton->IsChecked() ) RO->BspOpt = BSP_Good;
	if( OptimalButton->IsChecked() ) RO->BspOpt = BSP_Optimal;

	RO->Balance = BalanceBar->GetPos();
	RO->PortalBias = PortalBiasBar->GetPos();

	FString Text = LightmapCompressionCombo->GetString( LightmapCompressionCombo->GetCurrent() );
	if( Text == TEXT("DXT1") )
		RO->LightmapFormat = TEXF_DXT1;
	else if( Text == TEXT("DXT3") )
		RO->LightmapFormat = TEXF_DXT3;
	else if( Text == TEXT("RGB8") )
		RO->LightmapFormat = TEXF_RGBA8;
	RO->DitherLightmaps = DitherLightmapsCheck->IsChecked();
	RO->SaveOutLightmaps = SaveLightmapsCheck->IsChecked();
	RO->ReportErrors = ReportErrorsCheck->IsChecked();

	unguard;
}

void WPageOptions::OnBalanceBarMove()
{
	guard(WPageOptions::OnBalanceBarMove);
	GRebuildTools.GetCurrent()->Balance = BalanceBar->GetPos();
	Refresh();
	unguard;
}
void WPageOptions::OnPortalBiasBarMove()
{
	guard(WPageOptions::OnPortalBiasBarMove);
	GRebuildTools.GetCurrent()->PortalBias = PortalBiasBar->GetPos();
	Refresh();
	unguard;
}
void WPageOptions::OnConfigComboSelChange()
{
	guard(WPageOptions::OnConfigComboSelChange);

	FString Name = ConfigCombo->GetString( ConfigCombo->GetCurrent() );
	GRebuildTools.SetCurrent( *Name );
	InitFromCurrentConfig();
	Refresh();

	unguard;
}
void WPageOptions::OnLightmapCompressionComboSelChange()
{
	guard(WPageOptions::OnLightmapCompressionComboSelChange);

	FString Text = LightmapCompressionCombo->GetString( LightmapCompressionCombo->GetCurrent() );
	if( Text == TEXT("DXT1") )
		GRebuildTools.GetCurrent()->LightmapFormat = TEXF_DXT1;
	else if( Text == TEXT("DXT3") )
		GRebuildTools.GetCurrent()->LightmapFormat = TEXF_DXT3;
	else if( Text == TEXT("RGB8") )
		GRebuildTools.GetCurrent()->LightmapFormat = TEXF_RGBA8;
	Refresh();

	unguard;
}
void WPageOptions::OnGeomClick()
{
	guard(WPageOptions::OnGeomClick);
	if( GRebuildTools.GetCurrent()->Options & REBUILD_Geometry )
		GRebuildTools.GetCurrent()->Options &= ~REBUILD_Geometry;
	else
		GRebuildTools.GetCurrent()->Options |= REBUILD_Geometry;
	Refresh();
	unguard;
}
void WPageOptions::OnLameClicked()
{
	guard(WPageOptions::OnLameClicked);
	GRebuildTools.GetCurrent()->BspOpt = BSP_Lame;
	Refresh();
	unguard;
}
void WPageOptions::OnGoodClicked()
{
	guard(WPageOptions::OnGoodClicked);
	GRebuildTools.GetCurrent()->BspOpt = BSP_Good;
	Refresh();
	unguard;
}
void WPageOptions::OnOptimalClicked()
{
	guard(WPageOptions::OnOptimalClicked);
	GRebuildTools.GetCurrent()->BspOpt = BSP_Optimal;
	Refresh();
	unguard;
}
void WPageOptions::OnBSPClick()
{
	guard(WPageOptions::OnBSPClick);
	if( GRebuildTools.GetCurrent()->Options & REBUILD_BSP )
		GRebuildTools.GetCurrent()->Options &= ~REBUILD_BSP;
	else
		GRebuildTools.GetCurrent()->Options |= REBUILD_BSP;
	Refresh();
	unguard;
}
void WPageOptions::OnLightingClick()
{
	guard(WPageOptions::OnLightingClick);
	if( GRebuildTools.GetCurrent()->Options & REBUILD_Lighting )
		GRebuildTools.GetCurrent()->Options &= ~REBUILD_Lighting;
	else
		GRebuildTools.GetCurrent()->Options |= REBUILD_Lighting;
	Refresh();
	unguard;
}
void WPageOptions::OnDitherLightmaps()
{
	guard(WPageOptions::OnDitherLightmaps);
	GRebuildTools.GetCurrent()->DitherLightmaps = !GRebuildTools.GetCurrent()->DitherLightmaps;
	Refresh();
	unguard;
}
void WPageOptions::OnSaveLightmaps()
{
	guard(WPageOptions::OnSaveLightmaps);
	GRebuildTools.GetCurrent()->SaveOutLightmaps = !GRebuildTools.GetCurrent()->SaveOutLightmaps;
	Refresh();
	unguard;
}
void WPageOptions::OnReportErrors()
{
	guard(WPageOptions::OnReportErrors);
	GRebuildTools.GetCurrent()->ReportErrors = !GRebuildTools.GetCurrent()->ReportErrors;
	Refresh();
	unguard;
}
void WPageOptions::OnDefinePathsClick()
{
	guard(WPageOptions::OnDefinePathsClick);
	if( GRebuildTools.GetCurrent()->Options & REBUILD_Paths )
		GRebuildTools.GetCurrent()->Options &= ~REBUILD_Paths;
	else
		GRebuildTools.GetCurrent()->Options |= REBUILD_Paths;
	Refresh();
	unguard;
}
void WPageOptions::OnBuildOnlyVisibleClick()
{
	guard(WPageOptions::OnBuildOnlyVisibleClick);
	if( GRebuildTools.GetCurrent()->Options & REBUILD_OnlyVisible )
		GRebuildTools.GetCurrent()->Options &= ~REBUILD_OnlyVisible;
	else
		GRebuildTools.GetCurrent()->Options |= REBUILD_OnlyVisible;
	unguard;
}
void WPageOptions::BuildGeometry()
{
	guard(WPageOptions::BuildGeometry);
	if( GRebuildTools.GetCurrent()->Options & REBUILD_Geometry )
		GUnrealEd->Exec( TEXT("MAP REBUILD") );
	unguard;
}
void WPageOptions::BuildBSP()
{
	guard(WPageOptions::BuildBSP);
	if( GRebuildTools.GetCurrent()->Options & REBUILD_BSP )
		GUnrealEd->Exec( TEXT("BSP REBUILD") );
	unguard;
}
void WPageOptions::BuildLighting()
{
	guard(WPageOptions::BuildLighting);
	if( GRebuildTools.GetCurrent()->Options & REBUILD_Lighting )
		GUnrealEd->Exec( TEXT("LIGHT APPLY") );
	unguard;
}
void WPageOptions::BuildPaths()
{
	guard(WPageOptions::BuildPaths);
	if( GRebuildTools.GetCurrent()->Options & REBUILD_Paths )
		GUnrealEd->Exec( TEXT("PATHS DEFINE") );
	unguard;
}
void WPageOptions::BuildFluids()
{
	guard(WPageOptions::BuildFluid);
	if( GRebuildTools.GetCurrent()->Options & REBUILD_Fluids )
		GUnrealEd->Exec( TEXT("FLUID REBUILD") );
	unguard;
}
void WPageOptions::OnBuildClick()
{
	guard(WPageOptions::OnBuildClick);

	BuildGeometry();
	BuildBSP();
	BuildLighting();
	BuildPaths();
	BuildFluids();

	if( !GRebuildTools.GetCurrent()->ReportErrors )
		GWarn->MapCheck_Hide();

	unguard;
}
void WPageOptions::OnBuildPathsClick()
{
	guard(WPageOptions::OnBuildPathsClick);
	if( ::MessageBox( hWnd, TEXT("This command will erase all existing pathnodes and attempt to create a pathnode network on its own.  Are you sure this is what you want to do?\n\nNOTE : This process can take a VERY long time."), TEXT("Build Paths"), MB_YESNO) == IDYES )
		GUnrealEd->Exec( TEXT("PATHS BUILD") );
	unguard;
}
void WPageOptions::OnPathsChangedClick()
{
	guard(WPageOptions::OnPathsChangedClick);
	GUnrealEd->Exec( TEXT("PATHS DEFINECHANGED") );
	unguard;
}
void WPageOptions::OnSaveClick()
{
	guard(WPageOptions::OnSaveClick);

	FString Name = ConfigCombo->GetText();
	if( Name == TEXT("Default") )
	{
		appMsgf( 0, TEXT("Can't modify the default config.") );
		return;
	}

	FRebuildOptions* RO = GRebuildTools.Save( Name );
	RefreshConfigCombo();
	ConfigCombo->SetCurrent( *Name );
	UpdateCurrentConfig();
	Refresh();

	unguard;
}
void WPageOptions::OnDeleteClick()
{
	guard(WPageOptions::OnDeleteClick);

	FString Name = ConfigCombo->GetString( ConfigCombo->GetCurrent() );
	if( Name == TEXT("Default") )
	{
		appMsgf( 0, TEXT("Can't delete the default config.") );
		return;
	}

	GRebuildTools.Delete( Name );
	RefreshConfigCombo();
	ConfigCombo->SetCurrent(0);
	Refresh();

	unguard;
}

// --------------------------------------------------------------
//
// WPageLevelStats
//
// --------------------------------------------------------------

// Structors.
WPageLevelStats::WPageLevelStats ( WWindow* InOwnerWindow )
:	WPropertyPage( InOwnerWindow )
{
	GeomBox = BSPBox = LightingBox = DefinePathsBox = NULL;
	RefreshButton = BuildButton = NULL;
	BrushesLabel = ZonesLabel = PolysLabel = NodesLabel
		= RatioLabel = MaxDepthLabel = AvgDepthLabel = LightsLabel = NULL;
}

void WPageLevelStats::OpenWindow( INT InDlgId, HMODULE InHMOD )
{
	guard(WPageLevelStats::OpenWindow);
	WPropertyPage::OpenWindow( InDlgId, InHMOD );

	// Create child controls and let the base class determine their proper positions.
	GeomBox = new WGroupBox( this, IDGP_GEOM );
	GeomBox->OpenWindow( 1, 0 );
	BSPBox = new WGroupBox( this, IDGP_BSP );
	BSPBox->OpenWindow( 1, 0 );
	LightingBox = new WGroupBox( this, IDGP_LIGHTING );
	LightingBox->OpenWindow( 1, 0 );
	DefinePathsBox = new WGroupBox( this, IDGP_PATHS );
	DefinePathsBox->OpenWindow( 1, 0 );
	RefreshButton = new WButton( this, IDPB_REFRESH );
	RefreshButton->OpenWindow( 1, 0, 0, 0, 0, TEXT("") );
	BuildButton = new WButton( this, IDPB_REFRESH );
	BuildButton->OpenWindow( 1, 0, 0, 0, 0, TEXT("") );
	BrushesLabel = new WLabel( this, IDSC_BRUSHES );
	BrushesLabel->OpenWindow( 1, 0 );
	ZonesLabel = new WLabel( this, IDSC_ZONES );
	ZonesLabel->OpenWindow( 1, 0 );
	PolysLabel = new WLabel( this, IDSC_POLYS );
	PolysLabel->OpenWindow( 1, 0 );
	NodesLabel = new WLabel( this, IDSC_NODES );
	NodesLabel->OpenWindow( 1, 0 );
	RatioLabel = new WLabel( this, IDSC_RATIO );
	RatioLabel->OpenWindow( 1, 0 );
	MaxDepthLabel = new WLabel( this, IDSC_MAX_DEPTH );
	MaxDepthLabel->OpenWindow( 1, 0 );
	AvgDepthLabel = new WLabel( this, IDSC_AVG_DEPTH );
	AvgDepthLabel->OpenWindow( 1, 0 );
	LightsLabel = new WLabel( this, IDSC_LIGHTS );
	LightsLabel->OpenWindow( 1, 0 );

	PlaceControl( GeomBox );
	PlaceControl( BSPBox );
	PlaceControl( LightingBox );
	PlaceControl( DefinePathsBox );
	PlaceControl( RefreshButton );
	PlaceControl( BrushesLabel );
	PlaceControl( ZonesLabel );
	PlaceControl( PolysLabel );
	PlaceControl( NodesLabel );
	PlaceControl( RatioLabel );
	PlaceControl( MaxDepthLabel );
	PlaceControl( AvgDepthLabel );
	PlaceControl( LightsLabel );

	Finalize();

	// Delegates.
	RefreshButton->ClickDelegate = FDelegate(this, (TDelegate)&WPageLevelStats::Refresh);

	// Initialize controls.

	unguard;
}
void WPageLevelStats::OnDestroy()
{
	guard(WPageLevelStats::OnDestroy);
	WPropertyPage::OnDestroy();

	::DestroyWindow( GeomBox->hWnd );
	::DestroyWindow( BSPBox->hWnd );
	::DestroyWindow( LightingBox->hWnd );
	::DestroyWindow( DefinePathsBox->hWnd );
	::DestroyWindow( RefreshButton->hWnd );
	::DestroyWindow( BrushesLabel->hWnd );
	::DestroyWindow( ZonesLabel->hWnd );
	::DestroyWindow( PolysLabel->hWnd );
	::DestroyWindow( NodesLabel->hWnd );
	::DestroyWindow( RatioLabel->hWnd );
	::DestroyWindow( MaxDepthLabel->hWnd );
	::DestroyWindow( AvgDepthLabel->hWnd );
	::DestroyWindow( LightsLabel->hWnd );

	delete GeomBox;
	delete BSPBox;
	delete LightingBox;
	delete DefinePathsBox;
	delete BuildButton;
	delete RefreshButton;
	delete BrushesLabel;
	delete ZonesLabel;
	delete PolysLabel;
	delete NodesLabel;
	delete RatioLabel;
	delete MaxDepthLabel;
	delete AvgDepthLabel;
	delete LightsLabel;

	unguard;
}
void WPageLevelStats::Refresh()
{
	guard(WPageLevelStats::Refresh);

	FStringOutputDevice GetPropResult = FStringOutputDevice();

	// GEOMETRY
	GetPropResult.Empty();	GUnrealEd->Get( TEXT("MAP"), TEXT("BRUSHES"), GetPropResult );
	BrushesLabel->SetText( *GetPropResult );
	GetPropResult.Empty();	GUnrealEd->Get( TEXT("MAP"), TEXT("ZONES"), GetPropResult );
	ZonesLabel->SetText( *GetPropResult );

	// BSP
	GetPropResult.Empty();	GUnrealEd->Get( TEXT("BSP"), TEXT("POLYS"), GetPropResult );
	INT Polys = appAtoi( *GetPropResult );
	PolysLabel->SetText( *GetPropResult );
	GetPropResult.Empty();	GUnrealEd->Get( TEXT("BSP"), TEXT("NODES"), GetPropResult );
	INT Nodes = appAtoi( *GetPropResult );
	NodesLabel->SetText( *GetPropResult );
	GetPropResult.Empty();	GUnrealEd->Get( TEXT("BSP"), TEXT("MAXDEPTH"), GetPropResult );
	MaxDepthLabel->SetText( *GetPropResult );
	GetPropResult.Empty();	GUnrealEd->Get( TEXT("BSP"), TEXT("AVGDEPTH"), GetPropResult );
	AvgDepthLabel->SetText( *GetPropResult );

	if(!Polys)
		RatioLabel->SetText(TEXT("N/A"));
	else
	{
		FLOAT Ratio = (Nodes / (FLOAT)Polys);
		RatioLabel->SetText( *FString::Printf( TEXT("%1.2f:1"), Ratio ) );
	}

	// LIGHTING
	GetPropResult.Empty();	GUnrealEd->Get( TEXT("LIGHT"), TEXT("COUNT"), GetPropResult );
	LightsLabel->SetText( *GetPropResult );

	unguard;
}

// --------------------------------------------------------------
//
// WBuildPropSheet
//
// --------------------------------------------------------------

// Structors.
WBuildPropSheet::WBuildPropSheet( FName InPersistentName, WWindow* InOwnerWindow )
:	WWindow( InPersistentName, InOwnerWindow )
{
}

// WBuildPropSheet interface.
void WBuildPropSheet::OpenWindow()
{
	guard(WBuildPropSheet::OpenWindow);
	MdiChild = 0;
	PerformCreateWindowEx
	(
		NULL,
		TEXT("Build Options"),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
		0, 0,
		0, 0,
		OwnerWindow ? OwnerWindow->hWnd : NULL,
		NULL,
		hInstance
	);

	unguard;
}
void WBuildPropSheet::OnCreate()
{
	guard(WBuildPropSheet::OnCreate);
	WWindow::OnCreate();

	// Create the sheet
	PropSheet = new WPropertySheet( this, IDPS_BUILD );
	PropSheet->OpenWindow( 1, 0 );

	// Create the pages for the sheet
	OptionsPage = new WPageOptions( PropSheet->Tabs );
	OptionsPage->OpenWindow( IDPP_BUILD_OPTIONS, GUnrealEdModule);
	PropSheet->AddPage( OptionsPage );

	LevelStatsPage = new WPageLevelStats( PropSheet->Tabs );
	LevelStatsPage->OpenWindow( IDPP_BUILD_STATS, GUnrealEdModule);
	PropSheet->AddPage( LevelStatsPage );

	PropSheet->SetCurrent( 0 );

	// Resize the property sheet to surround the pages properly.
	RECT rect;
	::GetClientRect( OptionsPage->hWnd, &rect );
	::SetWindowPos( hWnd, HWND_TOP, 0, 0, rect.right + 32, rect.bottom + 64, SWP_NOMOVE );

	PositionChildControls();

	unguard;
}
void WBuildPropSheet::OnDestroy()
{
	guard(WBuildPropSheet::OnDestroy);
	WWindow::OnDestroy();

	delete OptionsPage;
	delete LevelStatsPage;
	delete PropSheet;
	unguard;
}
void WBuildPropSheet::OnSize( DWORD Flags, INT NewX, INT NewY )
{
	guard(WBuildPropSheet::OnSize);
	WWindow::OnSize(Flags, NewX, NewY);
	PositionChildControls();
	InvalidateRect( hWnd, NULL, FALSE );
	unguard;
}
void WBuildPropSheet::PositionChildControls()
{
	guard(WBuildPropSheet::PositionChildControls);
	if( !PropSheet || !::IsWindow( PropSheet->hWnd )
			)
		return;

	FRect CR = GetClientRect();
	::MoveWindow( PropSheet->hWnd, 0, 0, CR.Width(), CR.Height(), 1 );

	unguard;
}
INT WBuildPropSheet::OnSysCommand( INT Command )
{
	guard(WBuildPropSheet::OnSysCommand);
	if( Command == SC_CLOSE )
	{
		Show(0);
		return 1;
	}

	return 0;
	unguard;
}

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
