#ifndef _USB_GENERIC_H
#define _USB_GENERIC_H
#include <libmaple/libmaple_types.h>
typedef unsigned short u16;
typedef unsigned char u8;

#include <libmaple/usb.h>
#include <libmaple/nvic.h>
#include <libmaple/delay.h>
#include "usb_lib_globals.h"
#include "usb_reg_map.h"

#define USB_CONTROL_DONE 1

#define PMA_MEMORY_SIZE 512
#define MAX_USB_DESCRIPTOR_DATA_SIZE 200
#define USB_MAX_STRING_DESCRIPTOR_LENGTH 32

#define USB_EP0_BUFFER_SIZE       0x40
#define USB_EP0_TX_BUFFER_ADDRESS 0x40
#define USB_EP0_RX_BUFFER_ADDRESS (USB_EP0_TX_BUFFER_ADDRESS+USB_EP0_BUFFER_SIZE) 

#ifdef __cplusplus
extern "C" {
#endif

extern const usb_descriptor_string usb_generic_default_iManufacturer;
extern const usb_descriptor_string usb_generic_default_iProduct;

typedef struct USBEndpointInfo {
    void (*callback)(void);
    uint16 bufferSize;
    uint16 type; // bulk, interrupt, etc.
    uint8 tx:1; // 1 if TX, 0 if RX
    uint8 exclusive:1; // 1 if cannot use the same endpoint number for both rx and tx
    uint8 address;    
    uint16 pmaAddress;
} USBEndpointInfo;

typedef struct USBCompositePart {
    uint8 numInterfaces;
    uint8 numEndpoints;
	uint8 startInterface;
    uint16 descriptorSize;
    void (*getPartDescriptor)(uint8* out);
    void (*usbInit)(void);
    void (*usbReset)(void);
    void (*usbSetConfiguration)(void);
    void (*usbClearFeature)(void);
    void (*clear)(void);
    RESULT (*usbDataSetup)(uint8 request, uint8 interface, uint8 requestType, uint8 wValue0, uint8 wValue1, uint16 wIndex, uint16 wLength );
    RESULT (*usbNoDataSetup)(uint8 request, uint8 interface, uint8 requestType, uint8 wValue0, uint8 wValue1, uint16 wIndex);
    USBEndpointInfo* endpoints;
} USBCompositePart;

static inline void usb_generic_enable_rx(uint8 endpointAddress) {
    usb_set_ep_rx_stat(endpointAddress, USB_EP_STAT_RX_VALID);
}

static inline void usb_generic_enable_tx(uint8 endpointAddress) {
    usb_set_ep_tx_stat(endpointAddress, USB_EP_STAT_TX_VALID);
}

static inline void usb_generic_enable_rx_ep0() {
    usb_set_ep_rx_stat(USB_EP0, USB_EP_STAT_RX_VALID);
}

static inline void usb_generic_pause_rx_ep0() {
    usb_set_ep_rx_stat(USB_EP0, USB_EP_STAT_RX_NAK);
}


void usb_generic_set_disconnect_delay(uint32 delay);
void usb_generic_set_info(uint16 idVendor, uint16 idProduct, const char* iManufacturer, const char* iProduct, const char* iSerialNumber);
uint8 usb_generic_set_parts(USBCompositePart** _parts, unsigned _numParts);
void usb_generic_control_rx_setup(volatile void* buffer, uint16 length, volatile uint8* done);
void usb_generic_control_tx_setup(volatile void* buffer, uint16 length, volatile uint8* done);
void usb_generic_control_descriptor_tx(ONE_DESCRIPTOR* d);
void usb_generic_disable(void);
void usb_generic_enable(void);
extern volatile int8 usbGenericTransmitting;
void usb_copy_from_pma(uint8 *buf, uint16 len, uint16 pma_offset);
void usb_copy_to_pma(const uint8 *buf, uint16 len, uint16 pma_offset);

#ifdef __cplusplus
}
#endif

#define USB_GENERIC_ENDPOINT_TYPE_INTERRUPT USB_EP_EP_TYPE_INTERRUPT
#define USB_GENERIC_ENDPOINT_TYPE_BULK USB_EP_EP_TYPE_BULK
#define USB_GENERIC_ENDPOINT_TYPE_CONTROL USB_EP_EP_TYPE_CONTROL
#define USB_GENERIC_ENDPOINT_TYPE_ISO USB_EP_EP_TYPE_ISO

#endif
