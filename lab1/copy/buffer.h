//
// Created by nininini on 17-12-8.
//

#ifndef LAB3_BUFFER_H
#define LAB3_BUFFER_H

#define BUFF_LENGTH 128


typedef struct buff {
    unsigned char data[BUFF_LENGTH];
    /*** flag : indicate the index where the readBuff ends ***/
    int flag ;
} buffer;


#endif //LAB3_BUFFER_H
