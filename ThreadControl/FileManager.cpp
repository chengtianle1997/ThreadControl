#include "FileManager.h"
#include "afxwin.h"

int FileManager::CreateFolder(const char* filepath, char* finalpath, int Devnum)
{

	BOOL ret;

	sprintf(finalpath, "%s\\Camera%d_Image", filepath, Devnum);

	ret = CreateDirectory(filepath, NULL);
	if (!ret)
	{
		printf("Create root-directory failed!");
	}
	ret = CreateDirectory(finalpath, NULL);
	if (!ret)
	{
		printf("Create sub-directory failed!");
	}

	return ret;
}