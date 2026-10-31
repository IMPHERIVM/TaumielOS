// Versión Pre-Alpha: 0.0.0.1
#include "boot.h"

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    /*  ImageHandle es el identificador de la imagen .EFI que está ejecutando el firmware
        SystemTable es la dirección de memoria de la tabla principal del sistema UEFI;
        SystemTable contiene los servicios proporcionados por el firmware, incluidos los
        boot services.
    */
    (VOID)ImageHandle; //No trabajamos todavía con ImageHandle, descartamos ImageHandle a propósito para no tener un error en la compilación
    EFI_BOOT_SERVICES *BS = SystemTable->BootServices; //Puntero a una struct EFI_BOOT_SERVICES, accede al miembro BootServices de la struct SystemTable y guarda su valor
    EFI_STATUS Status;
    // Primera Consulta: Estado Inicial del Mapa de Memoria
    UINTN MemoryMapSize = 0;                    //tamaño del buffer del MemoryMap, en bytes
    UINTN MapKey = 0;                           //Clave asociada al MemoryMap obtenido. Iniciada en 0 para obtener para evitar un uninitialized variable error. 
    UINTN DescriptorSize = 0;                   //Tamaño, en bytes, de cada descriptor de memoria (Puede no ser igual a sizeof(EFI_MEMORY_DESCRIPTOR))
    UINT32 DescriptorVersion = 0;               //Versión del formato de los descriptores de memoria
    EFI_MEMORY_DESCRIPTOR *MemoryMap = NULL;    //Declara un puntero a un descriptor de memoria
    Status = BS->GetMemoryMap(                  //Obtiene el Mapa de memoria inicial
        &MemoryMapSize,
        NULL,
        &MapKey,
        &DescriptorSize,
        &DescriptorVersion
    );

    if (Status != EFI_BUFFER_TOO_SMALL) { // Anticipamos que el firmware especifique que el tamaño del buffer fue insuficiente. 
        return EFI_ERROR(Status) ? Status : EFI_INVALID_PARAMETER; 
        //En caso de que el firmware determine que sea suficiente, lo cual no tiene sentido. Retorna un Error conocido, y si no retorna EFI_INVALID_PARAMETER.
    }

    if (DescriptorSize == 0) { //Si el Descriptor tiene tamaño nulo, no es utilizable, lanzamos EFI_COMPROMISED_DATA.
        return EFI_COMPROMISED_DATA;
    }


    for (;;) { //Bucle indefinido.
        UINTN ExtraSize = 2*DescriptorSize; // Ampliamos la memoria disponible en 2 descriptores de tamaño
        MemoryMapSize  +=   ExtraSize; // Memoria disponible: MS + 2D adicionales.
        Status = BS->GetMemoryMap(
            &MemoryMapSize,
            NULL,
            &MapKey,
            &DescriptorSize,
            &DescriptorVersion
        );
        if (Status==EFI_SUCCESS){
            break;
        }
    }

    //Allocate some Memory via Pool
    VOID *Buffer = NULL;
    UINTN BufferSize = 1024;
    EFI_STATUS BufferStatus = BS->AllocatePool(
        EfiLoaderData,
        BufferSize,
        &Buffer
    );



    //SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Hello UEFI\r\n");
    if (EFI_ERROR(BufferStatus)) {
        return BufferStatus;
    }
    return EFI_SUCCESS;
    
    
    /*
    EFI_PHYSICAL_ADDRESS Buffer = 0;
    EFI_STATUS BootStatus = BS->AllocatePages(
        AllocateAnyPages,
        EfiLoaderData,
        1,
        &Buffer
    );*/
    
}
