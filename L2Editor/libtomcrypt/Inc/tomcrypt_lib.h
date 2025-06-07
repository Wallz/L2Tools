#ifdef _WIN64
#	ifdef _DEBUG
#		pragma comment( lib, "" )
#	else
#		pragma comment( lib, "" )
#	endif
#else
#	ifdef _DEBUG
#		pragma comment( lib, "tomcryptd.lib" )
#		pragma comment( lib, "tommathd.lib" )
#	else
#		pragma comment( lib, "tomcrypt.lib" )
#		pragma comment( lib, "tommath.lib" )
#	endif
#endif
