//Versión Pre-Alpha 0.0.0.6 Todavía le falta pero ya es algo. PD: decidan ustedes que nombre le damos a este OS, para crear un repo en git
#ifndef BOOT_FLAG //garantiza que boot.h solo se compile una sola vez (Al parecer se puede compilar varias veces el mismo archivo)
#define BOOT_FLAG
#include <stdint.h> //Librería con tipos de datos de tamaño fijo

//typos de datos
typedef uint8_t                 UINT8;
typedef uint16_t                UINT16;
typedef uint32_t                UINT32;
typedef uint64_t                UINT64;
typedef uintptr_t               UINTN; //dependiente de la arquitectura
typedef int8_t                  INT8;
typedef int16_t                 INT16;
typedef int32_t                 INT32;
typedef int64_t                 INT64;
typedef intptr_t                INTN; //también depende de la arquitectura
typedef char                    CHAR8;
typedef uint16_t                CHAR16;
typedef UINT8                   BOOLEAN;
#define VOID void
#define NULL ((VOID *) 0)




//macros Genéricos
#define FALSE ((UINT8) 0)
#define TRUE ((UINT8) 1)
#define MAX_BIT ((UINTN) 1 << (sizeof(UINTN)*8 - 1))

// Tipos esenciales para el estándar:
typedef VOID *EFI_HANDLE;
typedef UINTN RETURN_STATUS;
typedef RETURN_STATUS EFI_STATUS;

//macros para el EFI
#define RETURN_SUCCESS 0
#define EFI_SUCCESS RETURN_SUCCESS
#define ENCODE_ERROR(StatusCode) ((RETURN_STATUS)(MAX_BIT | (StatusCode)))
#define ENCODE_WARNING(StatusCode) ((RETURN_STATUS)(StatusCode))
#define RETURN_ERROR(Status) ((INTN)(Status) < 0)
//#define RETURN_WARNING(Status) ((INTN)(Status) > 0)
#define EFI_ERROR(A) (RETURN_ERROR(A))
//#define EFI_WARNING(A) (RETURN_WARNING(A))



// Advertencias del Sistema
#define EFI_WARN_UNKNOWN_GLYPH      ENCODE_WARNING(1)
#define EFI_WARN_DELETE_FAILURE     ENCODE_WARNING(2)
#define EFI_WARN_WRITE_FAILURE      ENCODE_WARNING(3)
#define EFI_WARN_BUFFER_TOO_SMALL   ENCODE_WARNING(4)
#define EFI_WARN_STALE_DATA         ENCODE_WARNING(5)
#define EFI_WARN_FILE_SYSTEM        ENCODE_WARNING(6)
#define EFI_WARN_RESET_REQUIRED     ENCODE_WARNING(7)

//Errores del sistema
#define EFI_LOAD_ERROR              ENCODE_ERROR(1)
#define EFI_INVALID_PARAMETER       ENCODE_ERROR(2)
#define EFI_UNSUPPORTED             ENCODE_ERROR(3)
#define EFI_BAD_BUFFER_SIZE         ENCODE_ERROR(4)
#define EFI_BUFFER_TOO_SMALL        ENCODE_ERROR(5)
#define EFI_NOT_READY               ENCODE_ERROR(6)
#define EFI_DEVICE_ERROR            ENCODE_ERROR(7)
#define EFI_WRITE_PROTECTED         ENCODE_ERROR(8)
#define EFI_OUT_OF_RESOURCES        ENCODE_ERROR(9)
#define EFI_VOLUME_CORRUPTED        ENCODE_ERROR(10)
#define EFI_VOLUME_FULL             ENCODE_ERROR(11)
#define EFI_NO_MEDIA                ENCODE_ERROR(12)
#define EFI_MEDIA_CHANGED           ENCODE_ERROR(13)
#define EFI_NOT_FOUND               ENCODE_ERROR(14)
#define EFI_ACCESS_DENIED           ENCODE_ERROR(15)
#define EFI_NO_RESPONSE             ENCODE_ERROR(16)
#define EFI_NO_MAPPING              ENCODE_ERROR(17)
#define EFI_TIMEOUT                 ENCODE_ERROR(18)
#define EFI_NOT_STARTED             ENCODE_ERROR(19)
#define EFI_ALREADY_STARTED         ENCODE_ERROR(20)
#define EFI_ABORTED                 ENCODE_ERROR(21)
#define EFI_ICMP_ERROR              ENCODE_ERROR(22)
#define EFI_TFTP_ERROR              ENCODE_ERROR(23)
#define EFI_PROTOCOL_ERROR          ENCODE_ERROR(24)
#define EFI_INCOMPATIBLE_VERSION    ENCODE_ERROR(25)
#define EFI_SECURITY_VIOLATION      ENCODE_ERROR(26)
#define EFI_CRC_ERROR               ENCODE_ERROR(27)
#define EFI_END_OF_MEDIA            ENCODE_ERROR(28)
#define EFI_END_OF_FILE             ENCODE_ERROR(31)
#define EFI_INVALID_LANGUAGE        ENCODE_ERROR(32)
#define EFI_COMPROMISED_DATA        ENCODE_ERROR(33)
#define EFI_IP_ADDRESS_CONFLICT     ENCODE_ERROR(34)
#define EFI_HTTP_ERROR              ENCODE_ERROR(35)



//Tipos de Dirección de Memoria
typedef UINT64 EFI_PHYSICAL_ADDRESS;//dirección de memoria
typedef UINT64 EFI_VIRTUAL_ADDRESS; //equivalente al de arriba por el identity mapping, pero el firmware lo pide
typedef UINT64 EFI_LBA;             //En esencia, cada disco duro está subdividido en bloques, y este registra la posición del mismo.




//estructuras:
#pragma pack(push, 1)//Creamos un stack interno con la config del padding actual y se establece el padding nuevo a 1
typedef struct {
    UINT32 Data1;//Data1 al Data3 deben estar en little-endian (Ejemplo: 0x12345678 se almacena como 78 56 34 12; big-endian sería 12 34 56 78)
    UINT16 Data2;
    UINT16 Data3;
    UINT8  Data4[8];//Data4 es un array de 8 bytes en orden natural sin swap.
}EFI_GUID; //Globally Unique Identifier.
#pragma pack(pop)//Sacamos del stack el ultimo elemento AKA:la config del padding, luego regresamos al config del padding por default


// Table Things

typedef struct {
    EFI_GUID    VendorGuid; // Un valor GUID de 128-bit que identifica la tabla de config. del sistema
    VOID        *VendorTable; // A generic pointer to the table associated with VendorGuid.
} EFI_CONFIGURATION_TABLE;

typedef struct {
    UINT64 Signature;
    UINT32 Revision;
    UINT32 HeaderSize; //Total de Bytes que ocupan en total los campos (desconozco si le afecta el padding al conteo. Revisaré eso después)
    UINT32 CRC32;
    UINT32 Reserved;
}EFI_TABLE_HEADER;




//EFIAPI, un poco molesto por su dependencia de la arquitectura y el tipo de compilador. Compilamos únicamente con GCC
// EFIAPI para x86
#if defined(__x86_64__) || defined(_M_X64)
    // x86_64
    #define EFIAPI __attribute__((ms_abi))
#elif defined(__i386__) || defined(_M_IX86)
    // x86 32 bits
    #define EFIAPI __attribute__((cdecl)) //explícito por claridad, aunque innecesario si todo funciona bien
#else
    #error "EFIAPI: Unsupported Architecture or Unsupported Compiler."
#endif


typedef EFI_STATUS (EFIAPI *EFI_INSTALL_CONFIGURATION_TABLE) (
    EFI_GUID        *Guid,
    VOID            *Table
);


//Task Priority Services (TPS)
typedef UINTN EFI_TPL;
#define TPL_APPLICATION 4
#define TPL_CALLBACK    8
#define TPL_NOTIFY      16
#define TPL_HIGH_LEVEL  31


typedef EFI_TPL (EFIAPI *EFI_RAISE_TPL) (
    EFI_TPL     NewTpl
);
typedef VOID (EFIAPI *EFI_RESTORE_TPL) (
    EFI_TPL     OldTpl
);


//Event Management

typedef VOID *EFI_EVENT;

#define EVT_TIMER               0x80000000
#define EVT_RUNTIME             0x40000000
#define EVT_NOTIFY_WAIT         0x00000100
#define EVT_NOTIFY_SIGNAL       0x00000200

#define EVT_SIGNAL_EXIT_BOOT_SERVICES       0x00000201
#define EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE   0x60000202

typedef VOID (EFIAPI *EFI_EVENT_NOTIFY) (
    EFI_EVENT   Event,
    VOID        *Context
);



typedef EFI_STATUS (EFIAPI *EFI_CREATE_EVENT) (
    UINT32              Type,
    EFI_TPL             NotifyTpl,
    EFI_EVENT_NOTIFY    NotifyFunction,
    VOID                *NotifyContext,
    EFI_EVENT           *Event
);

typedef EFI_STATUS (EFIAPI *EFI_CREATE_EVENT_EX) ( // Según la spec, faltan tres catalogados como OPTIONAL.
    UINT32              Type,
    EFI_TPL             NotifyTpl,
    EFI_EVENT_NOTIFY    NotifyFunction,
    const VOID          *NotifyContext,
    const EFI_GUID      *EventGroup,
    EFI_EVENT           *Event
);

typedef EFI_STATUS (EFIAPI *EFI_CLOSE_EVENT) (
    EFI_EVENT   Event
);

typedef EFI_STATUS (EFIAPI *EFI_SIGNAL_EVENT) (
    EFI_EVENT   Event
);

typedef EFI_STATUS (EFIAPI *EFI_WAIT_FOR_EVENT) (
    UINTN       NumberOfEvent,
    EFI_EVENT   *Event,
    UINTN       *Index
);

typedef EFI_STATUS (EFIAPI *EFI_CHECK_EVENT) (
    EFI_EVENT   Event
);


//Timer Management
typedef enum {
    TimerCancel,
    TimerPeriodic,
    TimerRelative
}EFI_TIMER_DELAY;

typedef EFI_STATUS (EFIAPI *EFI_SET_TIMER) (
    EFI_EVENT           Event,
    EFI_TIMER_DELAY     Type,
    UINT64              TriggerTime
);


// Protocols

typedef enum {
    EFI_NATIVE_INTERFACE
} EFI_INTERFACE_TYPE;

typedef EFI_STATUS (EFIAPI *EFI_INSTALL_PROTOCOL_INTERFACE) (
    EFI_HANDLE              *Handle,
    EFI_GUID                *Protocol,
    EFI_INTERFACE_TYPE      InterfaceType,
    VOID                    *Interface
);

typedef EFI_STATUS (EFIAPI *EFI_UNINSTALL_PROTOCOL_INTERFACE) (
    EFI_HANDLE  Handle,
    EFI_GUID    *Protocol,
    VOID        *Interface
);
typedef EFI_STATUS (EFIAPI *EFI_REINSTALL_PROTOCOL_INTERFACE) (
    EFI_HANDLE  Handle,
    EFI_GUID    *Protocol,
    VOID        *OldInterface,
    VOID        *NewInterface
);
typedef EFI_STATUS (EFIAPI *EFI_REGISTER_PROTOCOL_NOTIFY) (
    EFI_GUID    *Protocol,
    EFI_EVENT   Event,
    VOID        **Registration
);

//Search Type for Locate Handle:
typedef enum {
    AllHandles,
    ByRegisterNotify,
    ByProtocol
} EFI_LOCATE_SEARCH_TYPE;

typedef EFI_STATUS (EFIAPI *EFI_LOCATE_HANDLE) (
    EFI_LOCATE_SEARCH_TYPE      SearchType,
    EFI_GUID                    *Protocol, // Pasar a NULL
    VOID                        *SearchKey, // Pasar a NULL
    UINTN                       *BufferSize,
    EFI_HANDLE                  *Buffer
);

typedef EFI_STATUS (EFIAPI *EFI_HANDLE_PROTOCOL) (
    EFI_HANDLE          Handle,
    EFI_GUID            *Protocol,
    VOID                **Interface
);
//Paths
typedef EFI_STATUS (EFIAPI *EFI_OPEN_PROTOCOL) (
    EFI_HANDLE          Handle,
    EFI_GUID            *Protocol,
    VOID                **Interface,
    EFI_HANDLE          AgentHandle,
    EFI_HANDLE          ControllerHandle,
    UINT32              Attributes
);

//definition related to OpenProtocol:
#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL       0x00000001
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL             0x00000002
#define EFI_OPEN_PROTOCOL_TEST_PROTOCOL            0x00000004
#define EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER      0x00000008
#define EFI_OPEN_PROTOCOL_BY_DRIVER                0x00000010
#define EFI_OPEN_PROTOCOL_EXCLUSIVE                0x00000020
//close protocol

typedef EFI_STATUS (EFIAPI *EFI_CLOSE_PROTOCOL) (
    EFI_HANDLE      Handle,
    EFI_GUID        *Protocol,
    EFI_HANDLE      AgentHandle,
    EFI_HANDLE      ControllerHandle
);

typedef struct {
    EFI_HANDLE      AgentHandle;
    EFI_HANDLE      ControllerHandle;
    UINT32          Attributes;
    UINT32          OpenCoun;
} EFI_OPEN_PROTOCOL_INFORMATION_ENTRY;

typedef EFI_STATUS (EFIAPI *EFI_OPEN_PROTOCOL_INFORMATION) (
    EFI_HANDLE                              Handle,
    EFI_GUID                                *Protocol,
    EFI_OPEN_PROTOCOL_INFORMATION_ENTRY     **EntryBuffer,
    UINTN                                   *EntryCount
);

typedef struct{
    UINT8   Type;
    UINT8   SubType;
    UINT8   Length[2];
} EFI_DEVICE_PATH_PROTOCOL;

typedef EFI_STATUS (EFIAPI *EFI_DEVICE_PATH) (
    EFI_GUID                    *Protocol,
    EFI_DEVICE_PATH_PROTOCOL    **DevicePath,
    EFI_HANDLE                  *Device
);


typedef EFI_STATUS (EFIAPI *EFI_LOCATE_DEVICE_PATH) (
    EFI_GUID                    *Protocol,
    EFI_DEVICE_PATH_PROTOCOL    **DevicePath,
    EFI_HANDLE                  *Device
);

typedef EFI_STATUS (EFIAPI *EFI_CONNECT_CONTROLLER) (
    EFI_HANDLE                  ControllerHandle,
    EFI_HANDLE                  *DriverImageHandle, //OPTIONAL, returned to NULL
    EFI_DEVICE_PATH_PROTOCOL    *RemainingDevicePath, //OPTIONAL
    BOOLEAN                     Recursive
);

typedef EFI_STATUS (EFIAPI *EFI_DISCONNECT_CONTROLLER) (
    EFI_HANDLE          ControllerHandle,
    EFI_HANDLE          DriverImageHandle,
    EFI_HANDLE          ChildHandle
);

typedef EFI_STATUS (EFIAPI *EFI_PROTOCOLS_PER_HANDLE) (
    EFI_HANDLE          Handle,
    EFI_GUID            ***ProtocolBuffer,
    UINTN               *ProtocolBufferCount
);

typedef EFI_STATUS (EFIAPI *EFI_LOCATE_HANDLE_BUFFER) (
    EFI_LOCATE_SEARCH_TYPE      SearchType,
    EFI_GUID                    *Protocol,
    VOID                        *SearchKey,
    UINTN                       *NoHandles,
    EFI_HANDLE                  **Buffer
);
typedef EFI_STATUS  (EFIAPI *EFI_LOCATE_PROTOCOL) (
    EFI_GUID        *Protocol,
    VOID            *Registration,
    VOID            **Interface
);

typedef EFI_STATUS (EFIAPI *EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES) (
    EFI_HANDLE      *Handle
);
typedef EFI_STATUS (EFIAPI *EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES) (
    EFI_HANLDE      Handle
);

//Image Handling
typedef EFI_STATUS (EFIAPI *EFI_IMAGE_LOAD) (
    BOOLEAN                         BootPolicy,
    EFI_HANDLE                      ParentImageHandle,
    EFI_DEVICE_PATH_PROTOCOL        *DevicePath,
    VOID                            *SourceBuffer,
    UINTN                           SourceSize,
    EFI_HANDLE                      *ImageHandle
);

typedef EFI_STATUS (EFIAPI *EFI_IMAGE_START) (
    EFI_HANDLE              ImageHandle,
    UINTN                   *ExitDataSize,
    VOID                    **ExitData
);

typedef EFI_STATUS (EFIAPI *EFI_IMAGE_UNLOAD) (
    EFI_HANDLE          ImageHandle
);




//Bloque de memoria



// Pages
// Constantes
#define EFI_PAGE_SIZE 4096
// #define EFI_LARGE_PAGE_SIZE
// #define EFI_HUGE_PAGE_SIZE
#define EFI_PAGE_SHIFT 12
// #define EFI_LARGE_PAGE_SHIFT
// #define EFI_HUGE_PAGE_SHIFT
#define EFI_PAGE_MASK 0xFFF
//Funciones a punteros.
#define EFI_PAGES_TO_SIZE(Pages) ((Pages) << EFI_PAGE_SHIFT)
#define EFI_SIZE_TO_PAGES(Size) (((Size)+EFI_PAGE_SIZE -1)>> EFI_PAGE_SHIFT)




typedef enum {          //el esntándar UEFI recoge el enum, por lo que no-problem
    AllocateAnyPages,   //La dirección que más le gusta a la Cuántica
    AllocateMaxAddress, // Cualquiera por debajo de 80 años
    AllocateAddress,    // Dirección específica
    MaxAllocateType,    //Un Guardia, más alla de aquí se ignora
}EFI_ALLOCATE_TYPE;
typedef enum {
    /*Por lo que estoy viendo, definimos, excepto para EfiReservedMemoryType, una estructura asi:
    Efi(MEMORY_TYPE)Code,
    Efi(MEMORY_Type)Data,
    Code para el código propiamente dicho y Data para todo lo no ejecutable
    */
    EfiReservedMemoryType,      // Memoria Reservada por el firmware
    EfiLoaderCode,              // Boot Sector loaded on memory
    EfiLoaderData,              // Datos del bootloader: buffers, estructuras, variables globales
    EfiBootServicesCode,        // Habría que revisar más tarde cuales son los Boot Services para UEFI.
    EfiBootServicesData,
    EfiRuntimeServicesCode,     // Esta memoria no se libera
    EfiRuntimeServicesData,     // Por razones razonables esta tampoco
    EfiConventionalMemory,      // Memoria usable
    EfiUnusableMemory,          // Memoria asignada a errores del hardware
    EfiACPIReclaimMemory,       // No tengo claro que son las tablas ACPI, pero ya lo veré después
    EfiACPIMemoryNVS,           // Al parecer, parte de la memoria asignada a ACPI es volátil, pero parte de ella no. Interesante
    EfiMemoryMappedIO,          // Habría que ver después si puede hacer compatibilidad nativa con Arduino u otros artefactos. Va más allá del plan, pero en cualquier momento nos podemos ampliar.
    EfiMemoryMappedIOPortSpace, // Los puertos, no sé si va en Virtual o se tira directamente con las direcciones reales.
    EfiPalCode,                 // Processor Abstraction Layer. Rara vez la usaré, pero tiene que estar, y ahí está
    EfiPersistentMemory,        // Otra memoria no volátil, esta vez para servidores.
    EfiMaxMemoryType,           // Un Guardia, marca el límite de EFI_MEMORY_TYPE, más alla de aquí no se asocia a nada real, ergo se ignora
}EFI_MEMORY_TYPE;

typedef struct {
    UINT32                  Type;       //Tipo de Memoria
    UINT32                  Pad;        //Padding para alinear a 8 bytes
    EFI_PHYSICAL_ADDRESS    PhysicalStart;
    EFI_VIRTUAL_ADDRESS     VirtualStart;
    UINT64                  NumberOfPages; //marca cuantas paginas de 4KB ocupa, luego no requiere de un VirtualEnd, por ejemplo
    UINT64                  Attribute;  //Al parecer son para flags relacionados al caché y los permisos
}EFI_MEMORY_DESCRIPTOR;

typedef EFI_STATUS (EFIAPI *EFI_ALLOCATE_PAGES) ( //Asignar Memoria de forma arbitraria
    EFI_ALLOCATE_TYPE       Type,
    EFI_MEMORY_TYPE         MemoryType,
    UINTN                   Pages,
    EFI_PHYSICAL_ADDRESS    *Memory
);

typedef EFI_STATUS (EFIAPI *EFI_FREE_PAGES) ( //Liberar número arbitrario de Espacio de Memoria
    EFI_PHYSICAL_ADDRESS    Memory,
    UINTN                   Pages
);

typedef EFI_STATUS (EFIAPI *EFI_GET_MEMORY_MAP) ( //Mapa de asignación de memoria en la RAM
    UINTN                   *MemoryMapSize,
    EFI_MEMORY_DESCRIPTOR   *MemoryMap,
    UINTN                   *MapKey,
    UINTN                   *DescriptorSize,
    UINT32                  *DescriptorVersion
);

typedef EFI_STATUS (EFIAPI *EFI_ALLOCATE_POOL) ( //Alocar X bytes de memoria a un Pool
    EFI_MEMORY_TYPE     PoolType,
    UINTN               Size,
    VOID                **Buffer
);

typedef EFI_STATUS (EFIAPI *EFI_FREE_POOL) ( //Liberar el Pool
    VOID    *Buffer
);





// Text Management


typedef EFI_STATUS (EFIAPI *EFI_INPUT_KEY) (
    UINT16      ScanCode,
    CHAR16      UnicodeChar
);

typedef struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
    EFI_INPUT_RESET         Reset;
    EFI_INPUT_READ_KEY      ReadKeyStroke;
    EFI_EVENT               WaitForKey;
} EFI_SIMPLE_TEXT_INPUT_PROTOCOL;
typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL{
    EFI_TEXT_RESET                      Reset;
    EFI_TEXT_STRING                     OutputString;
    EFI_TEXT_TEST_STRING                TestString;
    EFI_TEXT_QUERY_MODE                 QueryMode;
    EFI_TEXT_SET_MODE                   SetMode;
    EFI_TEXT_SET_ATTRIBUTE              SetAttribute;
    EFI_TEXT_CLEAR_SCREEN               ClearScreen;
    EFI_TEXT_SET_CURSOR_POSITION        SetCursorPosition;
    EFI_TEXT_ENABLE_CURSOR              EnableCursor;
    SIMPLE_TEXT_OUTPUT_MODE             *Mode;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

//Input services
typedef EFI_STATUS (EFIAPI *EFI_INPUT_RESET) (
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL      *This,
    BOOLEAN                             ExtendedVerification
);
typedef EFI_STATUS (EFIAPI *EFI_INPUT_READ_KEY) (
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL      *This,
    EFI_INPUT_KEY                       *Key
);


// Output & Others
typedef EFI_STATUS (EFIAPI *EFI_TEXT_RESET) (
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL     *This,
    BOOLEAN                             ExtendedVerification
);
typedef EFI_STATUS (EFIAPI *EFI_TEXT_STRING) (
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL     *This,
    CHAR16                              *String
);
typedef EFI_STATUS (EFIAPI *EFI_TEXT_TEST_STRING) (
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL     *This,
    CHAR16                              *String
);
typedef EFI_STATUS (EFIAPI *EFI_TEXT_QUERY_MODE) (
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL     *This,
    UINTN                               ModelNumber,
    UINTN                               *Columns,
    UINTN                               *Rows
);

//Exit Services

typedef EFI_STATUS (EFIAPI *EFI_EXIT) (
    EFI_HANDLE          ImageHandle,
    EFI_STATUS          ExitStatus,
    UINTN               ExitDataSize,
    CHAR16              ExitData //OPTIONAL; returned to NULL
);
typedef EFI_STATUS (EFIAPI *EFI_EXIT_BOOT_SERVICES) (
    EFI_HANDLE      ImageHandle,
    UINTN           MapKey
);






//Miscelaneous

typedef EFI_STATUS (EFIAPI *EFI_STALL) (
    UINTN           Microseconds
);

typedef EFI_STATUS (EFIAPI *EFI_SET_WATCHDOG_TIMER) (
    UINTN           Timeout,
    UINT64          WatchdogCode,
    UINTN           DataSize,
    CHAR16          *WatchDogData
);
typedef EFI_STATUS (EFIAPI *EFI_CALCULATE_CRC32) (
    VOID            *Data,
    UINTN           DataSize,
    UINT32          *Crc32
);
typedef EFI_STATUS (EFIAPI *EFI_COPY_MEM) (
    VOID            *Destination,
    VOID            *Source,
    UINTN           Length
);
typedef EFI_STATUS (EFIAPI *EFI_SET_MEM) (
    VOID            *Buffer,
    UINTN           Size,
    UINT8           Value
);
typedef EFI_STATUS (EFIAPI *EFI_GET_NEXT_MONOTONIC_COUNT) (
    UINT64          *Count
);

// Boot Services
typedef struct {
    EFI_TABLE_HEADER                                Hdr;

    // TPS
    EFI_RAISE_TPL                                   RaiseTPL;
    EFI_RESTORE_TPL                                 RestoreTPL;

    //Pages & Pools
    EFI_ALLOCATE_PAGES                              AllocatePages;
    EFI_FREE_PAGES                                  FreePages;
    EFI_GET_MEMORY_MAP                              GetMemoryMap;
    EFI_ALLOCATE_POOL                               AllocatePool;
    EFI_FREE_POOL                                   FreePool;

    //Event & Timer Services
    EFI_CREATE_EVENT                                CreateEvent;
    EFI_SET_TIMER                                   SetTimer;
    EFI_WAIT_FOR_EVENT                              WaitForEvent;
    EFI_SIGNAL_EVENT                                SignalEvent;
    EFI_CLOSE_EVENT                                 CloseEvent;
    EFI_CHECK_EVENT                                 CheckEvent;

    //Protocols
    EFI_INSTALL_PROTOCOL_INTERFACE                  InstallProtocolInterface;
    EFI_REINSTALL_PROTOCOL_INTERFACE                ReinstallProtocolInterface;
    EFI_UNINSTALL_PROTOCOL_INTERFACE                UninstallProtocolInterface;
    EFI_HANDLE_PROTOCOL                             HandleProtocol;
    VOID                                            *Reserved;
    EFI_REGISTER_PROTOCOL_NOTIFY                    RegisterProtocolNotify;
    EFI_LOCATE_HANDLE                               LocateHandle;
    EFI_LOCATE_DEVICE_PATH                          LocateDevicePath;
    EFI_INSTALL_CONFIGURATION_TABLE                 InstallConfigurationTable;


    //Image Handle Services

    EFI_IMAGE_LOAD                                  ImageLoad;
    EFI_IMAGE_START                                 ImageStart;
    EFI_EXIT                                        Exit;
    EFI_IMAGE_UNLOAD                                ImageUnload;
    EFI_EXIT_BOOT_SERVICES                          ExitBootServices;

    EFI_GET_NEXT_MONOTONIC_COUNT                    GetNextMonotonicCount;
    EFI_STALL                                       Stall;
    EFI_SET_WATCHDOG_TIMER                          SetWatchdogTimer;

    EFI_CONNECT_CONTROLLER                          ConnectController;
    EFI_DISCONNECT_CONTROLLER                       DisconnectController;

    EFI_OPEN_PROTOCOL                               OpenProtocol;
    EFI_CLOSE_PROTOCOL                              CloseProtocol;
    EFI_OPEN_PROTOCOL_INFORMATION                   OpenProtocolInformation;
    EFI_PROTOCOLS_PER_HANDLE                        ProtocolsPerHandle;
    EFI_LOCATE_HANDLE_BUFFER                        LocateHandleBuffer;
    EFI_LOCATE_PROTOCOL                             LocateProtocol;
    EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES        InstallMultipleProtocolInterfaces;
    EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES      UninstallMultipleProtocolInterfaces;
    EFI_CALCULATE_CRC32                             CalculateCrc32;
    EFI_COPY_MEM                                    CopyMem;
    EFI_SET_MEM                                     SetMem;
    EFI_CREATE_EVENT_EX                             CreateEventEx;
}EFI_BOOT_SERVICES;

typedef struct {
    EFI_TABLE_HEADER            Hdr;
    CHAR16                      *FirmwareVendor;
    UINT32                      FirmwareRevision;
    EFI_HANDLE                  ConsoleInHandle;
    VOID                        *ConIn; // EFI_SIMPLE_TEXT_INPUT_PROTOCOL (Falta)
    EFI_HANDLE                  ConsoleOutHandle;
    VOID                        *ConOut; // EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL (Falta)
    EFI_HANDLE                  StandardErrorHandle;
    VOID                        *StdErr;            // EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *
    VOID                        *RuntimeServices;   // EFI_RUNTIME_SERVICES *
    EFI_BOOT_SERVICES           *BootServices;
    UINTN                       NumberOfTableEntries;
    EFI_CONFIGURATION_TABLE     *ConfigurationTable;
}EFI_SYSTEM_TABLE;

typedef EFI_STATUS (EFIAPI *EFI_IMAGE_ENTRY_POINT) (
    EFI_HANDLE              ImageHandle,
    EFI_SYSTEM_TABLE        *SystemTable
);
#endif%
