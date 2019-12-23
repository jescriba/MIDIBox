#include <avr/io.h>
#include "lib/pinDefines.h"

#define SLAVE_SELECT    SPI_SS_PORT &= ~(1 << SPI_SS)
#define SLAVE_DESELECT  SPI_SS_PORT |= (1 << SPI_SS)

/* Address for digit and control registers on AS1108 */
#define NO_OP_ADDRESS   0b00000000
#define DIGIT_0_ADDRESS 0b00000001
#define DIGIT_1_ADDRESS 0b00000010
#define DIGIT_2_ADDRESS 0b00000011
#define DIGIT_3_ADDRESS 0b00000100
#define DECODE_MODE_ADDRESS 0b00001001
#define INTENSITY_CONTROL_ADDRESS 0b00001010
#define SCAN_LIMIT_ADDRESS 0b00001011
#define SHUTDOWN_ADDRESS 0b00001100
#define FEATURE_ADDRESS 0b00001110
#define DISPLAY_TEST_ADDRESS 0b00001111

/* Bit locations for Feature register values */
#define FEATURE_BLINK_START 7
#define FEATURE_BLINK_SYNC 6
#define FEATURE_BLINK_FREQ_SELECT 5
#define FEATURE_BLINK_ENABLE 4
#define FEATURE_SPI_ENABLE 3
#define FEATURE_DECODE_SELECT 2
#define FEATURE_CONTROL_REGISTER_RESET 1
#define FEATURE_CLOCK_ENABLE 0


// Note: AS1108 initializes to default settings - scanning only one digit, internal decoder is disabled, and Intensity Control Register set to minimum values

// Functions

                  /* Init SPI to run EEPROM with phase, polarity = 0,0 */
void initSPI(void);

                  /* Generic.  Just loads up HW SPI register and waits */
void SPI_tradeByte(uint8_t byte);

                  /* writes byte of data at byte addresss for digit and control register.
		   * Note: first nibble is DONT CARE on address */
void DISPLAY_writeByte(uint8_t address, uint8_t byte);

void DISPLAY_writeChar(uint8_t address, char val);

void DISPLAY_writeChars(char str[]);

uint8_t DISPLAY_byteForChar(char val);

/*  Bring the chip out of shutdown mode */
void DISPLAY_setupNormalOperation();

/* Bring chip out of shutdown mode but reset feature register */
void DISPLAY_setupNormalOperationResetFeature();

 /* Display test mode writes all segments and digits */
void DISPLAY_test();

 /* Set display to no-decode mode */
void DISPLAY_setNoDecodeMode();

 /* Set display to Code-B/Hex decoding for all digits */
void DISPLAY_setDecodeAllMode();

 /* Set scan limit aka number of digits to display. Only the last 3 bits matter.
    Valid values: 0-3 for number of digits */
void DISPLAY_setScanLimit(uint8_t digitLimit);

// MARK: Intensity controls

 /* Sets the brightness with 0x00 being min and 0xFF being highest. MSB nibble ignored. */
void DISPLAY_setBrightness(uint8_t byte);

// MARK: Feature Register commands

 /* Sets display to use Code-B assuming decode mode is set */
void DISPLAY_setCodeBMode();

 /* Sets display to use Hex decode assuming decode mode is set */
void DISPLAY_setHexDecodeMode();

 /* Enable SPI-mode */
void DISPLAY_setSPIEnable();

 /* Blink enable */
void DISPLAY_enableBlinking();

 /* Blink disable */
void DISPLAY_disableBlinking();

/* Set blink period to 2s. */
void DISPLAY_setLowBlinkFrequency();

 /* Set blink period to 1s. This is default value */
void DISPLAY_setHighBlinkFrequency();

 /* Reset control register to default */
 void DISPLAY_reset();

 /* Set external clock */
 void DISPLAY_setExternalClock();

 /* Set to use internal oscillator as clock */
 void DISPLAY_setInternalClock();

 /* MARK: Convenience */
 void DISPLAY_defaultConfig();

 void DISPLAY_defaultTest();
