#include "CameraSDK.h"

//Get Device List
int Camera::GetDevList()
{
	//��ȡ�豸ö���б�
	memset(&stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
	nRet = MV_CC_EnumDevices(MV_USB_DEVICE, &stDevList);

	if (MV_OK != nRet)
	{
		printf("Enum Devices fail! nRet [0x%x]\n", nRet);
		return nRet;
	}

	if (stDevList.nDeviceNum > 0)
	{
		for (unsigned int i = 0; i < stDevList.nDeviceNum; i++)
		{
			printf("[device %d]:\n", i);
			//�豸��Ϣ
			MV_CC_DEVICE_INFO* pDeviceInfo = stDevList.pDeviceInfo[i];
			if (NULL == pDeviceInfo)
			{
				break;
				return -1;
			}
			PrintDeviceInfo(pDeviceInfo);
		}
	}
	else
	{
		printf("Find No Devices!\n");
		return -1;
	}
	return MV_OK;
}

//Open Camera 
int Camera::OpenCamera(CameraInitParam &camerainitparam)
{
	if (camerainitparam.devNum >= stDevList.nDeviceNum)
	{
		printf("Intput error!\n");
	}
	//ѡ���豸���������
	nRet = MV_CC_CreateHandle(&handle, stDevList.pDeviceInfo[camerainitparam.devNum]);
	if (MV_OK != nRet)
	{
		printf("Create Handle fail! nRet [0x%x]\n", nRet);
	}
	nRet = MV_CC_OpenDevice(handle);
	if (MV_OK != nRet)
	{
		printf("Open Device fail! nRet [0x%x]\n", nRet);
	}
	else {
		printf("Device is ready!\n");
	}

	//���ô���ģʽΪoff
	nRet = MV_CC_SetEnumValue(handle, "TriggerMode", 0);
	if (MV_OK != nRet)
	{
		printf("Set Trigger Mode fail! nRet [0x%x]\n", nRet);
	}

	//��ȡ���ݰ���С
	MVCC_INTVALUE stParam;
	memset(&stParam, 0, sizeof(MVCC_INTVALUE));
	nRet = MV_CC_GetIntValue(handle, "PayloadSize", &stParam);
	if (MV_OK != nRet)
	{
		printf("Get PayloadSize fail! nRet [0x%x]\n", nRet);
	}
	g_nPayloadSize = stParam.nCurValue;

	//Get the width and Height of the Camera
	MVCC_INTVALUE pIntValue;
	memset(&pIntValue, 0, sizeof(MVCC_INTVALUE));
	MV_CC_GetIntValue(handle, "Width", &pIntValue);
	camerainitparam.in_w = pIntValue.nCurValue;
	MV_CC_GetIntValue(handle, "Height", &pIntValue);
	camerainitparam.in_h = pIntValue.nCurValue;
}

//Set ExposureTime 
int Camera::SetExposureTime(float newExposureTime) 
{
	nRet = MV_CC_SetFloatValue(handle, "ExposureTime", newExposureTime);
	if (MV_OK != nRet)
	{
		printf("Set ExposureTime fail! nRet [0x%x]\n", nRet);
		return nRet;
	}
	return nRet;
}

//Set AcquisitionFrameRate
int Camera::SetAcquisitionFrameRate(float newAcquisitionFrameRate) 
{
	nRet = MV_CC_SetFloatValue(handle, "AcquisitionFrameRate", newAcquisitionFrameRate);
	if (MV_OK != nRet)
	{
		printf("Set AcquisitionFrameRate fail! nRet [0x%x]\n", nRet);
		return nRet;
	}
	return nRet;
}

//Set Gain
int Camera::SetGain(float newGain) 
{
	nRet = MV_CC_SetFloatValue(handle, "Gain", newGain);
	if (MV_OK != nRet)
	{
		printf("Set Gain fail! nRet [0x%x]\n", nRet);
		return nRet;
	}
	return nRet;
}

//Start Grabbing
int Camera::StartGrabbing() 
{
	nRet = MV_CC_StartGrabbing(handle);
	if (MV_OK != nRet)
	{
		printf("Start Grabbing fail! nRet [0x%x]\n", nRet);
		return nRet;
	}
	memset(&stOutFrame, 0, sizeof(MV_FRAME_OUT));
	return nRet;
}

//Get Image
int Camera::GetImage(MV_FRAME_OUT &stOutFrame) {
	nRet = MV_CC_GetImageBuffer(handle, &stOutFrame, 1000);
	if (nRet == MV_OK)
	{
		return nRet;
	}
	else
	{
		printf("No data[0x%x]\n", nRet);
		return nRet;
	}
	if (NULL != stOutFrame.pBufAddr)
	{
		nRet = MV_CC_FreeImageBuffer(handle, &stOutFrame);
		if (nRet != MV_OK)
		{
			printf("Free Image Buffer fail! nRet [0x%x]\n", nRet);
		}
		return nRet;
	}

}

//Stop Grabing Image
int Camera::StopGrabbing()
{
	nRet = MV_CC_StopGrabbing(handle);
	if (MV_OK != nRet)
	{
		printf("Stop Grabbing fail! nRet [0x%x]\n", nRet);
		return nRet;
	}
	return nRet;
}

//Close Device
int Camera::CloseDevice()
{
	nRet = MV_CC_CloseDevice(handle);
	if (MV_OK != nRet)
	{
		printf("ClosDevice fail! nRet [0x%x]\n", nRet);
		return nRet;
	}
	nRet = MV_CC_DestroyHandle(handle);
	if (MV_OK != nRet)
	{
		printf("Destroy Handle fail! nRet [0x%x]\n", nRet);
		return nRet;
	}
	if (nRet != MV_OK)
	{
		if (handle != NULL)
		{
			MV_CC_DestroyHandle(handle);
			handle = NULL;
		}
		return nRet;
	}
	return nRet;
}
