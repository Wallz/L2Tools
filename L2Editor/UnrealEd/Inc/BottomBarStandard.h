/*=============================================================================
	BottomBarStandard : The standard controls that are always at the bottom
	                    of the editor window.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Warren Marshall

    Work-in-progress todo's:

=============================================================================*/

// --------------------------------------------------------------
//
// WBottomBarStandard
//
// --------------------------------------------------------------

HWND GHwndBottomBarStandard = NULL;

#define IDSC_LOG_COMMAND				19500
#define IDCB_LOG_COMMAND				19501
#define IDPB_LOG_WND					19502
#define IDCK_SNAP_VERTEX				19504
#define IDCK_TOGGLE_GRID				19505
#define IDCB_GRID_SIZE					19506
#define IDCK_TOGGLE_ROT_GRID			19507
#define IDCB_ROT_GRID_SIZE				19508
#define IDCK_ZOOMCENTER_ALL				19509
#define IDCK_ZOOMCENTER					19510
#define IDCK_MAXIMIZE					19511
#define IDCK_SELECTION_LOCK				19514
// gam ---
#define IDSC_DRAW_SCALE_3D_LABEL		20001
#define IDEC_DRAW_SCALE_3D_X		    20002
#define IDEC_DRAW_SCALE_3D_Y		    20003
#define IDEC_DRAW_SCALE_3D_Z		    20004
// --- gam

#define dSPACE_BETWEEN_GROUPS 16
extern HWND GhwndEditorFrame;

struct {
	INT ID;
	INT Width;
	INT Height;
	INT Pad;		// Amount of space to leave between this control and the one to it's right
	TCHAR ToolTip[64];
}
GWBB_WndPos[] =
{
	IDSC_LOG_COMMAND,			64, 	20,		4,						TEXT("Enter a Log Command and Press ENTER"),
	IDCB_LOG_COMMAND,			384,	256,	4,						TEXT("Enter a Log Command and Press ENTER"),
	IDPB_LOG_WND,				22,		20,		dSPACE_BETWEEN_GROUPS,	TEXT("Show Full Log Window"),
	IDCK_SELECTION_LOCK,		22,		20,		4,						TEXT("Lock Selections?"),
	IDCK_SNAP_VERTEX,			22,		20,		dSPACE_BETWEEN_GROUPS,	TEXT("Toggle Vertex Snap"),
	IDCK_TOGGLE_GRID,			22,		20,		4,						TEXT("Toggle Drag Grid"),
	IDCB_GRID_SIZE,				64,		256,	dSPACE_BETWEEN_GROUPS,	TEXT("Drag Grid Size"),
	IDCK_TOGGLE_ROT_GRID,		22,		20,		4,						TEXT("Toggle Rotation Grid"),
	IDCK_MAXIMIZE,				22,		20,		dSPACE_BETWEEN_GROUPS,	TEXT("Maximize Viewport"),
    // gam ---
	IDSC_DRAW_SCALE_3D_LABEL,	80,		20,		4,						TEXT("DrawScale3D"),
	IDEC_DRAW_SCALE_3D_X,		48,		20,		4,						TEXT("DrawScale3D.X"),
	IDEC_DRAW_SCALE_3D_Y,		48,		20,		4,						TEXT("DrawScale3D.Y"),
	IDEC_DRAW_SCALE_3D_Z,		48,		20,		dSPACE_BETWEEN_GROUPS,	TEXT("DrawScale3D.Z"),
    // --- gam
	-1, -1, -1, -1, TEXT("")
};

WNDPROC lpfnEditWndProc = NULL; // original wndproc for the combo box 
FString GCommand;
LRESULT CALLBACK LogCommandEdit_Proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_KEYDOWN:
			switch (wParam)
			{
				case VK_RETURN:
				{
					char Text[256];
					::GetWindowTextA( hwnd, Text, 256 );
					GCommand = appFromAnsi( Text );
					GUnrealEd->Exec( *GCommand );
					PostMessageX( GHwndBottomBarStandard, WM_COMMAND, WM_ADD_MRU, 0 );
					return 0;
				}
			}
			break;
	}

	// Call the original window procedure for default processing.
	return CallWindowProcA(lpfnEditWndProc, hwnd, msg, wParam, lParam);
}

// gam ---
WNDPROC lpfnEditWndProcDrawScale3D = NULL; // original wndproc for the DrawScale3D edit boxes
LRESULT CALLBACK DrawScale3DEdit_Proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_KEYDOWN:
			switch (wParam)
			{
				case VK_RETURN:
				{
					PostMessageX( GHwndBottomBarStandard, WM_COMMAND, WM_APPLY_DRAW_SCALE, FALSE );
					return 0;
				}
				case VK_TAB:
				{
					PostMessageX( GHwndBottomBarStandard, WM_COMMAND, WM_APPLY_DRAW_SCALE, TRUE );
					return 0;
				}
			}
			break;
	}

	// Call the original window procedure for default processing.
	return CallWindowProcA(lpfnEditWndProcDrawScale3D, hwnd, msg, wParam, lParam);
}
// --- gam

#define MAX_MRU_COMMANDS 16

class WBottomBarStandard : public WWindow
{
	DECLARE_WINDOWCLASS(WBottomBarStandard,WWindow,Window)

	WToolTip* ToolTipCtrl;

	WCustomLabel *LogCommandLabel;
	WComboBox *LogCommandCombo, *DragGridSizeCombo;
	WPictureButton *SnapVertexCheck, *DragGridCheck, *RotGridCheck, *MaximizeCheck, *SelectionLockCheck;
	WButton *LogWndButton;
	HBRUSH hbrDark;

	TArray<WPictureButton> PictureButtons;
	TArray<WButton> Buttons;
	HBITMAP hbm, hbmLogWnd;
	BITMAP bm;
	FString MRUCommands[MAX_MRU_COMMANDS];
	INT NumMRUCommands;

    // gam ---
    WCustomLabel *DrawScale3DLabel;
    WEdit *DrawScale3DEdits[3];
    // --- gam

	// Structors.
	WBottomBarStandard( FName InPersistentName, WWindow* InOwnerWindow )
	:	WWindow( InPersistentName, InOwnerWindow )
	{
		//hbmLogWnd = NULL;
		hbm = (HBITMAP)LoadImageA( hInstance, MAKEINTRESOURCEA(IDB_BOTTOM_BAR), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR );	check(hbm);
		hbmLogWnd = (HBITMAP)LoadImageA( hInstance, MAKEINTRESOURCEA(IDB_BB_LOG_WND), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR );	check(hbmLogWnd);
		GetObjectA( hbm, sizeof(BITMAP), (LPSTR)&bm );
		NumMRUCommands = 0;
		hbrDark = CreateSolidBrush(RGB(128,128,128));
	}
	void OpenWindow()
	{
		guard(WBottomBarStandard::OpenWindow);
		MdiChild = 0;

		PerformCreateWindowEx
		(
			0,
			NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			OwnerWindow ? OwnerWindow->hWnd : NULL,
			NULL,
			hInstance
		);
		unguard;
	}
	void OnDestroy()
	{
		guard(WBottomBarStandard::OnDestroy);

		for( INT x = 0 ; x < PictureButtons.Num() ; ++x )
			DestroyWindow( PictureButtons(x).hWnd );
		PictureButtons.Empty();
		for( INT x = 0 ; x < Buttons.Num() ; ++x )
			DestroyWindow( Buttons(x).hWnd );
		Buttons.Empty();
		DeleteObject(hbm);
		DeleteObject(hbmLogWnd);
		DeleteObject(hbrDark);

		delete LogCommandLabel;
		delete LogCommandCombo;
		delete DragGridSizeCombo;

        // gam ---
        delete DrawScale3DLabel;
        DrawScale3DLabel = NULL;

        for( INT i = 0; i < ARRAY_COUNT( DrawScale3DEdits ); i++ )
        {
            delete DrawScale3DEdits[i];
            DrawScale3DEdits[i] = NULL;
        }
        // --- gam

		delete ToolTipCtrl;

		WWindow::OnDestroy();
		unguard;
	}
	void OnCreate()
	{
		guard(WBottomBarStandard::OnCreate);
		WWindow::OnCreate();

		GHwndBottomBarStandard = hWnd;

		ToolTipCtrl = new WToolTip(this);
		ToolTipCtrl->OpenWindow();

		// Create the child controls.  We also set their initial positions here, aligned to
		// the left hand edge.  When this window resizes itself, all controls will be shifted to
		// be aligned to the right edge, leaving room on the left for a text status line.

		RECT rc;
		::GetClientRect( hWnd, &rc );

		LogCommandLabel = new WCustomLabel( this, IDSC_LOG_COMMAND );
		LogCommandLabel->OpenWindow( 1, 0, SS_CENTER );
		LogCommandLabel->MoveWindow( 0, 0, 1, 1, 1);
		LogCommandLabel->SetText( TEXT("Command :") );

		LogCommandCombo = new WComboBox( this, IDCB_LOG_COMMAND );
		LogCommandCombo->OpenWindow( 1, 0, CBS_DROPDOWN );
		LogCommandCombo->MoveWindow( 0, 0, 32, 32, 1);
		LogCommandCombo->SelectionChangeDelegate = FDelegate(this, (TDelegate)&WBottomBarStandard::OnLogCommandSelChange);

		// We subclass the edit control inside of the combobox so we can catch events that
		// are normally invisible to us ... like the user pressing ENTER.
		HWND hwndEdit = GetWindow( LogCommandCombo->hWnd, GW_CHILD );
		check(hwndEdit);
		lpfnEditWndProc = (WNDPROC)SetWindowLongA(hwndEdit, GWL_WNDPROC, (LONG)LogCommandEdit_Proc);
		check(lpfnEditWndProc);

		new(Buttons)WButton( this, IDPB_LOG_WND );
		LogWndButton = &(Buttons(Buttons.Num() - 1));	check(LogWndButton);
		LogWndButton->OpenWindow( 1, 0, 0, 22, 20, NULL, 0, BS_OWNERDRAW );
		LogWndButton->SetBitmap( hbmLogWnd );
		
		new(PictureButtons)WPictureButton( this );
		SnapVertexCheck = &(PictureButtons(PictureButtons.Num() - 1));	check(SnapVertexCheck);
		SnapVertexCheck->SetUp( TEXT(""), IDCK_SNAP_VERTEX, 
			0, 0, 22, 20,
			hbm, 44, 0, 22, 20,
			hbm, 44, 20, 22, 20 );
		SnapVertexCheck->OpenWindow();
		SnapVertexCheck->OnSize( SIZE_MAXSHOW, 22, 20 );

		new(PictureButtons)WPictureButton( this );
		SelectionLockCheck = &(PictureButtons(PictureButtons.Num() - 1));	check(SelectionLockCheck);
		SelectionLockCheck->SetUp( TEXT(""), IDCK_SELECTION_LOCK, 
			0, 0, 22, 20,
			hbm, 220, 0, 22, 20,
			hbm, 220, 20, 22, 20 );
		SelectionLockCheck->OpenWindow();
		SelectionLockCheck->OnSize( SIZE_MAXSHOW, 22, 20 );

		new(PictureButtons)WPictureButton( this );
		DragGridCheck = &(PictureButtons(PictureButtons.Num() - 1));	check(DragGridCheck);
		DragGridCheck->SetUp( TEXT(""), IDCK_TOGGLE_GRID, 
			0, 0, 22, 20,
			hbm, 66, 0, 22, 20,
			hbm, 66, 20, 22, 20 );
		DragGridCheck->OpenWindow();
		DragGridCheck->OnSize( SIZE_MAXSHOW, 22, 20 );

		DragGridSizeCombo = new WComboBox( this, IDCB_GRID_SIZE );
		DragGridSizeCombo->OpenWindow( 1, 0 );
		DragGridSizeCombo->MoveWindow( 0, 0, 1, 1, 1);
		DragGridSizeCombo->AddString( TEXT("1") );
		DragGridSizeCombo->AddString( TEXT("2") );
		DragGridSizeCombo->AddString( TEXT("4") );
		DragGridSizeCombo->AddString( TEXT("8") );
		DragGridSizeCombo->AddString( TEXT("16") );
		DragGridSizeCombo->AddString( TEXT("32") );
		DragGridSizeCombo->AddString( TEXT("64") );
		DragGridSizeCombo->AddString( TEXT("128") );
		DragGridSizeCombo->AddString( TEXT("256") );
		DragGridSizeCombo->AddString( TEXT("512") );
		DragGridSizeCombo->AddString( TEXT("1024") );
		DragGridSizeCombo->AddString( TEXT("2048") );
		DragGridSizeCombo->AddString( TEXT("4096") );
		DragGridSizeCombo->SelectionChangeDelegate = FDelegate(this, (TDelegate)&WBottomBarStandard::OnDragGridSizeSelChange);

		new(PictureButtons)WPictureButton( this );
		RotGridCheck = &(PictureButtons(PictureButtons.Num() - 1));	check(RotGridCheck);
		RotGridCheck->SetUp( TEXT(""), IDCK_TOGGLE_ROT_GRID, 
			0, 0, 22, 20,
			hbm, 88, 0, 22, 20,
			hbm, 88, 20, 22, 20 );
		RotGridCheck->OpenWindow();
		RotGridCheck->OnSize( SIZE_MAXSHOW, 22, 20 );

		new(PictureButtons)WPictureButton( this );
		MaximizeCheck = &(PictureButtons(PictureButtons.Num() - 1));	check(MaximizeCheck);
		MaximizeCheck->SetUp( TEXT(""), IDCK_MAXIMIZE,
			0, 0, 22, 20,
			hbm, 154, 0, 22, 20,
			hbm, 154, 20, 22, 20 );
		MaximizeCheck->OpenWindow();
		MaximizeCheck->OnSize( SIZE_MAXSHOW, 22, 20 );

        // gam ---
		DrawScale3DLabel = new WCustomLabel( this, IDSC_DRAW_SCALE_3D_LABEL );
		DrawScale3DLabel->OpenWindow( 1, 0, SS_CENTER );
		DrawScale3DLabel->SetText( TEXT("DrawScale3D:") );

        DrawScale3DEdits[0] = new WEdit( this, IDEC_DRAW_SCALE_3D_X );
		DrawScale3DEdits[0]->OpenWindow( 1, 0, 0 );
		DrawScale3DEdits[0]->BlurDelegate = FDelegate(this, (TDelegate)&WBottomBarStandard::ApplyDrawScale3D);

        DrawScale3DEdits[1] = new WEdit( this, IDEC_DRAW_SCALE_3D_Y );
		DrawScale3DEdits[1]->OpenWindow( 1, 0, 0 );
		DrawScale3DEdits[1]->BlurDelegate = FDelegate(this, (TDelegate)&WBottomBarStandard::ApplyDrawScale3D);

        DrawScale3DEdits[2] = new WEdit( this, IDEC_DRAW_SCALE_3D_Z );
		DrawScale3DEdits[2]->OpenWindow( 1, 0, 0 );
		DrawScale3DEdits[2]->BlurDelegate = FDelegate(this, (TDelegate)&WBottomBarStandard::ApplyDrawScale3D);

		lpfnEditWndProcDrawScale3D = (WNDPROC)SetWindowLongA(DrawScale3DEdits[0]->hWnd, GWL_WNDPROC, (LONG)DrawScale3DEdit_Proc);
		check(lpfnEditWndProcDrawScale3D);

		verify( (WNDPROC)SetWindowLongA(DrawScale3DEdits[1]->hWnd, GWL_WNDPROC, (LONG)DrawScale3DEdit_Proc) == lpfnEditWndProcDrawScale3D);
		verify( (WNDPROC)SetWindowLongA(DrawScale3DEdits[2]->hWnd, GWL_WNDPROC, (LONG)DrawScale3DEdit_Proc) == lpfnEditWndProcDrawScale3D);
        // --- gam

		UpdateButtons();

		unguard;
	}
	void RefreshComboBox()
	{
		guard(WBottomBarStandard::RefreshComboBox);

		// Update the contents of the combobox with the MRU list of commands
		LogCommandCombo->Empty();

		for( INT x = 0 ; x < NumMRUCommands ; ++x )
			LogCommandCombo->AddString( *MRUCommands[x] );

		unguard;
	}
	void AddCommandToMRU( FString InCommand )
	{
		guard(WBottomBarStandard::AddCommandToMRU);

		// If the command already exists, leave.
		for( INT x = 0 ; x < NumMRUCommands ; ++x )
			if( InCommand == MRUCommands[x] )
				return;

		// Otherwise, add it to end of the list.
		NumMRUCommands++;
		if( NumMRUCommands > MAX_MRU_COMMANDS )
		{
			NumMRUCommands = MAX_MRU_COMMANDS;
			for( INT x = 0 ; x < MAX_MRU_COMMANDS - 1 ; ++x )
				MRUCommands[x] = MRUCommands[x + 1];
		}

		MRUCommands[NumMRUCommands - 1] = InCommand;

		RefreshComboBox();

		unguard;
	}
	void OnSize( DWORD Flags, INT NewX, INT NewY )
	{
		guard(WBottomBarStandard::OnSize);
		WWindow::OnSize(Flags, NewX, NewY);
		PositionChildControls();
		InvalidateRect( hWnd, NULL, FALSE );
		unguard;
	}
	void PositionChildControls( void )
	{
		guard(WBottomBarStandard::PositionChildControls);

		RECT rc;
		::GetClientRect( hWnd, &rc );

		INT LastX = 2;
		
		FDeferWindowPos dwp;

		for( INT x = 0 ; GWBB_WndPos[x].ID != -1 ; ++x )
		{
			HWND hwnd = GetDlgItem( hWnd, GWBB_WndPos[x].ID );
			if( !hwnd )
			{
				// Check the other controls to see if we can get a match.
				switch( GWBB_WndPos[x].ID )
				{
					case IDPB_LOG_WND:
						hwnd = LogWndButton->hWnd;
						break;
					case IDCK_SNAP_VERTEX:
						hwnd = SnapVertexCheck->hWnd;
						break;
					case IDCK_SELECTION_LOCK:
						hwnd = SelectionLockCheck->hWnd;
						break;
					case IDCK_TOGGLE_GRID:
						hwnd = DragGridCheck->hWnd;
						break;
					case IDCK_TOGGLE_ROT_GRID:
						hwnd = RotGridCheck->hWnd;
						break;
					case IDCK_MAXIMIZE:
						hwnd = MaximizeCheck->hWnd;
						break;
                    // gam ---
                    case IDSC_DRAW_SCALE_3D_LABEL:
						hwnd = DrawScale3DLabel->hWnd;
                        break;
                    case IDEC_DRAW_SCALE_3D_X:
						hwnd = DrawScale3DEdits[0]->hWnd;
						break;
                    case IDEC_DRAW_SCALE_3D_Y:
						hwnd = DrawScale3DEdits[1]->hWnd;
						break;
                    case IDEC_DRAW_SCALE_3D_Z:
						hwnd = DrawScale3DEdits[2]->hWnd;
						break;
                    // --- gam
				}
			}
			check(hwnd);

			dwp.MoveWindow( hwnd, LastX, rc.bottom / 4, GWBB_WndPos[x].Width, GWBB_WndPos[x].Height, 1 );
			LastX += GWBB_WndPos[x].Width + GWBB_WndPos[x].Pad;

			ToolTipCtrl->AddTool( hwnd, GWBB_WndPos[x].ToolTip, GWBB_WndPos[x].ID );
		}

		unguard;
	}
	void OnPaint()
	{
		guard(WBottomBarStandard::OnPaint);
		PAINTSTRUCT PS;
		HDC hDC = BeginPaint( *this, &PS );

		FRect Rect = GetClientRect();
		FillRect( hDC, Rect, hBrushGrey );

		EndPaint( *this, &PS );

		unguard;
	}
	// Updates the states of the buttons to match editor settings.
	void OnCommand( INT Command )
	{
		guard(WBottomBarStandard::OnCommand);

		switch( Command )
		{
			case WM_ADD_MRU:
				AddCommandToMRU( GCommand );
				break;

			case WM_BB_RCLICK:
			{
			}
			break;

			case WM_PB_PUSH:
				ButtonClicked(LastlParam);
				break;
            
            // gam ---
            case WM_APPLY_DRAW_SCALE:
            {
                ApplyDrawScale3D();

                WEdit* CurrentEdit = NULL;
                HWND CurrentFocus = GetFocus();

                for( INT i = 0; i < ARRAY_COUNT( DrawScale3DEdits ); i++)
                {
                    if( DrawScale3DEdits[i]->hWnd != CurrentFocus )
                        continue;

                    if( !LastlParam )
                        CurrentEdit = DrawScale3DEdits[i];
                    else
                    {
                        CurrentEdit = DrawScale3DEdits[(i + 1) % ARRAY_COUNT( DrawScale3DEdits )];
                        SetFocus( CurrentEdit->hWnd );
                    }

                    break;
                }

                if( CurrentEdit )
                    CurrentEdit->SetSelection( 0, -1 );

                break;
            }
            // --- gam

			default:
				switch( HIWORD(Command) )
				{
					case BN_CLICKED:
					{
						switch( LOWORD(Command) )
						{
							case IDCK_SNAP_VERTEX:
							{
								GUnrealEd->Constraints.SnapVertices = !GUnrealEd->Constraints.SnapVertices;
								UpdateButtons();
							}
							break;

							case IDCK_SELECTION_LOCK:
							{
								GEdSelectionLock = !GEdSelectionLock;
								GUnrealEd->Exec( *FString::Printf(TEXT("MODE SELECTIONLOCK=%d"), GEdSelectionLock ) );
								UpdateButtons();
							}
							break;

							case IDCK_TOGGLE_GRID:
							{
								GUnrealEd->Constraints.GridEnabled = !GUnrealEd->Constraints.GridEnabled;
								UpdateButtons();
							}
							break;

							case IDCK_TOGGLE_ROT_GRID:
							{
								GUnrealEd->Constraints.RotGridEnabled = !GUnrealEd->Constraints.RotGridEnabled;
								UpdateButtons();
							}
							break;

							case IDPB_LOG_WND:
							{
								if( GLogWindow )
								{
									GLogWindow->Show(1);
									SetFocus( *GLogWindow );
									GLogWindow->Display.ScrollCaret();
								}
								UpdateButtons();
							}
							break;

							// Center the current viewport on the selected object (make it fit in the viewport too)
							case IDCK_ZOOMCENTER:
							{
							}
							break;

							// Same as IDCK_ZOOMCENTER, but it affects ALL viewports.
							case IDCK_ZOOMCENTER_ALL:
							{
							}
							break;

							case IDCK_MAXIMIZE:
							{
								if( GCurrentViewportFrame )
								{
									if( IsZoomed( GCurrentViewportFrame ) )
										ShowWindow( GCurrentViewportFrame, SW_RESTORE );
									else
										ShowWindow( GCurrentViewportFrame, SW_MAXIMIZE );
									UpdateButtons();
								}
							}
							break;

							default:
								WWindow::OnCommand(Command);
								break;
						}
					}
					break;
				}
				break;
		}

		unguard;
	}
	void ButtonClicked( INT ID )
	{
		guard(WBottomBarStandard::ButtonClicked);

		switch( ID )
		{
			case IDPB_LOG_WND:
				if( GLogWindow )
				{
					GLogWindow->Show(1);
					SetFocus( *GLogWindow );
					GLogWindow->Display.ScrollCaret();
				}
				UpdateButtons();
				break;

			case IDCK_SNAP_VERTEX:
			{
				GUnrealEd->Constraints.SnapVertices = !GUnrealEd->Constraints.SnapVertices;
				UpdateButtons();
			}
			break;

			case IDCK_SELECTION_LOCK:
			{
				GEdSelectionLock = !GEdSelectionLock;
				GUnrealEd->RedrawLevel( GUnrealEd->Level );
				UpdateButtons();
			}
			break;

			case IDCK_TOGGLE_GRID:
			{
				GUnrealEd->Constraints.GridEnabled = !GUnrealEd->Constraints.GridEnabled;
				UpdateButtons();
			}
			break;

			case IDCK_TOGGLE_ROT_GRID:
			{
				GUnrealEd->Constraints.RotGridEnabled = !GUnrealEd->Constraints.RotGridEnabled;
				UpdateButtons();
			}
			break;

			case IDCK_MAXIMIZE:
			{
				if( GCurrentViewportFrame )
				{
					if( IsZoomed( GCurrentViewportFrame ) )
						ShowWindow( GCurrentViewportFrame, SW_RESTORE );
					else
						ShowWindow( GCurrentViewportFrame, SW_MAXIMIZE );
					UpdateButtons();
				}
			}
			break;
		}

		UpdateButtons();
		InvalidateRect( hWnd, NULL, FALSE );

		unguard;
	}
	INT OnSetCursor()
	{
		guard(WBottomBarStandard::OnSetCursor);
		WWindow::OnSetCursor();
		SetCursor(LoadCursorIdX(NULL,IDC_ARROW));
		return 0;
		unguard;
	}
	// Updates the states of the buttons to match editor settings.
	void UpdateButtons()
	{
		guard(WBottomBarStandard::UpdateButtons);

		SnapVertexCheck->bOn = !GUnrealEd->Constraints.SnapVertices;
		InvalidateRect( SnapVertexCheck->hWnd, NULL, 1 );
		SelectionLockCheck->bOn = !GEdSelectionLock;
		InvalidateRect( SelectionLockCheck->hWnd, NULL, 1 );
		DragGridCheck->bOn = !GUnrealEd->Constraints.GridEnabled;
		InvalidateRect( DragGridCheck->hWnd, NULL, 1 );
		RotGridCheck->bOn = !GUnrealEd->Constraints.RotGridEnabled;
		InvalidateRect( RotGridCheck->hWnd, NULL, 1 );
		MaximizeCheck->bOn = !IsZoomed( GCurrentViewportFrame );
		InvalidateRect( MaximizeCheck->hWnd, NULL, 1 );

		FString Size;
	
		Size = *FString::Printf(TEXT("%d"), (int)(GUnrealEd->Constraints.GridSize.X) );
		DragGridSizeCombo->SetCurrent( DragGridSizeCombo->FindStringExact( *Size ) );

        ReadDrawScale3D(); // gam

		unguard;
	}

	void OnDragGridSizeSelChange()
	{
		guard(WBottomBarStandard::OnDragGridSizeSelChange);

		FString Size = DragGridSizeCombo->GetString( DragGridSizeCombo->GetCurrent() );
		INT iSize = ::atoi( appToAnsi(*Size) );

		GUnrealEd->Constraints.GridSize.X = GUnrealEd->Constraints.GridSize.Y = GUnrealEd->Constraints.GridSize.Z = iSize;
		UpdateButtons();
		PostMessageX( GhwndEditorFrame, WM_COMMAND, WM_REDRAWALLVIEWPORTS, 0 );

		unguard;
	}
	void OnLogCommandSelChange()
	{
		guard(WBottomBarStandard::OnLogCommandSelChange);
		GUnrealEd->Exec( *LogCommandCombo->GetString( LogCommandCombo->GetCurrent() ) );
		unguard;
	}

    // gam ---
    void ReadDrawScale3D()
    {
		guard(WBottomBarStandard::ReadDrawScale3D);

        FVector DrawScale3D;
        bool bScaleX, bScaleY, bScaleZ;
        bool bFirst;

        bScaleX = true;
        bScaleY = true;
        bScaleZ = true;
        bFirst = true;

        if( !DrawScale3DEdits[0] )
            return;

		for( INT i = 0; i < GEditor->Level->Actors.Num(); i++ )
		{
			if( !GEditor->Level->Actors(i) || !GEditor->Level->Actors(i)->bSelected )
                continue;

            if( bFirst )
            {
                DrawScale3D = GEditor->Level->Actors(i)->DrawScale3D;
                bFirst = false;
                continue;
            }

            if( bScaleX )
            {
				if( DrawScale3D.X != GEditor->Level->Actors(i)->DrawScale3D.X )
                    bScaleX = false;
            }

            if( bScaleY )
            {
				if( DrawScale3D.Y != GEditor->Level->Actors(i)->DrawScale3D.Y )
                    bScaleY = false;
            }

            if( bScaleZ )
            {
				if( DrawScale3D.Z != GEditor->Level->Actors(i)->DrawScale3D.Z )
                    bScaleZ = false;
            }

            if( !bScaleX && !bScaleY && !bScaleZ )
                break;
		}

        if( bFirst || !bScaleX )
            DrawScale3DEdits[0]->SetText( TEXT("") );
        else
            DrawScale3DEdits[0]->SetText( *FString::Printf(TEXT("%0.4f"), DrawScale3D.X ) );

        if( bFirst || !bScaleY )
            DrawScale3DEdits[1]->SetText( TEXT("") );
        else
            DrawScale3DEdits[1]->SetText( *FString::Printf(TEXT("%0.4f"), DrawScale3D.Y ) );

        if( bFirst || !bScaleZ )
            DrawScale3DEdits[2]->SetText( TEXT("") );
        else
            DrawScale3DEdits[2]->SetText( *FString::Printf(TEXT("%0.4f"), DrawScale3D.Z ) );

        unguard;
    }

    void ApplyDrawScale3D()
    {
		guard(WBottomBarStandard::ApplyDrawScale3D);

        FVector DrawScale3D;
        bool bScaleX, bScaleY, bScaleZ;
        bool bNeedToScaleX, bNeedToScaleY, bNeedToScaleZ;
        FString Value;

        if( !DrawScale3DEdits[0] )
            return;

        Value = DrawScale3DEdits[0]->GetText();

        if( Value.Len() == 0 )
            bScaleX = false;
        else
        {
            DrawScale3D.X = appAtof( *Value );
            bScaleX = (Abs(DrawScale3D.X) > KINDA_SMALL_NUMBER);
        }

        Value = DrawScale3DEdits[1]->GetText();

        if( Value.Len() == 0 )
            bScaleY = false;
        else
        {
            DrawScale3D.Y = appAtof( *Value );
            bScaleY = (Abs(DrawScale3D.Y) > KINDA_SMALL_NUMBER);
        }

        Value = DrawScale3DEdits[2]->GetText();

        if( Value.Len() == 0 )
            bScaleZ = false;
        else
        {
            DrawScale3D.Z = appAtof( *Value );
            bScaleZ = (Abs(DrawScale3D.Z) > KINDA_SMALL_NUMBER);
        }

        if( !bScaleX && !bScaleY && !bScaleZ )
        {
            debugf( TEXT("DrawScale3D Apply: No Scale") );
            return;
        }

        bNeedToScaleX = false;
        bNeedToScaleY = false;
        bNeedToScaleZ = false;

		for( INT i = 0; i < GEditor->Level->Actors.Num(); i++ )
		{
			if( !GEditor->Level->Actors(i) || !GEditor->Level->Actors(i)->bSelected )
                continue;

            if( bScaleX && GEditor->Level->Actors(i)->DrawScale3D.X != DrawScale3D.X )
                bNeedToScaleX = true;

            if( bScaleY && GEditor->Level->Actors(i)->DrawScale3D.Y != DrawScale3D.Y )
                bNeedToScaleY = true;

            if( bScaleZ && GEditor->Level->Actors(i)->DrawScale3D.Z != DrawScale3D.Z )
                bNeedToScaleZ = true;
		}

        if( !bNeedToScaleX && !bNeedToScaleY && !bNeedToScaleZ )
        {
            debugf( TEXT("DrawScale3D Apply: No Need") );
            return;
        }

        GEditor->Trans->Begin( TEXT("DrawScale3D Apply") );

		for( INT i = 0; i < GEditor->Level->Actors.Num(); i++ )
		{
			if( !GEditor->Level->Actors(i) || !GEditor->Level->Actors(i)->bSelected )
                continue;

            if( bScaleX )
				GEditor->Level->Actors(i)->DrawScale3D.X = DrawScale3D.X;

            if( bScaleY )
				GEditor->Level->Actors(i)->DrawScale3D.Y = DrawScale3D.Y;

            if( bScaleZ )
				GEditor->Level->Actors(i)->DrawScale3D.Z = DrawScale3D.Z;

            GEditor->Level->Actors(i)->PostEditChange();
		}

        GEditor->Trans->End();

		GEditor->EdCallback( EDC_RedrawAllViewports, 0, 0 );

		unguard;
    }
    // --- gam

	void Refresh()
	{
		UpdateButtons();
	}
};

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
