// Karl Palsson, 2011
// false.ekta.is
// BSD/MIT license

#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "config.h"
#include "simrf.h"

// aMaxPHYPacketSize = 127, from the 802.15.4-2006 standard.
static uint8_t mrf_rx_buf[127];
static uint8_t simrf_sequence = 0;

volatile uint8_t flag_got_rx;
volatile uint8_t flag_got_tx;

static simrf_rx_info_t mrf_rx_info;
static simrf_tx_info_t mrf_tx_info;

static struct simrf_platform platform;

static uint8_t mrf_read_short(uint8_t address);
static uint8_t mrf_read_long(uint16_t address);
static void mrf_write_short(uint8_t address, uint8_t data);
static void mrf_write_long(uint16_t address, uint8_t data);

void simrf_setup(struct simrf_platform *ptrs)
{
	assert(ptrs->select);
	assert(ptrs->spi_xfr);
	assert(ptrs->delay_ms);
	//assert(ptrs->reset);  // reset is optional.
	memcpy(&platform, ptrs, sizeof (struct simrf_platform));
}

void simrf_hard_reset(void)
{
	if (platform.reset) {
		platform.reset(true);
		platform.delay_ms(10); // just my gut
		platform.reset(false);
		platform.delay_ms(20); // from manual
	}
}

void simrf_soft_reset(void)
{
	mrf_write_short(MRF_SOFTRST, 0x7); // from manual
	while ((mrf_read_short(MRF_SOFTRST) & 0x7) != 0) {
		; // wait for soft reset to finish
	}
}

uint16_t simrf_pan_read(void)
{
	uint8_t panh = mrf_read_short(MRF_PANIDH);
	return panh << 8 | mrf_read_short(MRF_PANIDL);
}

void simrf_pan_write(uint16_t panid)
{
	mrf_write_short(MRF_PANIDH, panid >> 8);
	mrf_write_short(MRF_PANIDL, panid & 0xff);
}

void simrf_address16_write(uint16_t address16)
{
	mrf_write_short(MRF_SADRH, address16 >> 8);
	mrf_write_short(MRF_SADRL, address16 & 0xff);
}

uint16_t simrf_address16_read(void)
{
	uint8_t a16h = mrf_read_short(MRF_SADRH);
	return a16h << 8 | mrf_read_short(MRF_SADRL);
}

void simrf_promiscuous(uint8_t enabled)
{
	// TODO - a tad ugly, this should really do a read modify write
	if (enabled) {
		mrf_write_short(MRF_RXMCR, 0x01);
	} else {
		mrf_write_short(MRF_RXMCR, 0x00);
	}
}

void simrf_send16(uint16_t dest16, uint8_t len, char * data)
{

	int i = 0;
	mrf_write_long(i++, 9); // header length
#if PAD_DIGI_HEADER
	mrf_write_long(i++, 9 + 2 + len);
#else
	mrf_write_long(i++, 9 + len);
#endif


	// 0 | pan compression | ack | no security | no data pending | data frame[3 bits]
	mrf_write_long(i++, 0b01100001); // first byte of Frame Control
	// 16 bit source, 802.15.4 (2003), 16 bit dest,
	mrf_write_long(i++, 0b10001000); // second byte of frame control
	mrf_write_long(i++, simrf_sequence++);

	uint16_t panid = simrf_pan_read(); // TODO Inefficient for perf, good for memory?

	mrf_write_long(i++, panid & 0xff); // dest panid
	mrf_write_long(i++, panid >> 8);
	mrf_write_long(i++, dest16 & 0xff); // dest16 low
	mrf_write_long(i++, dest16 >> 8); // dest16 high

	uint16_t src16 = simrf_address16_read(); // TODO likewise...
	mrf_write_long(i++, src16 & 0xff); // src16 low
	mrf_write_long(i++, src16 >> 8); // src16 high

#if PAD_DIGI_HEADER
	i += 2;
#endif
	for (int q = 0; q < len; q++) {
		mrf_write_long(i++, data[q]);
	}
	// ack on, and go!
	mrf_write_short(MRF_TXNCON, (1 << MRF_TXNACKREQ | 1 << MRF_TXNTRIG));
}

void mrf_set_interrupts(void)
{
	// interrupts for rx and tx normal complete
	mrf_write_short(MRF_INTCON, 0b11110110);
}

void mrf_set_channel(void)
{
	mrf_write_long(MRF_RFCON0, 0x13);
}

void simrf_init(void)
{
	mrf_write_short(MRF_PACON2, 0x98); // – Initialize FIFOEN = 1 and TXONTS = 0x6.
	mrf_write_short(MRF_TXSTBL, 0x95); // – Initialize RFSTBL = 0x9.

	mrf_write_long(MRF_RFCON0, 0x03); // – Initialize RFOPT = 0x03.
	mrf_write_long(MRF_RFCON1, 0x01); // – Initialize VCOOPT = 0x02.
	mrf_write_long(MRF_RFCON2, 0x80); // – Enable PLL (PLLEN = 1).
	mrf_write_long(MRF_RFCON6, 0x90); // – Initialize TXFIL = 1 and 20MRECVR = 1.
	mrf_write_long(MRF_RFCON7, 0x80); // – Initialize SLPCLKSEL = 0x2 (100 kHz Internal oscillator).
	mrf_write_long(MRF_RFCON8, 0x10); // – Initialize RFVCO = 1.
	mrf_write_long(MRF_SLPCON1, 0x21); // – Initialize CLKOUTEN = 1 and SLPCLKDIV = 0x01.

	//  Configuration for nonbeacon-enabled devices (see Section 3.8 “Beacon-Enabled and Nonbeacon-Enabled Networks”):
	mrf_write_short(MRF_BBREG2, 0x80); // Set CCA mode to ED
	mrf_write_short(MRF_CCAEDTH, 0x60); // – Set CCA ED threshold.
	mrf_write_short(MRF_BBREG6, 0x40); // – Set appended RSSI value to RXFIFO.
	mrf_set_interrupts();
	mrf_set_channel();
	// max power is by default.. just leave it...
	//Set transmitter power - See “REGISTER 2-62: RF CONTROL 3 REGISTER (ADDRESS: 0x203)”.
	mrf_write_short(MRF_RFCTL, 0x04); //  – Reset RF state machine.
	mrf_write_short(MRF_RFCTL, 0x00); // part 2
	// FIXME - _delay_us(500); // delay at least 192usec
	platform.delay_ms(1);
}

void simrf_interrupt_handler(void)
{
	uint8_t last_interrupt = mrf_read_short(MRF_INTSTAT);
	if (last_interrupt & MRF_I_RXIF) {
		if (flag_got_rx > 0) {
			// discard new packets until they've processed the last one :|
			// This is called for every interrupt, but without resetting the
			// rx fifo pointers, you won't actually receive any new packets!
			mrf_write_short(MRF_RXFLUSH, 0x01);
			return;
		}
		flag_got_rx++;
		// read out the packet data...
		mrf_write_short(MRF_BBREG1, 0x04); // RXDECINV - disable receiver
		// m + n + 2 from datasheet
		uint8_t frame_length = mrf_read_long(0x300);

		uint16_t frame_control = mrf_read_long(0x301);
		frame_control |= mrf_read_long(0x302) << 8;
		mrf_rx_info.fc_raw = frame_control;
		mrf_rx_info.frame_type = frame_control & 0x07;
		mrf_rx_info.pan_compression = (frame_control >> 6) & 0x1;
		mrf_rx_info.ack_bit = (frame_control >> 5) & 0x1;
		mrf_rx_info.dest_addr_mode = (frame_control >> 10) & 0x3;
		mrf_rx_info.frame_version = (frame_control >> 12) & 0x3;
		mrf_rx_info.src_addr_mode = (frame_control >> 14) & 0x3;
		mrf_rx_info.sequence_number = mrf_read_long(0x303);

		// only three bytes have been removed, frame control and sequence id
		// the data starts at 4 though, because byte 0 was the mrf length
		for (int i = 0; i <= frame_length - 4; i++) {
			mrf_rx_buf[i] = mrf_read_long(0x304 + i);
		}
		mrf_rx_info.frame_length = frame_length - 3 - 2;
		mrf_rx_info.lqi = mrf_read_long(0x301 + frame_length);
		mrf_rx_info.rssi = mrf_read_long(0x301 + frame_length + 1);

		mrf_write_short(MRF_BBREG1, 0x00); // RXDECINV - enable receiver
	}
	if (last_interrupt & MRF_I_TXNIF) {
		flag_got_tx++;
		uint8_t tmp = mrf_read_short(MRF_TXSTAT);
		// 1 means it failed, we want 1 to mean it worked.
		mrf_tx_info.tx_ok = !(tmp & ~(1 << TXNSTAT));
		mrf_tx_info.retries = tmp >> 6;
		mrf_tx_info.channel_busy = (tmp & (1 << CCAFAIL));
	}
}

void simrf_check_flags(void (*rx_handler) (simrf_rx_info_t *rxinfo, uint8_t *rxbuffer),
	void (*tx_handler) (simrf_tx_info_t *txinfo))
{
	// TODO - we could check whether the flags are > 1 here, indicating data was lost?
	if (flag_got_rx) {
		flag_got_rx = 0;
		if (rx_handler) {
			rx_handler(&mrf_rx_info, mrf_rx_buf);
		}
	}
	if (flag_got_tx) {
		flag_got_tx = 0;
		if (tx_handler) {
			tx_handler(&mrf_tx_info);
		}
	}
}

void simrf_immediate_sleep(void)
{
	uint16_t tmp = mrf_read_short(MRF_WAKECON);
	tmp |= MRF_WAKECON_IMMWAKE;
	mrf_write_short(MRF_WAKECON, tmp);

	tmp = mrf_read_short(MRF_SOFTRST);
	tmp |= MRF_SOFTRST_RSTPWR;
	mrf_write_short(MRF_SOFTRST, tmp);

	tmp = mrf_read_short(MRF_SLPACK);
	tmp |= MRF_SLPACK_SLPACK;
	mrf_write_short(MRF_SLPACK, tmp);
}

void simrf_rf_reset(void)
{
	// From datasheet... this will trash a wakecount if anyone sent it...
	// Recommended sequence RFCTL = 0x06 (reset mode) then RFCTL = 0x02 (transmit mode)
	uint16_t tmp = mrf_read_short(MRF_RFCTL);
	//	tmp |= 0x6;
	tmp |= MRF_RFCTL_RFRST;
	mrf_write_short(MRF_RFCTL, tmp);
	tmp &= ~MRF_RFCTL_RFRST;
	//	tmp &= ~(0x6);
	//	tmp |= 0x2;
	mrf_write_short(MRF_RFCTL, tmp);
	// TODO or.... user warning... wait 192uSec after this...
}

void simrf_immediate_wakeup(void)
{
	uint16_t tmp = mrf_read_short(MRF_WAKECON);
	mrf_write_short(MRF_WAKECON, tmp);
	tmp |= MRF_WAKECON_REGWAKE;
	mrf_write_short(MRF_WAKECON, tmp);
	platform.delay_ms(1);
	tmp &= ~MRF_WAKECON_REGWAKE;
	mrf_write_short(MRF_WAKECON, tmp);

	//This seems to make it only sometimes wakeup.  Could be timing, could just be bullshit
	//	simrf_rf_reset();
	platform.delay_ms(2);
}

/// PRIVATE INTERNALS

static uint8_t mrf_read_short(uint8_t address)
{
	platform.select(true);
	// 0 top for short addressing, 0 bottom for read
	platform.spi_xfr(address << 1 & 0b01111110);
	uint8_t res = platform.spi_xfr(0x0);
	platform.select(false);
	return res;
}

static uint8_t mrf_read_long(uint16_t address)
{
	platform.select(true);
	uint8_t ahigh = address >> 3;
	uint8_t alow = address << 5;
	platform.spi_xfr(0x80 | ahigh); // high bit for long
	platform.spi_xfr(alow);
	uint8_t res = platform.spi_xfr(0);
	platform.select(false);
	return res;
}

static void mrf_write_short(uint8_t address, uint8_t data)
{
	platform.select(true);
	// 0 for top address, 1 bottom for write
	platform.spi_xfr((address << 1 & 0b01111110) | 0x01);
	platform.spi_xfr(data);
	platform.select(false);
}

static void mrf_write_long(uint16_t address, uint8_t data)
{
	platform.select(true);
	uint8_t ahigh = address >> 3;
	uint8_t alow = address << 5;
	platform.spi_xfr(0x80 | ahigh); // high bit for long
	platform.spi_xfr(alow | 0x10); // last bit for write
	platform.spi_xfr(data);
	platform.select(false);
}

