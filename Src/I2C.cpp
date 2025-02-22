#include "I2C.hpp"



void I2C_Init(){
    // DDRC  &= ~(1 << PC5 | 1 <<PC4);
    // PORTC &= ~(1 << PC5 | 1 <<PC4);

    TWBR = 0;
    TWSR |= (0 << TWPS1) | (0 << TWPS0);
    TWCR |= (1 << TWEN);
}

void I2C_writeToRegister(uint8_t dev, uint8_t adr, uint8_t data){
    I2C_beginTransition(dev);
    I2C_write(adr);
    I2C_write(data);
    I2C_endTransition();
}

int8_t I2C_readByte(uint8_t dev, uint8_t adr){
    uint8_t data;
    I2C_beginTransition(dev + WRITE);
    I2C_write(adr);
    I2C_beginTransition(dev + READ);
    data = I2C_read_NAK();
    I2C_endTransition();
    
    return data;
}

int16_t I2C_read2Byte(uint8_t dev, uint8_t adr){
    int16_t data;
    I2C_beginTransition(dev + WRITE);
    I2C_write(adr);
    I2C_beginTransition(dev + READ);
    data = I2C_read_AK() << 8;
    data |= I2C_read_NAK();

    return data;
}


void I2C_beginTransition(uint8_t address){
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while(!(TWCR & (1 << TWINT)));

    I2C_write(address);
}

void I2C_endTransition(){
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
    while ((TWCR & (1 << TWSTO)));
}


void I2C_write(uint8_t data){
    TWDR = data;
    TWCR = (1 << TWINT) | (1 <<TWEN);
    while(!(TWCR & (1 << TWINT)));
}


#if MULTI_READ
// void I2C_read(uint8_t *buffer, uint8_t bufferSize){
//     for(uint8_t i = 0; i < bufferSize-1; i++){
//         *buffer = I2C_read_AK();
//         buffer++;
//     }
//     *buffer = I2C_read_NAK();
// }


uint8_t I2C_read_AK(){
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

uint8_t I2C_read_NAK() {
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}
#else

uint8_t I2C_read(){
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

#endif
