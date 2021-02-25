#include <ntddk.h>
#include "ThreadPriority.h"

void ThreadPriorityUnload(PDRIVER_OBJECT);
NTSTATUS ThreadPriorityCreateClose(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS ThreadPriorityDeviceControl(PDEVICE_OBJECT DeviceObject, PIRP Irp);

#define THREAD_PRIORITY_PREFIX "ThreadPriority: "

class AutoEnterLeaveFunction {
	LPCSTR _function;
public:
	AutoEnterLeaveFunction(LPCSTR function) {
		KdPrint((THREAD_PRIORITY_PREFIX "Enter: %s\n", _function = function));
	}
	~AutoEnterLeaveFunction() {
		KdPrint((THREAD_PRIORITY_PREFIX "Leave: %s\n", _function));
	}
};

#define AUTO_ENTER_LEAVE() AutoEnterLeaveFunction aelf(__FUNCTION__)

extern "C"
NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING) {
	AUTO_ENTER_LEAVE();


	DriverObject->DriverUnload = ThreadPriorityUnload;
	DriverObject->MajorFunction[IRP_MJ_CREATE] =
		DriverObject->MajorFunction[IRP_MJ_CLOSE] = ThreadPriorityCreateClose;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ThreadPriorityDeviceControl;
	PDEVICE_OBJECT DeviceObject;
	UNICODE_STRING devName, win32Name;
	RtlInitUnicodeString(&devName, L"\\Device\\ThreadPriority");
	NTSTATUS status = IoCreateDevice(DriverObject, 0, &devName, FILE_DEVICE_UNKNOWN, 0, FALSE, &DeviceObject);
	
	if (status != STATUS_SUCCESS) {
		KdPrint((THREAD_PRIORITY_PREFIX "Error creating device %d\n", status));
		return status;
	}

	RtlInitUnicodeString(&win32Name, L"\\??\\ThreadPriority");
	IoCreateSymbolicLink(&win32Name, &devName);

	return STATUS_SUCCESS;
}
void ThreadPriorityUnload(PDRIVER_OBJECT DriverObject) {
	AUTO_ENTER_LEAVE();

	UNICODE_STRING win32Name;
	RtlInitUnicodeString(&win32Name, L"\\??\\ThreadPriority");
	IoDeleteSymbolicLink(&win32Name);
	IoDeleteDevice(DriverObject->DeviceObject);
}

NTSTATUS ThreadPriorityCreateClose(PDEVICE_OBJECT, PIRP Irp) {

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}


NTSTATUS ThreadPriorityDeviceControl(PDEVICE_OBJECT, PIRP Irp) {

	AUTO_ENTER_LEAVE();
	auto stack = IoGetCurrentIrpStackLocation(Irp);
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	switch (stack->Parameters.DeviceIoControl.IoControlCode) {
	case IOCTL_THREAD_PRIORITY:
		if (stack->Parameters.DeviceIoControl.InputBufferLength < sizeof(ThreadPriorityData)) {
			status = STATUS_INVALID_BUFFER_SIZE;
			break;
		}
		ThreadPriorityData* data = (ThreadPriorityData*)Irp->AssociatedIrp.SystemBuffer;
		PKTHREAD Thread;
		status = ObReferenceObjectByHandle(data->hThread, THREAD_SET_INFORMATION, *PsThreadType, UserMode, (PVOID*)&Thread, nullptr);
		if (status != STATUS_SUCCESS) break;
		KeSetPriorityThread(Thread, data->Priority);
		ObDereferenceObject(Thread);
		status = STATUS_SUCCESS;
		break;
	}
	Irp->IoStatus.Status.Status = status;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}