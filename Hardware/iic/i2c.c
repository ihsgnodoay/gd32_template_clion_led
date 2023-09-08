#include "i2c.h"

void I2C_init() {
		uint32_t i2cx_scl_port_rcu = RCU_GPIOB;
    uint32_t i2cx_scl_port = GPIOB;
    uint32_t i2cx_scl_pin = GPIO_PIN_10;
    uint32_t i2cx_scl_af = GPIO_AF_4;

    uint32_t i2cx_sda_port_rcu = RCU_GPIOB;
    uint32_t i2cx_sda_port = GPIOB;
    uint32_t i2cx_sda_pin = GPIO_PIN_11;
    uint32_t i2cx_sda_af = GPIO_AF_4;

    uint32_t i2cx = I2C1;
    uint32_t i2cx_rcu = RCU_I2C1;
    uint32_t i2cx_speed = 400000;
    /****************** GPIO config **********************/
    // ʱ������
    rcu_periph_clock_enable(i2cx_scl_port_rcu);
		// ���ø��ù���
    gpio_af_set(i2cx_scl_port, i2cx_scl_af, i2cx_scl_pin);
		// �������ģʽ
    gpio_mode_set(i2cx_scl_port, GPIO_MODE_AF, GPIO_PUPD_NONE, i2cx_scl_pin);
    gpio_output_options_set(i2cx_scl_port, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, i2cx_scl_pin);
    
    // ʱ������
    rcu_periph_clock_enable(i2cx_sda_port_rcu);
    // ���ø��ù���
    gpio_af_set(i2cx_sda_port, i2cx_sda_af, i2cx_sda_pin);
		// �������ģʽ
    gpio_mode_set(i2cx_sda_port, GPIO_MODE_AF, GPIO_PUPD_NONE, i2cx_sda_pin);
    gpio_output_options_set(i2cx_sda_port, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, i2cx_sda_pin);
    
    /****************** I2C config  **********************/
    i2c_deinit(i2cx);
    // ʱ������
    rcu_periph_clock_enable(i2cx_rcu);
    // I2C��������
    i2c_clock_config(i2cx, i2cx_speed, I2C_DTCY_2);

    // ʹ��i2c
    i2c_mode_addr_config(i2cx, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0x00);
    i2c_enable(i2cx);

    // i2c ack enable
		i2c_ack_config(i2cx, I2C_ACK_ENABLE);
    //i2c_ackpos_config(i2cx, I2C_ACKPOS_CURRENT);
	
}

static uint8_t I2C_wait(uint32_t i2cx, uint32_t flag) {
    uint16_t TIMEOUT = 50000;
    uint16_t cnt = 0;

    while(!i2c_flag_get(i2cx, flag)) {
        cnt++;
        if(cnt > TIMEOUT) return 1;
    }
    return 0;
}

static uint8_t I2C_waitn(uint32_t i2cx, uint32_t flag) {
    uint16_t TIMEOUT = 50000;
    uint16_t cnt = 0;

    while(i2c_flag_get(i2cx, flag)) {
        cnt++;
        if(cnt > TIMEOUT) return 1;
    }
		return 0;
}


uint8_t I2C_write(uint8_t addr, uint8_t reg, uint8_t* data, uint32_t data_len) {
		uint32_t i2cx = I2C1;
    uint8_t address = addr << 1;
		
		/************* start ***********************/
		// �ȴ�I2C����
    if(I2C_waitn(i2cx, I2C_FLAG_I2CBSY)) return 1;
	
    // start
    i2c_start_on_bus(i2cx);
		
		 // �ȴ�I2C���豸�ɹ�������ʼ�ź�
    if(I2C_wait(i2cx, I2C_FLAG_SBSEND)) return 2;
	
		/************* device address **************/
    // �����豸��ַ
    i2c_master_addressing(i2cx, address, I2C_TRANSMITTER);
		 // �ȴ���ַ�������
    if(I2C_wait(i2cx, I2C_FLAG_ADDSEND)) return 3;
    i2c_flag_clear(i2cx, I2C_FLAG_ADDSEND);
	
		/************ register address ************/
    // �Ĵ�����ַ
    // �ȴ��������ݻ�����Ϊ��
    if(I2C_wait(i2cx, I2C_FLAG_TBE)) return 4;
		
		// ��������
    i2c_data_transmit(i2cx, reg);

    // �ȴ����ݷ������
    if(I2C_wait(i2cx, I2C_FLAG_BTC)) return 5;
		
		/***************** data ******************/
    // ��������
    uint32_t i;
    for(i = 0; i < data_len; i++) {
        uint32_t d = data[i];

        // �ȴ��������ݻ�����Ϊ��
        if(I2C_wait(i2cx, I2C_FLAG_TBE)) return 6;

        // ��������
        i2c_data_transmit(i2cx, d);

        // �ȴ����ݷ������
        if(I2C_wait(i2cx, I2C_FLAG_BTC)) return 7;
    }
		
		/***************** stop ********************/
    // stop
    i2c_stop_on_bus(i2cx);
    if(I2C_waitn(i2cx, I2C_CTL0(I2C1)&I2C_CTL0_STOP)) return 8;
	
		return 0;
}

uint8_t I2C_write_byte(uint8_t addr, uint8_t reg, uint8_t data){
	return I2C_write(addr,reg,&data,1);
}







void I2C_deinit() {
	
}


uint8_t I2C_read(uint8_t addr, uint8_t reg, uint8_t* data, uint32_t len) {
	

		uint32_t i2cx = I2C1;
		uint8_t address = addr << 1;
		
		/************* start ***********************/
    // �ȴ�I2C����
		if(I2C_waitn(i2cx, I2C_FLAG_I2CBSY)) return 1;

    // ���������ź�
    i2c_start_on_bus(i2cx);
		if(I2C_wait(i2cx, I2C_FLAG_SBSEND)) return 2;

		/************* device address **************/
    // ���ʹ��豸��ַ
    i2c_master_addressing(i2cx, address, I2C_TRANSMITTER);
		
//		//ack
//		i2c_ack_config(i2cx, I2C_ACK_ENABLE);
//		i2c_ackpos_config(i2cx, I2C_ACKPOS_CURRENT);
//		if(I2C_wait(i2cx, (I2C_CTL0(i2cx) & I2C_CTL0_ACKEN))) return 11;
//		// i2c_ack_config(i2cx, I2C_ACK_DISABLE);
		
		if(I2C_wait(i2cx, I2C_FLAG_ADDSEND)) return 3;
		i2c_flag_clear(i2cx, I2C_FLAG_ADDSEND);

		/********** register address **************/
		// �ȴ����ͻ�����	
    if(I2C_wait(i2cx, I2C_FLAG_TBE)) return 4;
    
		// ���ͼĴ�����ַ
    i2c_data_transmit(i2cx, reg);
		
		// �ȴ������������	
    if(I2C_wait(i2cx, I2C_FLAG_BTC)) return 5;
		
//		//ack
//		i2c_ack_config(i2cx, I2C_ACK_ENABLE);
//		i2c_ackpos_config(i2cx, I2C_ACKPOS_CURRENT);
//		if(I2C_wait(i2cx, (I2C_CTL0(i2cx) & I2C_CTL0_ACKEN))) return 21;
		
		if(I2C_wait(i2cx, I2C_FLAG_TBE)) return 6;

		/************* start ***********************/
    // �����������ź�
    i2c_start_on_bus(i2cx);
		
//		//ack
//		i2c_ack_config(i2cx, I2C_ACK_ENABLE);
//		i2c_ackpos_config(i2cx, I2C_ACKPOS_CURRENT);
//		if(I2C_wait(i2cx, (I2C_CTL0(i2cx) & I2C_CTL0_ACKEN))) return 22;
		
		if(I2C_wait(i2cx, I2C_FLAG_SBSEND)) return 7;

		/************* device address **************/
    // ���ʹ��豸��ַ
    i2c_master_addressing(i2cx, address, I2C_RECEIVER);
		if(I2C_wait(i2cx, I2C_FLAG_ADDSEND)) return 8;
		i2c_flag_clear(i2cx, I2C_FLAG_ADDSEND);

		//ack
		i2c_ack_config(i2cx, I2C_ACK_ENABLE);
		i2c_ackpos_config(i2cx, I2C_ACKPOS_CURRENT);
		if(I2C_wait(i2cx, (I2C_CTL0(i2cx) & I2C_CTL0_ACKEN))) return 23;

    // ��ȡ����
		uint8_t i;
    for (i = 0; i < len; i++) {
				if(i != len - 1) {
						// �ȴ����ջ�����	
						if(I2C_wait(i2cx, I2C_FLAG_BTC)) return 9;
				}
			
				// �ȴ�ACK���ݷ������
				// �ȴ����ջ�����	
				if(I2C_wait(i2cx, I2C_FLAG_RBNE)) return 10;
        data[i] = i2c_data_receive(i2cx);
			
        if (i == len - 1) {
            // �ڶ�ȡ���һ���ֽ�֮ǰ������ACK��������ֹͣ�ź�
						// �����Զ�NACK
						//i2c_ackpos_config(i2cx, I2C_ACKPOS_NEXT);
						//if(I2C_wait(i2cx, (I2C_CTL0(i2cx) & I2C_CTL0_ACKEN))) return 9;
						i2c_ack_config(i2cx, I2C_ACK_DISABLE);
        }
    }
		
		i2c_stop_on_bus(i2cx);
		if(I2C_waitn(i2cx, I2C_CTL0(I2C1) & I2C_CTL0_STOP)) return 11;
    return 0;
}



uint8_t I2C_read_byte(uint8_t addr, uint8_t reg){
	uint8_t data;
	I2C_read(addr,reg,&data,1);
	return data;
}