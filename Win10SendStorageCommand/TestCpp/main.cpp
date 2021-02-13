#include <Windows.h>
#include <Setupapi.h>

#pragma comment( lib, "setupapi.lib" )

#include <iostream>
#include <string>
using namespace std;

int main(void)
{
    HDEVINFO diskClassDevices;
    GUID diskClassDeviceInterfaceGuid = GUID_DEVINTERFACE_DISK;
    DWORD deviceIndex = 0;
    DWORD requiredSize;
    HANDLE disk = INVALID_HANDLE_VALUE;
    STORAGE_DEVICE_NUMBER diskNumber;
    DWORD bytesReturned;

    // 디스크 클래스에 해당하는 Handle을 가져온다. 
    diskClassDevices = SetupDiGetClassDevs(&diskClassDeviceInterfaceGuid,
        NULL,
        NULL,
        DIGCF_PRESENT |
        DIGCF_DEVICEINTERFACE);
  
    SP_DEVICE_INTERFACE_DATA deviceInterfaceData;

    ZeroMemory(&deviceInterfaceData, sizeof(SP_DEVICE_INTERFACE_DATA));
    deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
    deviceIndex = 0;


    PSP_DEVICE_INTERFACE_DETAIL_DATA deviceInterfaceDetailData;
    while (SetupDiEnumDeviceInterfaces(diskClassDevices,
        NULL,
        &diskClassDeviceInterfaceGuid,
        deviceIndex,
        &deviceInterfaceData))
    {
        ++deviceIndex;

        SetupDiGetDeviceInterfaceDetail(diskClassDevices,
            &deviceInterfaceData,
            NULL,
            0,
            &requiredSize,
            NULL);

        deviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(requiredSize);
        if (deviceInterfaceDetailData == NULL)
        {
            std::cout << "malloc error!" << std::endl;
            break;
        }

        ZeroMemory(deviceInterfaceDetailData, requiredSize);
        deviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

        SetupDiGetDeviceInterfaceDetail(diskClassDevices,
            &deviceInterfaceData,
            deviceInterfaceDetailData,
            requiredSize,
            NULL,
            NULL);

        disk = CreateFile(deviceInterfaceDetailData->DevicePath,
            GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

        DeviceIoControl(disk,
            IOCTL_STORAGE_GET_DEVICE_NUMBER,
            NULL,
            0,
            &diskNumber,
            sizeof(STORAGE_DEVICE_NUMBER),
            &bytesReturned,
            NULL);

        CloseHandle(disk);
        disk = INVALID_HANDLE_VALUE;

        cout << deviceInterfaceDetailData->DevicePath << endl;
        cout << "\\\\?\\PhysicalDrive" << diskNumber.DeviceNumber << endl;
        cout << "Device Type : " << diskNumber.DeviceType << endl;
        cout << endl;
    }

    if (INVALID_HANDLE_VALUE != diskClassDevices) {
        SetupDiDestroyDeviceInfoList(diskClassDevices);
    }

	return 0;
}