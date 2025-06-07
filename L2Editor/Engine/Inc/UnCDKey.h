/*=============================================================================
	UnCDKey.h: CD Key validation
	Copyright 1997-2002 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Jack Porter
=============================================================================*/

/*-----------------------------------------------------------------------------
	Defines
-----------------------------------------------------------------------------*/

#define CDKEYBASEMAP (TEXT("ABCDEFGHJLKMNPQRTUVWXYZ2346789"))
#define CDKEYBASE (appStrlen(CDKEYBASEMAP))
#define CDKEYBASEMAPINDEX(ch) (INT)((appStrchr(CDKEYBASEMAP,(ch))-CDKEYBASEMAP))
#define CDKEYAPPENDSTRING TEXT("This is going to change")

/*-----------------------------------------------------------------------------
	Global CD Key functions
-----------------------------------------------------------------------------*/

UBOOL ENGINE_API ValidateCDKey();
FString ENGINE_API GetCDKeyHash();
FString ENGINE_API GetCDKeyResponse( const TCHAR* Challenge );
FString ENGINE_API EncryptWithCDKeyHash( const TCHAR* String, const TCHAR* HashAppend );
FString ENGINE_API DecryptWithCDKeyHash( const TCHAR* String, const TCHAR* HashAppend, const TCHAR* InCDKey );

/*-----------------------------------------------------------------------------
	Inlines (for security)
-----------------------------------------------------------------------------*/

union FCdKeyMD5Qword
{
	BYTE Digest[16];
	QWORD Q;
};

inline TCHAR HexToCDKeyMap( TCHAR ch )
{
	if( ch >= '0' && ch <='9' )
		return CDKEYBASEMAP[ch-'0'];
	if( ch >= 'A' && ch <='Z' )
		return CDKEYBASEMAP[ch-'A'+10];
	if( ch >= 'a' && ch <='z' )
		return CDKEYBASEMAP[ch-'a'+10];
	return 0;
}

inline FString HexToCDKeyMap( const TCHAR *hex )
{
	FString Map;
	for( INT i=0;hex[i];i++ )
		Map = Map + FString::Printf(TEXT("%c"), HexToCDKeyMap(hex[i]) );
	return Map;
}

inline TCHAR CDKeyMapToHex( TCHAR ch )
{
	INT Index = CDKEYBASEMAPINDEX(ch);
	if( Index < 10 )
		return Index + '0';
	else
		return Index - 10 + 'a';
}

inline FString CDKeyMapToHex( const TCHAR* ch )
{
	FString Map;
	for( INT i=0;ch[i];i++ )
		Map = Map + FString::Printf(TEXT("%c"), CDKeyMapToHex(ch[i]) );
	return Map;
}

inline UBOOL ValidateCDKey( const TCHAR* CDKey )
{
	FString CDKeyStr = CDKey;
	FString FullKey = CDKeyStr.Mid(12,5)+CDKeyStr.Mid(6,5)+CDKeyStr.Mid(0,5)+CDKeyStr.Mid(18,5);
	FString Random = FullKey.Left(14);

	QWORD Seed = appStrtoq( *CDKeyMapToHex(*Random), NULL, CDKEYBASE );
	
	FString Check = FString::Printf(TEXT("%I64d%s"), Seed, CDKEYAPPENDSTRING);

	const ANSICHAR* AnsiCheck = appToAnsi(*Check);
	FCdKeyMD5Qword md5;

	FMD5Context Context;
	appMD5Init( &Context );
	appMD5Update( &Context, (unsigned char*)AnsiCheck, Check.Len() );
	appMD5Final( md5.Digest, &Context );

	FString CheckOutput = HexToCDKeyMap(appQtoa(md5.Q, CDKEYBASE));
	CheckOutput = CheckOutput.Left(6);
	while( CheckOutput.Len() < 6 )
		CheckOutput = FString::Printf( TEXT("%c%s"), CDKEYBASEMAP[0], *CheckOutput );

	return CheckOutput == FullKey.Mid(14);
}

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

