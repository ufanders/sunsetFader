#define DESIRED_BAUDRATE (250000) //250kbps for DMX-512
#define UART_RX_INT_EN PIE1bits.RCIE

//DMX parameters
#define DMX_ADDRESS 1
#define DMX_CHANNELS 8 //use at least 2 channels
enum {IDLE, BREAK, MARK_AFTER_BREAK, STARTB, STARTADR}; //DMX states

void DmxInit(void);
void DmxUpdate(void);
unsigned char DmxDidChange(void);
void DmxConsumeChange(void);
void DMX_ISR(void);
unsigned char DmxAddressSet(unsigned int);
unsigned int DmxAddressGet(void);
unsigned char DmxFieldGet(unsigned char);
