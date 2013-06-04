#include ".\filepathutil.h"

#include <string.h>
#include <stdlib.h>
#include <direct.h>

CFilePathUtil::CFilePathUtil()
{

}




CFilePathUtil::~CFilePathUtil()
{

}



void CFilePathUtil::SplitPath(const char *path)
{
	const char *fullpath = FullPath(path);

	_splitpath_s
		(
		fullpath,
		drive,
		dir,
		fname,
		ext
		);
}


const char *CFilePathUtil::JoinPath()
{
	_makepath_s
		(
		buf,
		drive,
		dir,
		fname,
		ext
		);

	return buf;
}

void CFilePathUtil::FullPath( const char* szPath, char* szReturnValue )
{
	strcpy( szReturnValue, FullPath( szPath ) );
}

char *CFilePathUtil::FullPath(const char* path)
{
	_fullpath
		(fullpath_buf
		,path
		,DEFAULT_FILEPATH
		);

	return fullpath_buf;
}

void CFilePathUtil::AddName(const char *adding)
{
	strcat(fname,adding);
}

const char * CFilePathUtil::CutCWD(const char *path)
{
	const char *fullpath = FullPath(path);
	char cwd[DEFAULT_FILEPATH];
	_getcwd(cwd,DEFAULT_FILEPATH);
	strcat(cwd,"\\");
	int slen = strlen(fullpath);


	int i,idx;
	idx = 0;

	// skipping
	for(i=0;i<slen;++i)
	{
		int src = toupper(fullpath[i]);
		int dest = toupper(cwd[i]);

		if (src!=dest)
			break;
	}

	// copying
	idx = 0;
	for(;i<slen;++i)
	{
		if( fullpath[i] == '\\')
			buf[idx] = '/';
		else
			buf[idx] = fullpath[i];
		idx++;
	}

	buf[idx] = 0;

	return buf;
}


BOOL CFilePathUtil::ExtractSpotRightBuffer_Descending(	const char *szFullFileName,
														char *szReturnBuffer,
														char **szFindValues,
														const int iValueCnt,
														BOOL bExceptionSpot )
{
	int		iSpot, iLen, iCur, iCurLen;
	BOOL	bFind = FALSE;

	iLen = strlen( szFullFileName );
	int i = 0;
	for( i = iLen; i >= 0; --i )
	{
		for( int j = 0; j < iValueCnt; ++j )
		{
			iCurLen = strlen( szFindValues[j] );
			if( strncmp(&szFullFileName[i],szFindValues[j],iCurLen) == 0 )
			{
				bFind = TRUE;
				break;
			}
		}

		if( TRUE == bFind ) break;
	}

	if( i < 0 ) return FALSE;
	if( FALSE == bFind ) return FALSE;

	iSpot = (TRUE==bExceptionSpot)?i + iCurLen : i;

	for( int i = iSpot; i < iLen; ++i )
	{
		iCur = i - iSpot;

		szReturnBuffer[iCur] = szFullFileName[i];
	}

	szReturnBuffer[iCur+1] = '\0';

	return TRUE;
}


BOOL CFilePathUtil::ExtractSpotLeftBuffer_Descending(	const char	*szFullFileName,
														char			*szReturnBuffer,
														char			**szFindValues,
														const int		iValueCnt,
														const int		iSkipDegree,
														BOOL			bExceptionSpot )
{
	int		iSpot, 
		iLen, 
		iStrLen = 0, 
		iStrCurLen = 0, 
		iFindValueIdx = 0, 
		iSkipDegree_ = iSkipDegree;
	BOOL	bFind = FALSE;
	BOOL	bFindValue_OneTimeMore = FALSE;

	iLen = strlen( szFullFileName );

	int i = 0;
	for( i = iLen; i >= 0; --i )
	{
		bFind = FALSE;
		if( FALSE == bFindValue_OneTimeMore )
		{
			for( int j = 0; j < iValueCnt; ++j )
			{
				iStrCurLen = strlen( szFindValues[j] );
				if( (strncmp( &szFullFileName[i], szFindValues[j], iStrCurLen ) == 0) && iStrCurLen > iStrLen )
				{
					iFindValueIdx = j;
					iStrLen = iStrCurLen;
					bFind = TRUE;
					bFindValue_OneTimeMore = TRUE;
				}
			}
		}
		else
		{
			if( (strncmp( &szFullFileName[i], szFindValues[iFindValueIdx], iStrLen ) == 0) )
			{
				bFind = TRUE;
			}
		}

		if( TRUE == bFind)
		{
			if( 1 > iSkipDegree_-- )	
			{
				break;
			}
		}
	}

	if( i < 0 ) return FALSE;

	iSpot = (TRUE==bExceptionSpot)? i : i + iStrLen;

	for( int i = 0; i < iSpot; ++i )
	{
		szReturnBuffer[i] = szFullFileName[i];
	}

	szReturnBuffer[iSpot] = '\0';

	return TRUE;
}

#include <stdio.h>

void CFilePathUtil::PullStr( char* szBuffer, UINT uiStartPos )
{
	if( !szBuffer ) return;
	if( uiStartPos == 0 ) return;

	char		szTempBuf[DEFAULT_FILEPATH];
	int			iLen, i = 0;

	strcpy( szTempBuf, szBuffer );

	iLen = strlen( szBuffer );
	memset( szBuffer, 0, sizeof(char) * iLen );

	while( szTempBuf[i] != '\0' )
	{
		szBuffer[i] = szTempBuf[i + uiStartPos];
		++i;
	}

	szBuffer[i] = '\0';
}


BOOL CFilePathUtil::FindStr( const char *szSrcBuffer, const char *szFindBuffer )
{
	UINT iLen, iFindLen;

	iLen = strlen( szSrcBuffer );
	iFindLen = strlen( szFindBuffer );

	for( UINT ui = 0; ui < iLen; ++ui )
	{
		if( 0 == strncmp( &szSrcBuffer[ui], szFindBuffer, iFindLen ) )
		{
			return TRUE;
		}
	}

	return FALSE;
}


int CFilePathUtil::ExtractDirPath( const char *szFullFileName, 
	char *szReturnDirPath,
	UINT uiSkipPathDegree )
{
	int			i = 0;
	char		**szFindValues = 0;

	szFindValues = new char*[8];

	for( int i_ = 0; i_ < 8; ++i_ )
	{
		szFindValues[i_] = new char[4];
		memset( szFindValues[i_], 0, sizeof(char) * 4 );
	}

	char		*szDoubleQuotation = "\\\\";
	char		*szSingleQuotation = "\\";

	if( TRUE == FindStr(szFullFileName, szDoubleQuotation ) )
	{
		sprintf( szFindValues[0], "%s", szDoubleQuotation);
	}
	else
	{
		sprintf( szFindValues[0], "%s", szSingleQuotation );
	}

	sprintf( szFindValues[1], "%s", TEXT("/") );

	ExtractSpotLeftBuffer_Descending( szFullFileName, 
		szReturnDirPath, 
		szFindValues, 
		2,
		uiSkipPathDegree, FALSE );

	for( int i_ = 0; i_ < 8; ++i_ )
	{
		SAFEDELETE_ARRAY( szFindValues[i_] );
	}

	SAFEDELETE_ARRAY( szFindValues );

	return i;
}


BOOL CFilePathUtil::ExtractFileExceptEx_( const char* szFullFileName,
	char* szReturnBuffer )
{
	char		**szFindValues;
	BOOL		bReturn = FALSE;

	szFindValues = new char*[1];
	szFindValues[0] = new char[2];
	memset( szFindValues[0], 0, sizeof( char ) * 2 );
	sprintf( szFindValues[0], "%s", TEXT(".") );

	bReturn = ExtractSpotLeftBuffer_Descending( szFullFileName, 
		szReturnBuffer, 
		szFindValues, 
		1,
		0, TRUE );

	SAFEDELETE_ARRAY( szFindValues[0] );
	SAFEDELETE_ARRAY( szFindValues );

	return bReturn;
}


BOOL CFilePathUtil::ExtractFileName_(const char* szFullFileName, 
	char* szReturnBuffer )
{
	char		**szFindValues = 0;
	BOOL		bReturn = FALSE;

	szFindValues = new char*[3];

	for( UINT ui = 0; ui < 3; ++ui )
	{
		szFindValues[ui] = new char[4];
		memset( szFindValues[ui], 0, sizeof(char) * 4 );
	}

	char		*szDoubleQuotation = "\\\\";
	char		*szSingleQuotation = "\\";

	if( TRUE == FindStr(szFullFileName, szDoubleQuotation ) )
	{
		sprintf( szFindValues[0], "%s", szDoubleQuotation);
	}
	else
	{
		sprintf( szFindValues[0], "%s", szSingleQuotation );
	}

	sprintf( szFindValues[1], "%s", TEXT("/") );

	bReturn = ExtractSpotRightBuffer_Descending( szFullFileName, szReturnBuffer, szFindValues, 2);

	for( UINT ui = 0; ui < 3; ++ui )
	{
		SAFEDELETE_ARRAY( szFindValues[ui] );
	}

	SAFEDELETE_ARRAY( szFindValues );

	return bReturn;
}


BOOL CFilePathUtil::ExtractFileName(const char* szFullFileName, 
	char* szReturnBuffer )
{
	char		**szFindValues = 0;
	char		szTempBuf[DEFAULT_FILEPATH];
	BOOL		bReturn = FALSE;

	szFindValues = new char*[3];

	for( UINT ui = 0; ui < 3; ++ui )
	{
		szFindValues[ui] = new char[4];
		memset( szFindValues[ui], 0, sizeof(char) * 4 );
	}

	char		*szDoubleQuotation = "\\\\";
	char		*szSingleQuotation = "\\";

	if( TRUE == FindStr(szFullFileName, szDoubleQuotation ) )
	{
		sprintf( szFindValues[0], "%s", szDoubleQuotation);
	}
	else
	{
		sprintf( szFindValues[0], "%s", szSingleQuotation );
	}

	sprintf( szFindValues[1], "%s", TEXT("/") );

	ExtractSpotRightBuffer_Descending( szFullFileName, szTempBuf, szFindValues, 2 );

	for( UINT ui = 0; ui < 3; ++ui )
	{
		SAFEDELETE_ARRAY( szFindValues[ui] );
	}

	SAFEDELETE_ARRAY( szFindValues );

	return ExtractFileExceptEx_( szTempBuf, szReturnBuffer );
}


BOOL CFilePathUtil::ExtractFileEx_( const char* szFullFileName,
	char* szReturnBuffer )
{
	char		**szFindValues = 0;
	char		szTemp_[DEFAULT_FILEPATH];

	BOOL		bFind = FALSE;

	szFindValues = new char*[1];
	szFindValues[0] = new char[2];
	memset( szFindValues[0], 0, sizeof( char ) * 2 );
	sprintf( szFindValues[0], "%s", TEXT(".") );

	bFind = ExtractSpotRightBuffer_Descending( szFullFileName, szTemp_, szFindValues, 1 );

	SAFEDELETE_ARRAY( szFindValues[0] );
	SAFEDELETE_ARRAY( szFindValues );

	if( FALSE == bFind )
	{
		return FALSE;
	}
	else
	{
		if( DEFAULT_MAX_FILEEX < strlen(szTemp_) )
		{
			return FALSE;
		}
		else
		{
			strcpy( szReturnBuffer, szTemp_ );
		}
	}

	return TRUE;
}

BOOL CFilePathUtil::ConvertValues( const char* szOriginalBuffer,
									char* szReturnBuffer,
									const char* szOriginValue,
									UINT iOriginLen,
									const char* szConvertValue,
									UINT iConvertLen )
{
	int		iSpotCnt = 0, 
		iLen = 0, 
		iCurDst = 0,
		iCurSrc = 0,
		iBuf = 0,
		iIvsOffsetCnt = 0;

	BOOL	bFind = FALSE;

	char	szOriginalBuffer_[DEFAULT_FILEPATH];

	char	**ppszSplit = 0;

	strcpy( szOriginalBuffer_, szOriginalBuffer );

	iLen = strlen( szOriginalBuffer );

	for( int i = 0; i < iLen; ++i )
	{		
		if( 0 < iIvsOffsetCnt )
		{
			--iIvsOffsetCnt;
		}

		if( 1 > iIvsOffsetCnt )
		{
			char	*szCurOrigin = &szOriginalBuffer_[i];
			if( 0 == strncmp( szCurOrigin, szOriginValue, iOriginLen ) )
			{
				++iSpotCnt;

				for( UINT j = 0; j < iConvertLen; ++j )
				{
					szReturnBuffer[iCurDst++] = szConvertValue[j];
				}

				iIvsOffsetCnt = iOriginLen;
			}
			else
			{
				szReturnBuffer[iCurDst++] = szOriginalBuffer_[i];
			}
		}
	}

	szReturnBuffer[iCurDst] = 0;
	//szReturnBuffer[iCurDst] = '\0';

	OutputDebugString( szReturnBuffer );

	return TRUE;
}


BOOL CFilePathUtil::ConvertToDoubleQuotationPath( const char* szFileName,
	char* szReturnBuffer )
{
	char	szSingleQuotation[4], szDoubleQuotation[4];

	memset( szDoubleQuotation, 0, sizeof(char) * 4 );
	memset( szSingleQuotation, 0, sizeof(char) * 4 );

	sprintf( szSingleQuotation, "%s", TEXT("\\") );

	sprintf( szDoubleQuotation, "%s%s", TEXT("\\") ,TEXT("\\") );

	return ConvertValues( szFileName, szReturnBuffer, szSingleQuotation,1, szDoubleQuotation,2 );
}


BOOL CFilePathUtil::ConvertToSingleQuotationPath( const char* szFileName,
	char* szReturnBuffer )
{
	char	szSingleQuotation[4], szDoubleQuotation[4];

	memset( szDoubleQuotation, 0, sizeof(char) * 4 );
	memset( szSingleQuotation, 0, sizeof(char) * 4 );

	sprintf( szSingleQuotation, "%s", TEXT("\\") );

	sprintf( szDoubleQuotation, "%s%s", TEXT("\\") ,TEXT("\\") );

	return ConvertValues( szFileName, szReturnBuffer, szDoubleQuotation,2, szSingleQuotation,1 );
}


