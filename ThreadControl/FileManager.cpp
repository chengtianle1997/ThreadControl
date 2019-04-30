#include "FileManager.h"
#include "afxwin.h"

int FileManager::CreateFolder(const char* filepath, char* finalpath, unsigned char *SerialNum)
{

	BOOL ret;

	sprintf(finalpath, "%s\\Camera%s_Image", filepath, SerialNum);

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