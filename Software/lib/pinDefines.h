// ---------------
//   Pin Defines
// ---------------

#define SWITCH_PORT             PORTD
#define SWITCH_PIN              PIND
#define SWITCH_DDR              DDRD

#define SWITCH                  PD2

#define SPEAKER                 PD6                            /* OC0A */
#define SPEAKER_PORT            PORTD
#define SPEAKER_PIN             PIND
#define SPEAKER_DDR             DDRD

#define POT                     PC0                            /* ADC0 */
#define POT_PORT                PORTC
#define POT_PIN                 PINC
#define POT_DDR                 DDRC

//  SPI mode defines

#define SPI_SS                     PB2
#define SPI_SS_PORT                PORTB
#define SPI_SS_PIN                 PINB
#define SPI_SS_DDR                 DDRB

#define SPI_MOSI                     PB3
#define SPI_MOSI_PORT                PORTB
#define SPI_MOSI_PIN                 PINB
#define SPI_MOSI_DDR                 DDRB

#define SPI_MISO                     PB4
#define SPI_MISO_PORT                PORTB
#define SPI_MISO_PIN                 PINB
#define SPI_MISO_DDR                 DDRB

#define SPI_SCK                     PB5
#define SPI_SCK_PORT                PORTB
#define SPI_SCK_PIN                 PINB
#define SPI_SCK_DDR                 DDRB
