#include <stdlib.h>
#include "Common.h"
#include "RandomWell512.h"
#include "BitmapTexture.h"

int main()
{
	int textureResolution = -1;
	enum RANDOMTYPE { RANDOMTYPE_INTERNAL, RANDOMTYPE_WELL512, RANDOMTYPE_COUNT };

	string randomNameTable[] = {"RANDOMTYPE_INTERNAL", "RANDOMTYPE_WELL512"};
	const int countRandomType = static_cast<int>(RANDOMTYPE_COUNT);

	RandomWell512 randomWell512;
	CBitmapTexture *bitmapTable = new CBitmapTexture[countRandomType];
	string* savedFileTable = new string[countRandomType];

	string inputText = "";
	int inputTextureResolution = -1;
	while(1)
	{
		cout << "\nInput texture resolution(1~2048):";
		cin >> inputText;
		inputTextureResolution = atoi(inputText.c_str());
		if(0<inputTextureResolution && 2048>=inputTextureResolution)
		{
			textureResolution = inputTextureResolution;
			break;
		}
		else
		{
			cout << inputText << " is not a valid resolution range.\n";
		}
	}

	for(int irandom = 0; irandom < countRandomType; ++irandom)
	{
		RANDOMTYPE randomtype = static_cast<RANDOMTYPE>(irandom);
		CBitmapTexture &bitmaptexture = bitmapTable[irandom];
		bitmaptexture.Initialize( randomNameTable[irandom].c_str(), textureResolution );

		UINT colorRandom = 0;

		switch(randomtype)
		{
		case RANDOMTYPE_INTERNAL:
			{
				for(UINT ibuf = 0; ibuf < bitmaptexture.GetBufferSize(); ++ibuf)
				{
					colorRandom = rand() % 255;
					bitmaptexture.SetColor(ibuf, CBitmapTexture::SET_COLOR_ARGB(0, colorRandom, 0, 0));
				}
			}
			break;
		case RANDOMTYPE_WELL512:
			{
				for(UINT ibuf = 0; ibuf < bitmaptexture.GetBufferSize(); ++ibuf)
				{
					colorRandom = randomWell512.GetValue(0, 255);
					bitmaptexture.SetColor(ibuf, CBitmapTexture::SET_COLOR_ARGB(0, colorRandom, 0, 0));
				}
			}
			break;
		}

		bitmaptexture.SaveBitmap();
		savedFileTable[irandom] = bitmaptexture.GetFileName();

		bitmaptexture.Release();
	}


	char szFilePath[2048];
	char szCurrentDirectory[1024];
	GetCurrentDirectory(1024, szCurrentDirectory);

	string textSuccessed("Open saved texture file? (" 
		+ std::to_string(static_cast<LONGLONG>(countRandomType)) 
		+ "EA)\n" + szCurrentDirectory + "\\");

	if(IDYES == MessageBox(NULL, (textSuccessed.c_str()), "Saved BitmapTexture.", MB_YESNO))
	{
		for(int i = 0; i < countRandomType; ++i)
		{
			sprintf_s(szFilePath, 2048, "%s\\%s", szCurrentDirectory, savedFileTable[i].c_str() );
			ShellExecute(NULL, "open", szFilePath, NULL, NULL, SW_SHOWDEFAULT);
		}
		
		ShellExecute(NULL, "open", szCurrentDirectory, NULL, NULL, SW_SHOWDEFAULT);
	}

	return EXIT_SUCCESS;
}
