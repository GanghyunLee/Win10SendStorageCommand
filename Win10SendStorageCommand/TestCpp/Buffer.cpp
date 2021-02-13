/*
#include <Windows.h>
#include <Setupapi.h>
#include <Ntddstor.h>

#pragma comment( lib, "setupapi.lib" )

#include <iostream>
#include <string>
using namespace std;

#define START_ERROR_CHK()           \
    DWORD error = ERROR_SUCCESS;    \
    DWORD failedLine;               \
    string failedApi;

#define CHK( expr, api )            \
    if ( !( expr ) ) {              \
        error = GetLastError( );    \
        failedLine = __LINE__;      \
        failedApi = ( api );        \
        goto Error_Exit;            \
    }

#define END_ERROR_CHK()             \
    error = ERROR_SUCCESS;          \
    Error_Exit:                     \
    if ( ERROR_SUCCESS != error ) { \
        cout << failedApi << " failed at " << failedLine << " : Error Code - " << error << endl;    \
    }

int main(int argc, char** argv) {

    HDEVINFO diskClassDevices;
    GUID diskClassDeviceInterfaceGuid = GUID_DEVINTERFACE_DISK;
    SP_DEVICE_INTERFACE_DATA deviceInterfaceData;
    PSP_DEVICE_INTERFACE_DETAIL_DATA deviceInterfaceDetailData;
    DWORD requiredSize;
    DWORD deviceIndex;

    HANDLE disk = INVALID_HANDLE_VALUE;
    STORAGE_DEVICE_NUMBER diskNumber;
    DWORD bytesReturned;

    START_ERROR_CHK();

    //
    // Get the handle to the device information set for installed
    // disk class devices. Returns only devices that are currently
    // present in the system and have an enabled disk device
    // interface.
    //
    diskClassDevices = SetupDiGetClassDevs(&diskClassDeviceInterfaceGuid,
        NULL,
        NULL,
        DIGCF_PRESENT |
        DIGCF_DEVICEINTERFACE);
    CHK(INVALID_HANDLE_VALUE != diskClassDevices,
        "SetupDiGetClassDevs");

    ZeroMemory(&deviceInterfaceData, sizeof(SP_DEVICE_INTERFACE_DATA));
    deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
    deviceIndex = 0;

    while (SetupDiEnumDeviceInterfaces(diskClassDevices,
        NULL,
        &diskClassDeviceInterfaceGuid,
        deviceIndex,
        &deviceInterfaceData)) {

        ++deviceIndex;

        SetupDiGetDeviceInterfaceDetail(diskClassDevices,
            &deviceInterfaceData,
            NULL,
            0,
            &requiredSize,
            NULL);
        CHK(ERROR_INSUFFICIENT_BUFFER == GetLastError(),
            "SetupDiGetDeviceInterfaceDetail - 1");

        deviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(requiredSize);
        CHK(NULL != deviceInterfaceDetailData,
            "malloc");

        ZeroMemory(deviceInterfaceDetailData, requiredSize);
        deviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

        CHK(SetupDiGetDeviceInterfaceDetail(diskClassDevices,
            &deviceInterfaceData,
            deviceInterfaceDetailData,
            requiredSize,
            NULL,
            NULL),
            "SetupDiGetDeviceInterfaceDetail - 2");

        disk = CreateFile(deviceInterfaceDetailData->DevicePath,
            GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
        CHK(INVALID_HANDLE_VALUE != disk,
            "CreateFile");

        CHK(DeviceIoControl(disk,
            IOCTL_STORAGE_GET_DEVICE_NUMBER,
            NULL,
            0,
            &diskNumber,
            sizeof(STORAGE_DEVICE_NUMBER),
            &bytesReturned,
            NULL),
            "IOCTL_STORAGE_GET_DEVICE_NUMBER");

        CloseHandle(disk);
        disk = INVALID_HANDLE_VALUE;

        cout << deviceInterfaceDetailData->DevicePath << endl;
        cout << "\\\\?\\PhysicalDrive" << diskNumber.DeviceNumber << endl;
        cout << endl;
    }
    CHK(ERROR_NO_MORE_ITEMS == GetLastError(),
        "SetupDiEnumDeviceInterfaces");

    END_ERROR_CHK();

Exit:

    if (INVALID_HANDLE_VALUE != diskClassDevices) {
        SetupDiDestroyDeviceInfoList(diskClassDevices);
    }

    if (INVALID_HANDLE_VALUE != disk) {
        CloseHandle(disk);
    }

    return error;
}
*/