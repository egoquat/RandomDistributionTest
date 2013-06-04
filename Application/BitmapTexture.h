#pragma once
#include <stdio.h>
#include <string>
#include "FilePathUtil.h"

#define MAXIMUM_BITMAP_SQRT 2048
#define MAXIMUM_BITMAP_BUFFER MAXIMUM_BITMAP_SQRT*MAXIMUM_BITMAP_SQRT

using namespace std;

// per 1 bitmap texture instance.
class CBitmapTexture
{
private:
	//# Member variances
	CFilePathUtil		*m_pFilePathUtil;

	char		_szBmpFileName[512];
	UINT		_uiWidth, _uiHeight, _uiBufferSize, _uiBitmapBufferSize;

	BYTE*		_arbyColor;
	UINT*		_aruiColor;

	BOOL		_filledUpByteColor;
	BOOL		_filledUpUintColor;

public:
	//# Utility Global Functions.
	// Color  0xAARRGGBB
	static UINT GET_COLOR_A(UINT CO) { return ((CO>>24)&0xff); }
	static UINT GET_COLOR_R(UINT CO) { return  ((CO>>16)&0xff); }
	static UINT GET_COLOR_G(UINT CO) { return  ((CO>>8)&0xff); }
	static UINT GET_COLOR_B(UINT CO) { return  ((CO)&0xff); }
	static UINT SET_COLOR_ARGB(UINT a, UINT r, UINT g ,UINT b) 
	{
		return ((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)));
	}

	static UINT SET_COLOR_RGBA(UINT r, UINT g, UINT b,UINT a) 
	{	
		return SET_COLOR_ARGB(a,r,g,b);
	}

private:
	BOOL SetWidth(UINT width) 
	{
		BOOL validation = width <= MAXIMUM_BITMAP_SQRT && width > 0;
		assert(validation);
		_uiWidth = width;
		return validation;
	}

	BOOL SetHeight(UINT height)
	{
		BOOL validation = height <= MAXIMUM_BITMAP_SQRT && height > 0;
		assert(validation);
		_uiHeight = height;
		return validation;
	}

	BOOL SetBufferSize(UINT bufferSize)
	{
		BOOL validation = bufferSize <= MAXIMUM_BITMAP_BUFFER && bufferSize > 0;
		assert(validation);
		_uiBufferSize = bufferSize;
		_uiBitmapBufferSize = bufferSize * 3;
		return validation;
	}

	BOOL SetResolution(UINT width, UINT height)
	{
		return SetWidth(width) && SetHeight(height) && SetBufferSize(width * height);
	}

	BOOL SetColorToBuffer(UINT index, UINT Color, BYTE* byteColors_out)
	{
		UINT bufferIndex = index * 3;
		byteColors_out[bufferIndex] = GET_COLOR_B(Color);
		byteColors_out[bufferIndex+1] = GET_COLOR_G(Color);
		byteColors_out[bufferIndex+2] = GET_COLOR_R(Color);
		return TRUE;
	}

	//# Private Functions
	BOOL SaveBitmap(char* bitmapFileName, BYTE *arby1DColors, UINT uiWidth, int uiHeight);
	void ConvertARGBToBitmap( const UINT *aruiSrcColor, UINT uiWidth, UINT uiHeight,BYTE *arbyDstColor );
	void NamingBitmapFile( const char* fileName_in, char* fileName_out )
	{
		char		szFileEx[8];

		m_pFilePathUtil = m_pFilePathUtil->GetThis();
		if( TRUE == m_pFilePathUtil->ExtractFileEx_( fileName_in, szFileEx ) )
		{
			if( 0 != strcmp( strlwr(szFileEx), TEXT("bmp") ) )
			{
				sprintf( fileName_out, "%s.bmp", fileName_in );
			}
			else
			{
				strcpy( fileName_out, fileName_in );
			}
		}
		else
		{
			sprintf( fileName_out, "%s.bmp", fileName_in );
		}
	}


	BOOL FillupArgbToBitmap(const UINT* argb, UINT width, UINT height, BYTE* buffer_out)
	{
		BYTE		a, r, g, b;	a=r=g=b=0;
		UINT		uiCur = 0, uiByteIdx = 0;
		UINT		SrcCo = 0;

		if(FALSE==ISNULL(buffer_out)) { SAFEDELETE_ARRAY(buffer_out); }
		if(TRUE==ISNULL(argb)) {	assert(0); return FALSE;}
		buffer_out = new BYTE[_uiBitmapBufferSize];

		for( UINT uiX = 0; uiX < width; ++uiX )
		{
			for( UINT uiY = 0; uiY < height; ++uiY )
			{
				uiCur = ( uiY * width ) + uiX;

				SrcCo = argb[uiCur];
				SetColorToBuffer(uiCur, SrcCo, buffer_out);
			}
		}

		_filledUpByteColor = TRUE;

		return TRUE;
	}

public:
	const char* GetFileName() { return _szBmpFileName; }
	UINT GetWidth() {  return _uiWidth; }
	UINT GetHeight() { return _uiHeight; }
	UINT GetBufferSize() { return _uiBufferSize; }
	BOOL SetColor(UINT index, UINT Color)
	{
		BOOL validation = index < GetBufferSize() && index >= 0 && _arbyColor != NULL;
		assert(validation);
		if(FALSE == validation) return FALSE;

		SetColorToBuffer(index, Color, _arbyColor);
		return TRUE;
	}

	BOOL Initialize( const char *szFileName, UINT *Colors, UINT uiWidth, UINT uiHeight );
	BOOL Initialize( const char *szFileName, UINT *Colors, UINT uiResolution );
	BOOL Initialize( const char *szFileName, UINT uiWidth, UINT uiHeight );
	BOOL Initialize( const char *szFileName, UINT uiResolution );

	void SetARGBColorSets(UINT *Colors)
	{
		FillupArgbToBitmap(Colors, _uiWidth, _uiHeight, _arbyColor);
	}

	void SetBitmapColorSets(BYTE *Colors)
	{
		if(ISNULL(_arbyColor)==FALSE) { SAFEDELETE_ARRAY(_arbyColor); }
		int sizeColor = SIZEOF_ARRAY(Colors);
		memcpy(_arbyColor, Colors, sizeColor);
		_filledUpByteColor = TRUE;
	}

	BOOL SaveBitmap();

	BOOL Release();
public:
	CBitmapTexture(void);
	~CBitmapTexture(void);
};


