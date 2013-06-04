#include <stdlib.h>
#include "Common.h"
#include "RandomWell512.h"
#include "BitmapTexture.h"
#include "VarianceHelper.h"

int main()
{
	const UINT colorRange = 255;
	int textureResolution = -1;
	enum RANDOMTYPE { RANDOMTYPE_INTERNAL, RANDOMTYPE_WELL512, RANDOMTYPE_COUNT };

	string randomNameTable[] = {"RANDOMTYPE_INTERNAL", "RANDOMTYPE_WELL512"};
	const int countRandomType = static_cast<int>(RANDOMTYPE_COUNT);

	RandomWell512 randomWell512;
	CBitmapTexture *bitmapTable = new CBitmapTexture[countRandomType];
	string* savedFileTable = new string[countRandomType];
	CVarianceHelper<UINT>* varianceTable = new CVarianceHelper<UINT>[countRandomType];

	string inputText = "";
	int resolution = -1;
	while(1)
	{
		cout << "\nInput texture resolution(1~2048):";
		cin >> inputText;
		resolution = atoi(inputText.c_str());
		if(0<resolution && 2048>=resolution)
		{
			textureResolution = resolution;
			break;
		}
		else
		{
			cout << inputText << " is not a valid resolution range.\n";
		}
	}

	for(int irand = 0; irand < countRandomType; ++irand)
	{
		RANDOMTYPE randomtype = static_cast<RANDOMTYPE>(irand);

		CBitmapTexture& bitmaptexture = bitmapTable[irand];
		CVarianceHelper<UINT>& variance = varianceTable[irand];

		bitmaptexture.Initialize( randomNameTable[irand].c_str(), textureResolution );
		variance.Initialize(bitmaptexture.GetBufferSize());

		UINT color = 0;

		switch(randomtype)
		{
		case RANDOMTYPE_INTERNAL:
			{
				for(UINT ibuf = 0; ibuf < bitmaptexture.GetBufferSize(); ++ibuf)
				{
					color = rand() % (colorRange + 1);
					bitmaptexture.SetColor(ibuf, CBitmapTexture::SET_COLOR_ARGB(0, 0, color, 0));
					variance.SetData(ibuf, color);
				}
			}
			break;
		case RANDOMTYPE_WELL512:
			{
				for(UINT ibuf = 0; ibuf < bitmaptexture.GetBufferSize(); ++ibuf)
				{
					color = randomWell512.GetValue(0, colorRange + 1);
					bitmaptexture.SetColor(ibuf, CBitmapTexture::SET_COLOR_ARGB(0, 0, color, 0));
					variance.SetData(ibuf, color);
				}
			}
			break;
		}

		bitmaptexture.SaveBitmap();
		savedFileTable[irand] = bitmaptexture.GetFileName();

		bitmaptexture.Release();

		variance.Calculate();
	}

	char szFilePath[2048];
	char szCurrentDirectory[1024];
	GetCurrentDirectory(1024, szCurrentDirectory);
	string textMeasure = "";
	for(int i=0; i<countRandomType; ++i)
	{
		CVarianceHelper<UINT> &variance = varianceTable[i];
		string text(savedFileTable[i] + " \n : avg(0~"
			+std::to_string(static_cast<LONGLONG>(colorRange))+")("
			+ std::to_string(static_cast<LONGLONG>(variance.getAverage())) 
			+ "),variance(" 
			+ std::to_string(static_cast<LONGLONG>(variance.getVariance()))+"),StandardDeviation(" 
			+ std::to_string(static_cast<LONGLONG>(variance.getStandardDeviation()))+"),CountOf'0'("
			+ std::to_string(static_cast<LONGLONG>(variance.getCountData(colorRange))) +")" +  "\n");
		textMeasure.append(text);
	}

	string textSuccessed("Open saved texture file(" 
		+ std::to_string(static_cast<LONGLONG>(countRandomType)) 
		+ "EA)?\n" + textMeasure		
		+ "\n"
		+ szCurrentDirectory + "\\");

	if(IDYES == MessageBox(NULL, (textSuccessed.c_str()), "Saved BitmapTexture.", MB_YESNO))
	{
		for(int i = 0; i < countRandomType; ++i)
		{
			sprintf_s(szFilePath, 2048, "%s\\%s", szCurrentDirectory, savedFileTable[i].c_str() );
			ShellExecute(NULL, "open", szFilePath, NULL, NULL, SW_SHOWDEFAULT);
		}
		
		ShellExecute(NULL, "open", szCurrentDirectory, NULL, NULL, SW_SHOWDEFAULT);
	}

	delete[] bitmapTable;
	delete[] savedFileTable;
	delete[] varianceTable;

	return EXIT_SUCCESS;
}
