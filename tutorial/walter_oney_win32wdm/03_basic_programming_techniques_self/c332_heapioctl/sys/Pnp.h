/*
	Plug 'n Play header file.
	Copyright (C) 1999, 2000 by Walter Oney.
*/



#ifndef __Pnp_h__
#define __Pnp_h__


#include "Driver.h"
#include "Device.h"
#include "Ioctl.h"
#include "Power.h"



GENERICAPI NTSTATUS	GENERIC_EXPORT GenericDeregisterInterface(PGENERIC_EXTENSION pdx, const GUID* guid);
GENERICAPI NTSTATUS GENERIC_EXPORT GenericDispatchPnp(PGENERIC_EXTENSION, PIRP);
GENERICAPI NTSTATUS GENERIC_EXPORT GenericEnableInterface(PGENERIC_EXTENSION pdx, const GUID* guid, BOOLEAN enable);
GENERICAPI PDEVICE_CAPABILITIES GENERIC_EXPORT GenericGetDeviceCapabilities(PGENERIC_EXTENSION pdx);
GENERICAPI NTSTATUS	GENERIC_EXPORT GenericRegisterInterface(PGENERIC_EXTENSION pdx, const GUID* guid);
GENERICAPI VOID		GENERIC_EXPORT GenericSetDeviceState(PGENERIC_EXTENSION pdx, PNP_DEVICE_STATE pnpstate);



NTSTATUS DefaultPnpHandler(PGENERIC_EXTENSION pdx, PIRP Irp);
NTSTATUS HandleCancelRemove(PGENERIC_EXTENSION pdx, PIRP Irp);
NTSTATUS HandleCancelStop(PGENERIC_EXTENSION pdx, PIRP Irp);
NTSTATUS HandleQueryCapabilities(PGENERIC_EXTENSION pdx, PIRP Irp);
NTSTATUS HandleQueryRemove(PGENERIC_EXTENSION pdx, PIRP Irp);
NTSTATUS HandleQueryState(PGENERIC_EXTENSION pdx, PIRP Irp);
NTSTATUS HandleQueryStop(PGENERIC_EXTENSION pdx, PIRP Irp);
NTSTATUS HandleRemoveDevice(PGENERIC_EXTENSION pdx, PIRP Irp);
NTSTATUS HandleStartDevice(PGENERIC_EXTENSION pdx, PIRP Irp);
NTSTATUS HandleStopDevice(PGENERIC_EXTENSION pdx, PIRP Irp);
NTSTATUS HandleSurpriseRemoval(PGENERIC_EXTENSION pdx, PIRP Irp);
NTSTATUS HandleUsageNotification(PGENERIC_EXTENSION pdx, PIRP Irp);
NTSTATUS OnNotificationComplete(PDEVICE_OBJECT fdo, PIRP Irp, PGENERIC_EXTENSION pdx);

VOID AdjustDeviceCapabilities(PGENERIC_EXTENSION pdx, PDEVICE_CAPABILITIES pdc);
VOID AdjustWakeCapabilities(PGENERIC_EXTENSION pdx, PDEVICE_CAPABILITIES pdc, DEVICE_POWER_STATE dstate);
VOID DeregisterInterface(PGENERIC_EXTENSION pdx, PINTERFACE_RECORD ifp);
VOID DeregisterAllInterfaces(PGENERIC_EXTENSION pdx);
VOID EnableAllInterfaces(PGENERIC_EXTENSION pdx, BOOLEAN enable);
PINTERFACE_RECORD FindInterfaceRecord(PGENERIC_EXTENSION pdx, const GUID* guid);




#endif // __Pnp_h__
