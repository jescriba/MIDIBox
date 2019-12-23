#include "AS1108.h"
#include <util/delay.h>

void initSPI(void) {
  SPI_SS_DDR |= (1 << SPI_SS);                        /* set SS output */
  SPI_SS_PORT |= (1 << SPI_SS);       /* start off not selected (high) */

  SPI_MOSI_DDR |= (1 << SPI_MOSI);                   /* output on MOSI */
  SPI_MISO_PORT |= (1 << SPI_MISO);                  /* pullup on MISO */
  SPI_SCK_DDR |= (1 << SPI_SCK);                      /* output on SCK */

  /* Don't have to set phase, polarity b/c
   * default works with 25LCxxx chips */
  SPCR |= (1 << SPR1);                /* div 16, safer for breadboards */
  SPCR |= (1 << MSTR);                                  /* clockmaster */
  SPCR |= (1 << SPE);                                        /* enable */
}

void SPI_tradeByte(uint8_t byte) {
  SPDR = byte;                       /* SPI starts sending immediately */
  loop_until_bit_is_set(SPSR, SPIF);                /* wait until done */
                                /* SPDR now contains the received byte */
}

void DISPLAY_writeByte(uint8_t address, uint8_t byte) {
  SLAVE_SELECT;
  SPI_tradeByte(address);
  SPI_tradeByte(byte);
  SLAVE_DESELECT;
}

void DISPLAY_writeChar(uint8_t address, char val) {
  uint8_t byteVal = DISPLAY_byteForChar(val);
  DISPLAY_writeByte(address, byteVal);
}

uint8_t addressForDigit(int i) {
  switch (i) {
    case 0:
      return DIGIT_0_ADDRESS;
    case 1:
      return DIGIT_1_ADDRESS;
    case 2:
      return DIGIT_2_ADDRESS;
    case 3:
      return DIGIT_3_ADDRESS;
    default:
      return DIGIT_3_ADDRESS;
  }
}

void DISPLAY_writeChars(char str[]) {
  for(int i = 0; i < 4; i++) {
    char c = DISPLAY_byteForChar(str[i]);
    DISPLAY_writeByte(addressForDigit(i), c);
  }
}

void DISPLAY_setupNormalOperation() {
  DISPLAY_writeByte(SHUTDOWN_ADDRESS, 0x81);
}

void DISPLAY_setupNormalOperationResetFeature() {
  DISPLAY_writeByte(SHUTDOWN_ADDRESS, 0x01);
}

void DISPLAY_test() {
  // Note: these data byte values just come straight from the AS1108 datasheet.
  DISPLAY_writeByte(DISPLAY_TEST_ADDRESS, 0xFF);
}

void DISPLAY_setNoDecodeMode() {
  DISPLAY_writeByte(DECODE_MODE_ADDRESS, 0x00);
}

void DISPLAY_setDecodeAllMode() {
  DISPLAY_writeByte(DECODE_MODE_ADDRESS, 0x0F);
}

void DISPLAY_setScanLimit(uint8_t byte) {
  DISPLAY_writeByte(SCAN_LIMIT_ADDRESS, byte);
}

// MARK: Intensity control register
void DISPLAY_setBrightness(uint8_t byte) {
  DISPLAY_writeByte(INTENSITY_CONTROL_ADDRESS, byte);
}

// MARK: Feature Register controls

/* Note: The feature register is byte initialized to 0 on power
 up so I have a global to prevent overwriting existing values
 when using the functions below */
uint8_t featureData = 0x00;

void DISPLAY_setCodeBMode() {
  // bit-wise trick to set 0 at feature decode select bit without changing other bites
  featureData &= ~(1 << FEATURE_DECODE_SELECT);
  DISPLAY_writeByte(FEATURE_ADDRESS, featureData);
}

void DISPLAY_setHexDecodeMode() {
  featureData |= (1 << FEATURE_DECODE_SELECT);
  DISPLAY_writeByte(FEATURE_ADDRESS, featureData);
}

void DISPLAY_setSPIEnable() {
  featureData |= (1 << FEATURE_SPI_ENABLE);
  DISPLAY_writeByte(FEATURE_ADDRESS, featureData);
}

void DISPLAY_enableBlinking() {
  featureData |= (1 << FEATURE_BLINK_ENABLE);
  DISPLAY_writeByte(FEATURE_ADDRESS, featureData);
}

void DISPLAY_disableBlinking() {
  featureData &= ~(1 << FEATURE_BLINK_ENABLE);
  DISPLAY_writeByte(FEATURE_ADDRESS, featureData);
}

void DISPLAY_setLowBlinkFrequency() {
  featureData |= (1 << FEATURE_BLINK_FREQ_SELECT);
  DISPLAY_writeByte(FEATURE_ADDRESS, featureData);
}

void DISPLAY_setHighBlinkFrequency() {
  featureData &= ~(1 << FEATURE_BLINK_FREQ_SELECT);
  DISPLAY_writeByte(FEATURE_ADDRESS, featureData);
}

void DISPLAY_reset() {
  featureData |= (1 << FEATURE_CONTROL_REGISTER_RESET);
  DISPLAY_writeByte(FEATURE_ADDRESS, featureData);
}

void DISPLAY_setExternalClock() {
  featureData |= (1 << FEATURE_CLOCK_ENABLE);
  DISPLAY_writeByte(FEATURE_ADDRESS, featureData);
}

void DISPLAY_setInternalClock() {
  featureData &= ~(1 << FEATURE_CLOCK_ENABLE);
  DISPLAY_writeByte(FEATURE_ADDRESS, featureData);
}

void DISPLAY_defaultConfig() {
  DISPLAY_setupNormalOperationResetFeature();
  DISPLAY_setScanLimit(3);
  DISPLAY_setBrightness(0x01);
  DISPLAY_setNoDecodeMode();
}

void DISPLAY_defaultTest() {
  for (int j = 0; j < 9; j++) {
    DISPLAY_writeByte(DIGIT_0_ADDRESS, 1 << j);
    DISPLAY_writeByte(DIGIT_1_ADDRESS, 1 << j);
    DISPLAY_writeByte(DIGIT_2_ADDRESS, 1 << j);
    DISPLAY_writeByte(DIGIT_3_ADDRESS, 1 << j);
    _delay_ms(200);
  }
}

// MARK: No-decode digit and other values byte representations
uint8_t DISPLAY_byteForChar(char val) {
  switch (val)
  {
    case 'a':
      return 0b01111101;
    case 'A':
      return 0b01110111;
    case 'b':
      return 0b00011111;
    case 'B':
      return 0b01111111;
    case 'c':
      return 0b00001101;
    case 'C':
      return 0b01001110;
    case 'd':
      return 0b00111101;
    case 'e':
      return 0b01101111;
    case 'E':
      return 0b01001111;
    case 'f':
    case 'F':
      return 0b01000111;
    case 'g':
      return 0b01111011;
    case 'G':
      return 0b01011110;
    case 'o':
      return 0b00011101;
    case 'L':
      return 0b00001110;
    case 'h':
      return 0b00010111;
    case 'H':
      return 0b00110111;
    case 'D':
    case 'O':
    case '0':
      return 0b01111110;
    case 'l':
    case '1':
      return 0b00110000;
    case '2':
      return 0b01101101;
    case '3':
      return 0b01111001;
    case '4':
      return 0b00110011;
    case '5':
      return 0b01011011;
    case '6':
      return 0b01011111;
    case '7':
      return 0b01110000;
    case '8':
      return 0b01111111;
    case '9':
      return 0b01110011;
    case '-':
      return 0b00000001;
    default:
      return 0;
  }
}
