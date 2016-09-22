/*
	Declarations for memtest driver
	Copyright (C) 1999, 2000 by Walter Oney
*/


#ifndef __Driver_h__
#define __Driver_h__
#include "generic.h"

#define DRIVERNAME "MEMTEST"				// for use in messages
#define LDRIVERNAME L"MEMTEST"				// for use in UNICODE string constants


// Device extension structure

typedef struct _DEVICE_EXTENSION 
{
	PDEVICE_OBJECT DeviceObject;			// device object this extension belongs to
	PDEVICE_OBJECT LowerDeviceObject;		// next lower driver in same stack
	PDEVICE_OBJECT Pdo;						// the PDO
	IO_REMOVE_LOCK RemoveLock;				// removal control locking structure
	UNICODE_STRING devname;
	PGENERIC_EXTENSION pgx;					// device extension for GENERIC.SYS
	LONG handles;							// # open handles
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;




// Global functions

VOID RemoveDevice(PDEVICE_OBJECT fdo);
NTSTATUS CompleteRequest(PIRP Irp, NTSTATUS status, ULONG_PTR info);
NTSTATUS StartDevice(PDEVICE_OBJECT fdo, PCM_PARTIAL_RESOURCE_LIST raw, PCM_PARTIAL_RESOURCE_LIST translated);
VOID StopDevice(PDEVICE_OBJECT fdo, BOOLEAN oktouch = FALSE);

// I/O request handlers

NTSTATUS DispatchCreate(PDEVICE_OBJECT fdo, PIRP Irp);
NTSTATUS DispatchClose(PDEVICE_OBJECT fdo, PIRP Irp);
NTSTATUS DispatchControl(PDEVICE_OBJECT fdo, PIRP Irp);
NTSTATUS DispatchPower(PDEVICE_OBJECT fdo, PIRP Irp);
NTSTATUS DispatchWmi(PDEVICE_OBJECT fdo, PIRP Irp);
NTSTATUS DispatchPnp(PDEVICE_OBJECT fdo, PIRP Irp);

extern BOOLEAN win98;
extern UNICODE_STRING servkey;

#endif // __Driver_h__
