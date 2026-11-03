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
    //Precondiciones
    UINTN MemoryMapSize = 0;                    //tamaño del buffer del MemoryMap, en bytes
    UINTN MapKey = 0;                           //Clave asociada al MemoryMap obtenido. 
    UINTN DescriptorSize = 0;                   //Tamaño, en bytes, de cada descriptor de memoria (Puede no ser igual a sizeof(EFI_MEMORY_DESCRIPTOR))
    UINT32 DescriptorVersion = 0;               //Versión del formato de los descriptores de memoria
    EFI_MEMORY_DESCRIPTOR *MemoryMap = NULL;    //Declara un puntero a un descriptor de memoria
    //Mapa de memoria inicial
    Status = BS->GetMemoryMap(                  
        &MemoryMapSize,
        NULL,
        &MapKey,
        &DescriptorSize,
        &DescriptorVersion
    );
    
    if (Status != EFI_BUFFER_TOO_SMALL) { // Anticipamos que el firmware especifique que el tamaño del buffer fue insuficiente. 
        return EFI_ERROR(Status) ? Status : EFI_INVALID_PARAMETER; 
        //En caso de que el firmware determine que sea suficiente, lo cual no tiene sentido. Retorna el código del error, y si no retorna EFI_INVALID_PARAMETER.
    }
    if (DescriptorSize == 0) { //Si el Descriptor tiene tamaño nulo, no es utilizable, lanzamos EFI_COMPROMISED_DATA.
        return EFI_COMPROMISED_DATA;
    }
    
    UINTN RequiredSize = MemoryMapSize;
    UINTN NewDescriptorSize = DescriptorSize;
    VOID *Buffer = NULL;
    for (;;) { //Bucle indefinido.
        if (NewDescriptorSize == 0){
            return EFI_COMPROMISED_DATA;
        }
        if (NewDescriptorSize > (MAX_UINTN / 2) || RequiredSize > MAX_UINTN - (2 * NewDescriptorSize)){ //Impide un Size Overflow
            return EFI_BAD_BUFFER_SIZE;
        }
        UINTN BufferSize = RequiredSize + (2 * NewDescriptorSize); // Memoria disponible: MS + 2DS adicionales. Allocate some initial memory
        // Ampliamos la memoria disponible en 2 descriptores de tamaño
        EFI_STATUS BufferStatus = BS->AllocatePool( //Allocamos la cantidad de memoria que asignamos en BufferSize
            EfiLoaderData,
            BufferSize,
            &Buffer
        );

        if (EFI_ERROR(BufferStatus)) { // En caso de un error por parte de AllocatePool, revisamos su código
            return BufferStatus;
        }
        MemoryMap = (EFI_MEMORY_DESCRIPTOR *)Buffer; //Mapa de Memoria
        
        Status = BS->GetMemoryMap(
            &BufferSize,            //Capacidad disponible del buffer
            MemoryMap,              //Dirección donde se escribirán los descriptores
            &MapKey,                //Recibe la clave asociada al mapa obtenido
            &NewDescriptorSize,        //Recibe el tamaño de cada Descriptor
            &DescriptorVersion      //Versión de los Descriptores
        );

        if (Status != EFI_BUFFER_TOO_SMALL && Status != EFI_SUCCESS){ //En caso de un error no esperado
            EFI_STATUS NewStatus = BS ->FreePool(Buffer); //Liberamos el Pool
            if (EFI_ERROR(NewStatus)) { //En caso de error (Lo más probable sea EFI_INVALID_PARAMETER) retornamos su código
                return NewStatus;
            }
            Buffer = NULL; //Vaciamos el Buffer.
            return Status;
        }

        else if (Status==EFI_SUCCESS){
            break;
        }
        RequiredSize = BufferSize; //Reajustamos el Tamaño requerido a nuestras necesidades
        

        
    }

    
    
    return EFI_SUCCESS;
}
