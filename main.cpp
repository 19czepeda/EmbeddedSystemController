#include "mbed.h"
#include "tsi_sensor.h"
#include "SLCD.h"
#include "nRF24L01P.h"

Serial pc(USBTX,USBRX);
DigitalOut led1(LED1);
DigitalOut led2(LED2);
TSIAnalogSlider touchsensor(PTB16, PTB17, 40);
SLCD display;
nRF24L01P my_nrf24l01p(PTA16,PTA17,PTA15,PTD2,PTA13,PTD4);
  
int main() {
    
    #define TRANSFER_SIZE 3

    //char rxData[TRANSFER_SIZE];
    //int rxDataCnt = 0;
    char txData[TRANSFER_SIZE];
    int txDataCnt = 0;
   

    // initialize wireless comm.
    my_nrf24l01p.powerUp();
    my_nrf24l01p.setTransferSize( TRANSFER_SIZE );
    my_nrf24l01p.setReceiveMode();
    my_nrf24l01p.enable();
    my_nrf24l01p.setAirDataRate(2000);
    // set wireless RX
    //my_nrf24l01p.setRxAddress(0xABCDABCDABCDABCD);
     my_nrf24l01p.setTxAddress(0xEFEFEFEFEFEFEFEF);
    // Display the (default) setup of the nRF24L01+ chip
    //pc.printf( "nRF24L01+ Frequency    : %d MHz\r\n",  my_nrf24l01p.getRfFrequency() );
    //pc.printf( "nRF24L01+ Output power : %d dBm\r\n",  my_nrf24l01p.getRfOutputPower() );
    //pc.printf( "nRF24L01+ Data Rate    : %d kbps\r\n", my_nrf24l01p.getAirDataRate() );
    //pc.printf( "nRF24L01+ TX Address   : 0x%010llX\r\n", my_nrf24l01p.getTxAddress() );
    //pc.printf( "nRF24L01+ RX Address   : 0x%010llX\r\n", my_nrf24l01p.getRxAddress() );
       while (1) {
        // If we've received anything over the host serial link...
 
            // ...add it to the transmit buffer
           
            float touchval = touchsensor.readPercentage();
            int move = touchval * 255;
            txData[txDataCnt++] = 5;
            txData[txDataCnt++] = 0;
            txData[txDataCnt++] = move;
            //printf(txData);
 
            // If the transmit buffer is full
            if ( txDataCnt >= sizeof( txData ) ) {
                led1 = !led1;
 
                // Send the transmitbuffer via the nRF24L01+
                my_nrf24l01p.write( NRF24L01P_PIPE_P0, txData, txDataCnt );
 
                txDataCnt = 0;
            }
            wait(1);

            // Toggle LED1 (to help debug Host -> nRF24L01+ communication)
     
    }
}
