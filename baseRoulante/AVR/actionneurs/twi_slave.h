#define TWI_BUFFER_SIZE 5

#define MASTER_CMD_AX1_GOTO1    0X11
#define MASTER_CMD_AX2_GOTO1    0X12
#define MASTER_CMD_AX1_GOTO2    0X13
#define MASTER_CMD_AX2_GOTO2    0X14
#define MASTER_CMD_AX1_GOTO3    0X15
#define MASTER_CMD_AX2_GOTO3    0X16
#define MASTER_CMD_AX1_GOTO4    0X17
#define MASTER_CMD_AX2_GOTO4    0X18
#define MASTER_CMD_SERVO1_UP    0X21
#define MASTER_CMD_SERVO2_UP    0X22
#define MASTER_CMD_SERVO1_DOWN  0X31
#define MASTER_CMD_SERVO2_DOWN  0X32
#define MASTER_CMD_ASC1_GOTO1   0X41
#define MASTER_CMD_ASC2_GOTO1   0X42
#define MASTER_CMD_ASC1_GOTO2   0X43
#define MASTER_CMD_ASC2_GOTO2   0X44
#define MASTER_CMD_ASC1_GOTO3   0X45
#define MASTER_CMD_ASC2_GOTO3   0X46
#define MASTER_CMD_ASC1_GOTO4   0X47
#define MASTER_CMD_ASC2_GOTO4   0X48
#define MASTER_CMD_ASCB_GOTO1   0X51
#define MASTER_CMD_ASCB_GOTO2   0X52
#define MASTER_CMD_ASCB_GOTO3   0X53
#define MASTER_CMD_ASCB_GOTO4   0X54
#define MASTER_CMD_STOP         0XA0
#define MASTER_CMD_RECAL        0XB0

extern unsigned char messageBuf[];

extern uint16_t ax_cons1;
extern uint16_t ax_cons2;

/****************************************************************************
 *  Function definitions
 ****************************************************************************/
void TWI_Loop( void );
void TWI_Init ( void );
void TWI_Slave_Initialise( unsigned char );
unsigned char TWI_Transceiver_Busy( void );
unsigned char TWI_Get_State_Info( void );
void TWI_Start_Transceiver_With_Data( unsigned char * , unsigned char );
void TWI_Start_Transceiver( void );
unsigned char TWI_Get_Data_From_Transceiver( unsigned char *, unsigned char );

/****************************************************************************
 *  Bit and byte definitions
 ****************************************************************************/
#define TWI_READ_BIT  0   // Bit position for R/W bit in "address byte".
#define TWI_ADR_BITS  1   // Bit position for LSB of the slave address bits in the init byte.
#define TWI_GEN_BIT   0   // Bit position for LSB of the general call bit in the init byte.

#define TRUE          1
#define FALSE         0

/****************************************************************************
 *  TWI State codes
 ****************************************************************************/
// General TWI Master staus codes                      
#define TWI_START                  0x08  // START has been transmitted  
#define TWI_REP_START              0x10  // Repeated START has been transmitted
#define TWI_ARB_LOST               0x38  // Arbitration lost

// TWI Master Transmitter staus codes                      
#define TWI_MTX_ADR_ACK            0x18  // SLA+W has been tramsmitted and ACK received
#define TWI_MTX_ADR_NACK           0x20  // SLA+W has been tramsmitted and NACK received 
#define TWI_MTX_DATA_ACK           0x28  // Data byte has been tramsmitted and ACK received
#define TWI_MTX_DATA_NACK          0x30  // Data byte has been tramsmitted and NACK received 

// TWI Master Receiver staus codes  
#define TWI_MRX_ADR_ACK            0x40  // SLA+R has been tramsmitted and ACK received
#define TWI_MRX_ADR_NACK           0x48  // SLA+R has been tramsmitted and NACK received
#define TWI_MRX_DATA_ACK           0x50  // Data byte has been received and ACK tramsmitted
#define TWI_MRX_DATA_NACK          0x58  // Data byte has been received and NACK tramsmitted

// TWI Slave Transmitter staus codes
#define TWI_STX_ADR_ACK            0xA8  // Own SLA+R has been received; ACK has been returned
#define TWI_STX_ADR_ACK_M_ARB_LOST 0xB0  // Arbitration lost in SLA+R/W as Master; own SLA+R has been received; ACK has been returned
#define TWI_STX_DATA_ACK           0xB8  // Data byte in TWDR has been transmitted; ACK has been received
#define TWI_STX_DATA_NACK          0xC0  // Data byte in TWDR has been transmitted; NOT ACK has been received
#define TWI_STX_DATA_ACK_LAST_BYTE 0xC8  // Last data byte in TWDR has been transmitted (TWEA = �0�); ACK has been received

// TWI Slave Receiver staus codes
#define TWI_SRX_ADR_ACK            0x60  // Own SLA+W has been received ACK has been returned
#define TWI_SRX_ADR_ACK_M_ARB_LOST 0x68  // Arbitration lost in SLA+R/W as Master; own SLA+W has been received; ACK has been returned
#define TWI_SRX_GEN_ACK            0x70  // General call address has been received; ACK has been returned
#define TWI_SRX_GEN_ACK_M_ARB_LOST 0x78  // Arbitration lost in SLA+R/W as Master; General call address has been received; ACK has been returned
#define TWI_SRX_ADR_DATA_ACK       0x80  // Previously addressed with own SLA+W; data has been received; ACK has been returned
#define TWI_SRX_ADR_DATA_NACK      0x88  // Previously addressed with own SLA+W; data has been received; NOT ACK has been returned
#define TWI_SRX_GEN_DATA_ACK       0x90  // Previously addressed with general call; data has been received; ACK has been returned
#define TWI_SRX_GEN_DATA_NACK      0x98  // Previously addressed with general call; data has been received; NOT ACK has been returned
#define TWI_SRX_STOP_RESTART       0xA0  // A STOP condition or repeated START condition has been received while still addressed as Slave

// TWI Miscellaneous status codes
#define TWI_NO_STATE               0xF8  // No relevant state information available; TWINT = �0�
#define TWI_BUS_ERROR              0x00  // Bus error due to an illegal START or STOP condition
