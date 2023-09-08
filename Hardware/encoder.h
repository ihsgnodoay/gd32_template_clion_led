#ifndef __ENCODER_H__ 
#define __ENCODER_H__


void encoder_init(void);
short get_encoder1();
short get_encoder2();

/**
返回累积的计数值
*/
int get_encoder();
void encoder_clear();

void encoder_test();

#endif