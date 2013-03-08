/* 
 * File:   lib_mrf24j.h
 * Author: Karl Palsson, false.ekta.is
 *
 * BSD/MIT licensed
 *
 * Created on February 20, 2011, 4:02 PM
 */

#ifndef LIB_MRF24J_H
#define	LIB_MRF24J_H

#ifdef	__cplusplus
extern "C" {
#endif

#define MRF_RXMCR 0x00
#define MRF_PANIDL 0x01
#define MRF_PANIDH 0x02
#define MRF_SADRL 0x03
#define MRF_SADRH 0x04
#define MRF_EADR0 0x05
#define MRF_EADR1 0x06
#define MRF_EADR2 0x07
#define MRF_EADR3 0x08
#define MRF_EADR4 0x09
#define MRF_EADR5 0x0A
#define MRF_EADR6 0x0B
#define MRF_EADR7 0x0C
#define MRF_RXFLUSH 0x0D
//#define MRF_Reserved 0x0E
//#define MRF_Reserved 0x0F
#define MRF_ORDER 0x10
#define MRF_TXMCR 0x11
#define MRF_ACKTMOUT 0x12
#define MRF_ESLOTG1 0x13
#define MRF_SYMTICKL 0x14
#define MRF_SYMTICKH 0x15
#define MRF_PACON0 0x16
#define MRF_PACON1 0x17
#define MRF_PACON2 0x18
//#define MRF_Reserved 0x19
#define MRF_TXBCON0 0x1A

// TXNCON: TRANSMIT NORMAL FIFO CONTROL REGISTER (ADDRESS: 0x1B)
#define MRF_TXNCON      0x1B
#define MRF_TXNTRIG     0
#define MRF_TXNSECEN    1
#define MRF_TXNACKREQ   2
#define MRF_INDIRECT    3
#define MRF_FPSTAT      4

#define MRF_TXG1CON 0x1C
#define MRF_TXG2CON 0x1D
#define MRF_ESLOTG23 0x1E
#define MRF_ESLOTG45 0x1F
#define MRF_ESLOTG67 0x20
#define MRF_TXPEND 0x21
#define MRF_WAKECON 0x22
#define MRF_WAKECON_IMMWAKE	(1<<7)
#define MRF_WAKECON_REGWAKE	(1<<6)
#define MRF_FRMOFFSET 0x23
// TXSTAT: TX MAC STATUS REGISTER (ADDRESS: 0x24)
#define MRF_TXSTAT 0x24
#define TXNRETRY1       7
#define TXNRETRY0       6
#define CCAFAIL         5
#define TXG2FNT         4
#define TXG1FNT         3
#define TXG2STAT        2
#define TXG1STAT        1
#define TXNSTAT         0

#define MRF_TXBCON1 0x25
#define MRF_GATECLK 0x26
#define MRF_TXTIME 0x27
#define MRF_HSYMTMRL 0x28
#define MRF_HSYMTMRH 0x29
#define MRF_SOFTRST 0x2A
#define MRF_SOFTRST_RSTPWR	(1<<2)
#define MRF_SOFTRST_RSTBB	(1<<1)
#define MRF_SOFTRST_RSTMAC	(1<<0)
//#define MRF_Reserved 0x2B
#define MRF_SECCON0 0x2C
#define MRF_SECCON1 0x2D
#define MRF_TXSTBL 0x2E
//#define MRF_Reserved 0x2F
#define MRF_RXSR 0x30
#define MRF_INTSTAT 0x31
#define MRF_INTCON 0x32
#define MRF_GPIO 0x33
#define MRF_TRISGPIO 0x34
#define MRF_SLPACK 0x35
#define MRF_SLPACK_SLPACK	(1<<7)
#define MRF_RFCTL 0x36
#define MRF_RFCTL_RFRST		(1<<2)
#define MRF_SECCR2 0x37
#define MRF_BBREG0 0x38
#define MRF_BBREG1 0x39
#define MRF_BBREG2 0x3A
#define MRF_BBREG3 0x3B
#define MRF_BBREG4 0x3C
//#define MRF_Reserved 0x3D
#define MRF_BBREG6 0x3E
#define MRF_CCAEDTH 0x3F

#define MRF_RFCON0 0x200
#define MRF_RFCON1 0x201
#define MRF_RFCON2 0x202
#define MRF_RFCON3 0x203
#define MRF_RFCON5 0x205
#define MRF_RFCON6 0x206
#define MRF_RFCON7 0x207
#define MRF_RFCON8 0x208
#define MRF_SLPCAL0 0x209
#define MRF_SLPCAL1 0x20A
#define MRF_SLPCAL2 0x20B
#define MRF_RSSI 0x210
#define MRF_SLPCON0 0x211
#define MRF_SLPCON1 0x220
#define MRF_WAKETIMEL 0x222
#define MRF_WAKETIMEH 0x223
#define MRF_REMCNTL 0x224
#define MRF_REMCNTH 0x225
#define MRF_MAINCNT0 0x226
#define MRF_MAINCNT1 0x227
#define MRF_MAINCNT2 0x228
#define MRF_MAINCNT3 0x229
#define MRF_TESTMODE 0x22F
#define MRF_ASSOEADR1 0x231
#define MRF_ASSOEADR2 0x232
#define MRF_ASSOEADR3 0x233
#define MRF_ASSOEADR4 0x234
#define MRF_ASSOEADR5 0x235
#define MRF_ASSOEADR6 0x236
#define MRF_ASSOEADR7 0x237
#define MRF_ASSOSADR0 0x238
#define MRF_ASSOSADR1 0x239
#define MRF_UPNONCE0 0x240
#define MRF_UPNONCE1 0x241
#define MRF_UPNONCE2 0x242
#define MRF_UPNONCE3 0x243
#define MRF_UPNONCE4 0x244
#define MRF_UPNONCE5 0x245
#define MRF_UPNONCE6 0x246
#define MRF_UPNONCE7 0x247
#define MRF_UPNONCE8 0x248
#define MRF_UPNONCE9 0x249
#define MRF_UPNONCE10 0x24A
#define MRF_UPNONCE11 0x24B
#define MRF_UPNONCE12 0x24C

#define MRF_I_RXIF  0b00001000
#define MRF_I_TXNIF 0b00000001

// These are defines based on the 802.15.4 standard, hence the MAC prefix,
// rather than MRF_
#define MAC_FRAME_TYPE_BEACON 0
#define MAC_FRAME_TYPE_DATA 1
#define MAC_FRAME_TYPE_ACK 2
#define MAC_FRAME_TYPE_MACCMD 3

#define MAC_ADDRESS_MODE_NONE 0
#define MAC_ADDRESS_MODE_RESERVED 1
#define MAC_ADDRESS_MODE_16 2
#define MAC_ADDRESS_MODE_64 3

#define MAC_FRAME_VERSION_2003 0
#define MAC_FRAME_VERSION_2006 1

#include <stdbool.h>
#include <stdint.h>

    struct simrf_platform {
        void (*select) (bool value);
        void (*reset) (bool value);
        uint8_t (*spi_xfr) (uint8_t c);
        void (*delay_ms) (int value);
    };

typedef struct _simrf_rx_info {
    uint8_t frame_length;
    uint8_t frame_type:3;
    uint8_t security_enabled:1;
    uint8_t ack_bit:1;
    uint8_t pan_compression:1;
    uint8_t frame_version:2;
    uint8_t dest_addr_mode:2;
    uint8_t src_addr_mode:2;
    uint8_t sequence_number;
    uint8_t lqi;
    uint8_t rssi;
} simrf_rx_info_t;

/**
 * Based on the TXSTAT register, but "better"
 */
typedef struct _simrf_tx_info {
    uint8_t tx_ok:1;
    uint8_t retries:2;
    uint8_t channel_busy:1;
} simrf_tx_info_t;

/**
 * Set up the platform specific driver methods.
 * CALL THIS FIRST!
 * @param ptrs
 */
void simrf_setup(struct simrf_platform *ptrs);

/**
 * Hard reset via the /RESET line.
 * Resets, and handles the proper delays between pin changes for you.
 * If a reset pin handler wasn't provided, this function has no effect.
 */
void simrf_hard_reset(void);

/**
 * Politely ask the module to reset itself.
 * You may like to use this instead of hard_reset and save yourself a pin...
 */
void simrf_soft_reset(void);

/**
 * Resets all registers for proper operation.
 * Call this after any sort of hard reset, or power on.
 */
void simrf_init(void);

/**
 * Look up the PAN ID set for ourself
 * @return panid
 */
uint16_t simrf_pan_read(void);

/**
 * Set the PAN ID to use for sending.
 * @param panid
 */
void simrf_pan_write(uint16_t panid);

/**
 * Enables or disables promiscuous mode.
 * True means "Receive all packet types with good CRC"
 * False means "Discard packet when there is a MAC address mismatch,
 * illegal frame type, dPAN/sPAN or MAC short address mismatch"
 * @param enabled
 */
void simrf_promiscuous(uint8_t enabled);

/**
 * Set our own short (16bit) address to use for source address.
 * @param address16
 */
void simrf_address16_write(uint16_t address16);

/**
 * Read our own short (16bit) address in use presently.
 * @return 
 */
uint16_t simrf_address16_read(void);

/**
 * Set the channel to 12, 2.41Ghz, xbee channel 0xC
 * FIXME - this is hardcoded right now!!
 */
void mrf_set_channel(void);

/**
 * Simple send 16, with acks, not much of anything.. assumes src16 and local pan only.
 * 
 * @param dest16
 * @param len
 * @param data
 */
void simrf_send16(uint16_t dest16, uint8_t len, char *data);

/**
 * Call this from within an interrupt handler connected to the MRFs output
 * interrupt pin.  It handles reading in any data from the module, and letting it
 * continue working.
 * I haven't been able to reliably leave interrupts on, and throw away new packets
 * until the old one was read out by client software.  Until I can work that out,
 * I highly recommend disabling interrupts from module in your handle_rx callback.
 * Otherwise, you run the risk of having a new packet trample all over the current packet.
 * (TODO: why is this so hard to get right?!)
 *
 * Note, this is really only a problem in promiscuous mode...
 */
void simrf_interrupt_handler(void);

/**
 * Call this function periodically, it will invoke your nominated handlers
 * @param rx_handler
 * @param tx_handler
 */void simrf_check_flags(void (*rx_handler) (simrf_rx_info_t *rxinfo, uint8_t *rxbuffer),
                     void (*tx_handler) (simrf_tx_info_t *txinfo));
     
     /**
      * Put the module to sleep immediately
      */
     void simrf_immediate_sleep(void);
     
     /**
      * Wakeup via register.  This will block for 2ms.
      */
     void simrf_immediate_wakeup(void);


#ifdef	__cplusplus
}
#endif

#endif	/* LIB_MRF24J_H */

