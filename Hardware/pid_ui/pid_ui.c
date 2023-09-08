#include "pid_ui.h"

//���°�ť����ص�ָ��
void (*update_func)(float pos_p,float pos_i,float pos_d,float inc_p,float inc_i,float inc_d);
//ʹ��״̬�ص�����
void (*state_func)(int state);

//ʹ�ܰ�ť
lv_obj_t * btn2;
lv_obj_t * label2;
//����ͼ
lv_obj_t * chart;
lv_chart_series_t * ser;
//pwm����ͼ����
lv_coord_t pwm_datas[100];

//������ת��֮������
char encoder_str[20];
//Ŀ��ֵ����
char target_str[20];
//pwm����
char pwm_str[20];
//��ѹ����
char voltage_str[20];

//����ʽPID��λ��ʽPID������ؼ�
lv_obj_t * pos_ta_kp;
lv_obj_t * pos_ta_ki;
lv_obj_t * pos_ta_kd;
lv_obj_t * inc_ta_kp;
lv_obj_t * inc_ta_ki;
lv_obj_t * inc_ta_kd;

//���������ʽPID��λ��ʽPID��ֵ
float pos_p;
float pos_i;
float pos_d;
float inc_p;
float inc_i;
float inc_d;

//����
static lv_obj_t * kb;
//����ֵ
int encoder;
//Ŀ��ֵ
float target;
//pwm
int pwm;
//��ѹ
float voltage;

//������label
lv_obj_t * label_encode_data;
//Ŀ��ֵlabel
lv_obj_t * label_target_data;
//pwmֵlabel
lv_obj_t * label_pwm_data;
//��ѹֵlabel
lv_obj_t * label_voltage_data;
//���»ص�
void set_update_callback(void (*func)(float pos_p,float pos_i,float pos_d,float inc_p,float inc_i,float inc_d)){
	update_func = func;
}
//����ʹ��״̬�ص�
void set_enable_state_callback(void (*func)(int state)){
	state_func = func;
}	

/**
 * @brief �ı��¼�
 * @param e �¼�����
 */
static void ta_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);            // ��ȡ�¼�����
    lv_obj_t * ta = lv_event_get_target(e);                 // ��ȡ�ı�����
    lv_obj_t * kb = (lv_obj_t *)lv_event_get_user_data(e);  // ��ȡ���̶���
    /* ��ȡ�ı�����¼� */
    if(code == LV_EVENT_CLICKED) {
        lv_keyboard_set_textarea(kb, ta);                   // ���������ı����й���
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);          // ������̵������־����ʾ����
        lv_obj_add_state(ta, LV_STATE_FOCUSED);             // ���ı�������Ϊ�۽�״̬
    }
    /* ��ȡ�ı��۽��¼� */
//    if(code == LV_EVENT_FOCUSED) {
//        lv_keyboard_set_textarea(kb, ta);                   // ���������ı����й���
//        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);          // ������̵������־����ʾ����
 
//    }
    /* ��ȡ�ı�ʧȥ�����¼� */
    if(code == LV_EVENT_DEFOCUSED) {
        lv_keyboard_set_textarea(kb, NULL);                 // ȡ�����̵Ĺ���
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);            // ��Ӽ������ر�־�����ؼ���
    }
}
 
/**
 * @brief ����ȷ���¼�
 * @param e �¼�����
 */
static void kb_event_cb(lv_event_t * e)
{
    lv_obj_t * kb = lv_event_get_target(e);             // ��ȡ��ǰ�¼�����Ҳ���Ǽ��̶���
 
    lv_obj_t * ta = lv_keyboard_get_textarea(kb);      // ��ȡ����̰󶨵����뻺�����Ķ���
 
    lv_keyboard_set_textarea(kb, NULL);                 // ȡ�����̵Ĺ���
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);            // ��Ӽ������ر�־�����ؼ���
 
    lv_obj_clear_state(ta, LV_STATE_FOCUSED);           // ����ı���ľ۽�״̬
}
//��ȡ�����pid����
static void get_pid_data(void){
	pos_p = atof(lv_textarea_get_text(pos_ta_kp));
	pos_i = atof(lv_textarea_get_text(pos_ta_ki));
	pos_d = atof(lv_textarea_get_text(pos_ta_kd));
	inc_p = atof(lv_textarea_get_text(inc_ta_kp));
	inc_i = atof(lv_textarea_get_text(inc_ta_ki));
	inc_d = atof(lv_textarea_get_text(inc_ta_kd));
	
}
//��ť����¼�����
static void event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
				//��ȡ����ֵ
				get_pid_data();
				//ִ�лص�
				if(update_func!=NULL){
					update_func(pos_p,pos_i,pos_d,inc_p,inc_i,inc_d);
				}
				
				//ִ�в���
				printf("%f %f %f %f %f %f\n",pos_p,pos_i,pos_d,inc_p,inc_i,inc_d);
			return;
    }
		if(code == LV_EVENT_VALUE_CHANGED) {
				lv_state_t state = lv_obj_get_state(btn2);
				//ѡ��
				if(state==3){
					if(state_func!=NULL){
						state_func(1);
					}
					lv_label_set_text(label2, "Enable");
					printf("checked\n");
				}else if(state==2){//δѡ��
					printf("not checked\n");
					if(state_func!=NULL){
						state_func(0);
					}
					lv_label_set_text(label2, "Disable");
				}
    }
}
//��ȡʹ�ܰ�ť״̬
uint16_t get_enable_state(){
	lv_state_t state = lv_obj_get_state(btn2);
	return state;
}

//���±���������
void update_encoder(int data){
	encoder = data;
	sprintf(encoder_str,"%d",encoder);
	lv_label_set_text(label_encode_data, encoder_str);
}
//����Ŀ������
void update_target(float data){
	target = data;
	sprintf(target_str,"%.3f",target);
	lv_label_set_text(label_target_data, target_str);
}
//����pwmֵ
void update_pwm(int data){
	pwm= data;
	sprintf(pwm_str,"%d",pwm);
	lv_label_set_text(label_pwm_data, pwm_str);
}
//���µ�ѹ����
void update_voltage(float data){
	voltage = data;
	sprintf(voltage_str,"%.3f",voltage);
	lv_label_set_text(label_voltage_data, voltage_str);
}
//���²���ͼ
void update_oscillogram(){
		uint32_t pcnt = sizeof(pwm_datas) / sizeof(pwm_datas[0]);
		memcpy(pwm_datas,ecg_sample,pcnt*sizeof(pwm_datas[0]));
    lv_chart_set_point_count(chart, pcnt);
    lv_chart_set_ext_y_array(chart, ser, (lv_coord_t *)pwm_datas);
}
//����pid��ֵ
void update_pid_value(float pos_p,float pos_i,float pos_d,float inc_p,float inc_i,float inc_d){
	char p_p[20];
	char p_i[20];
	char p_d[20];
	char i_p[20];
	char i_i[20];
	char i_d[20];
	sprintf(p_p,"%.3f",pos_p);
	sprintf(p_i,"%.3f",pos_i);
	sprintf(p_d,"%.3f",pos_d);
	sprintf(i_p,"%.3f",inc_p);
	sprintf(i_i,"%.3f",inc_i);
	sprintf(i_d,"%.3f",inc_d);
	lv_textarea_add_text(pos_ta_kp,p_p);
	lv_textarea_add_text(pos_ta_ki,p_i);
	lv_textarea_add_text(pos_ta_kd,p_d);
	lv_textarea_add_text(inc_ta_kp,i_p);
	lv_textarea_add_text(inc_ta_ki,i_i);
	lv_textarea_add_text(inc_ta_kd,i_d);
}

void show_keyboard(void){
	/*Create a keyboard to use it with an of the text areas*/
	lv_obj_t *kb = lv_keyboard_create(lv_scr_act());
	
	/*Create a text area. The keyboard will write here*/
	lv_obj_t * ta;
	ta = lv_textarea_create(lv_scr_act());
	lv_obj_align(ta, LV_ALIGN_TOP_LEFT, 10, 10);
	lv_obj_add_event_cb(ta, ta_event_cb, LV_EVENT_ALL, kb);
	lv_textarea_set_placeholder_text(ta, "Hello");
	lv_obj_set_size(ta, 140, 80);

	ta = lv_textarea_create(lv_scr_act());
	lv_obj_align(ta, LV_ALIGN_TOP_RIGHT, -10, 10);
	lv_obj_add_event_cb(ta, ta_event_cb, LV_EVENT_ALL, kb);
	lv_obj_set_size(ta, 140, 80);
	lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_NUMBER);

	lv_keyboard_set_textarea(kb, ta);
}


void pid_chart(lv_obj_t * obj){
	
		/*Create a chart*/
    chart = lv_chart_create(obj);
    lv_obj_set_size(chart, 520, 150);
    lv_obj_align(chart, LV_ALIGN_CENTER, -30, -30);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, -1000, 1000);

    /*Do not display points on the data*/
    lv_obj_set_style_size(chart, 0, LV_PART_INDICATOR);

    ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
		//���²���ͼ
    update_oscillogram();
		//����ͼ��
		lv_obj_add_flag(chart, LV_OBJ_FLAG_HIDDEN); 
}


void position_pid(lv_obj_t * obj){
	//����
	kb = lv_keyboard_create(lv_scr_act());
	lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_NUMBER);
	
	lv_obj_t * pid_lay = lv_obj_create(obj);
	lv_obj_set_size(pid_lay, 520, 100);
	lv_obj_align(pid_lay, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_set_flex_flow(pid_lay, LV_FLEX_FLOW_ROW);
	
	//kp
	lv_obj_t * label_kp = lv_label_create(pid_lay);
	lv_label_set_long_mode(label_kp, LV_LABEL_LONG_WRAP);     
	lv_label_set_text(label_kp, "kp:");
	lv_obj_set_style_text_align(label_kp, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label_kp, LV_ALIGN_CENTER, 0, -40);
	//kp����
	pos_ta_kp = lv_textarea_create(pid_lay);
	lv_obj_set_size(pos_ta_kp, 100, 100);
	lv_textarea_set_one_line(pos_ta_kp, true);

	//ki
	lv_obj_t * label_ki = lv_label_create(pid_lay);
	lv_label_set_long_mode(label_ki, LV_LABEL_LONG_WRAP);     
	lv_label_set_text(label_ki, "ki:");
	lv_obj_set_style_text_align(label_ki, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label_ki, LV_ALIGN_CENTER, 0, -40);
	//ki����
	/* ����һ���ı��������ڼ�����������Ϣ */
	pos_ta_ki = lv_textarea_create(pid_lay);
	lv_obj_set_size(pos_ta_ki, 100, 100);
	lv_textarea_set_one_line(pos_ta_ki, true);
	
	//kd
	lv_obj_t * label_kd = lv_label_create(pid_lay);
	lv_label_set_long_mode(label_kd, LV_LABEL_LONG_WRAP);     
	lv_label_set_text(label_kd, "kd:");
	lv_obj_set_style_text_align(label_kd, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label_kd, LV_ALIGN_CENTER, 0, -40);
	//kd����
	/* ����һ���ı��������ڼ�����������Ϣ */
	pos_ta_kd = lv_textarea_create(pid_lay);
	lv_obj_set_size(pos_ta_kd, 100, 100);
	lv_textarea_set_one_line(pos_ta_kd, true);
	
	
	/* �ı��ؼ��������¼�*/
	lv_obj_add_event_cb(pos_ta_kp, ta_event_cb, LV_EVENT_ALL, kb);
	lv_obj_add_event_cb(pos_ta_ki, ta_event_cb, LV_EVENT_ALL, kb);
	lv_obj_add_event_cb(pos_ta_kd, ta_event_cb, LV_EVENT_ALL, kb);
	
	/* ��ʼʱ���������� */
	lv_keyboard_set_textarea(kb, NULL);                     // δ����������������
	lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);                // ����ʾ����������

	/* ���̵�ȷ���¼� */
	lv_obj_add_event_cb(kb, kb_event_cb, LV_EVENT_READY , NULL);
}

void incremental_pid(lv_obj_t * obj){
	//����
	kb = lv_keyboard_create(lv_scr_act());
	lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_NUMBER);
	
	lv_obj_t * pid_lay = lv_obj_create(obj);
	lv_obj_set_size(pid_lay, 520, 100);
	lv_obj_align(pid_lay, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_set_flex_flow(pid_lay, LV_FLEX_FLOW_ROW);
	
	//kp
	lv_obj_t * label_kp = lv_label_create(pid_lay);
	lv_label_set_long_mode(label_kp, LV_LABEL_LONG_WRAP);     
	lv_label_set_text(label_kp, "kp:");
	lv_obj_set_style_text_align(label_kp, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label_kp, LV_ALIGN_CENTER, 0, -40);
	//kp����
	inc_ta_kp = lv_textarea_create(pid_lay);
	lv_obj_set_size(inc_ta_kp, 100, 100);
//	lv_textarea_set_placeholder_text(inc_ta_kp,);
	lv_textarea_set_one_line(inc_ta_kp, true);

	//ki
	lv_obj_t * label_ki = lv_label_create(pid_lay);
	lv_label_set_long_mode(label_ki, LV_LABEL_LONG_WRAP);     
	lv_label_set_text(label_ki, "ki:");
	lv_obj_set_style_text_align(label_ki, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label_ki, LV_ALIGN_CENTER, 0, -40);
	//ki����
	/* ����һ���ı��������ڼ�����������Ϣ */
	inc_ta_ki = lv_textarea_create(pid_lay);
	lv_obj_set_size(inc_ta_ki, 100, 100);
	lv_textarea_set_one_line(inc_ta_ki, true);
	
	//kd
	lv_obj_t * label_kd = lv_label_create(pid_lay);
	lv_label_set_long_mode(label_kd, LV_LABEL_LONG_WRAP);     
	lv_label_set_text(label_kd, "kd:");
	lv_obj_set_style_text_align(label_kd, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label_kd, LV_ALIGN_CENTER, 0, -40);
	//kd����
	/* ����һ���ı��������ڼ�����������Ϣ */
	inc_ta_kd = lv_textarea_create(pid_lay);
	lv_obj_set_size(inc_ta_kd, 100, 100);
	lv_textarea_set_one_line(inc_ta_kd, true);
	
	
	/* �ı��ؼ��������¼�*/
	lv_obj_add_event_cb(inc_ta_kp, ta_event_cb, LV_EVENT_ALL, kb);
	lv_obj_add_event_cb(inc_ta_ki, ta_event_cb, LV_EVENT_ALL, kb);
	lv_obj_add_event_cb(inc_ta_kd, ta_event_cb, LV_EVENT_ALL, kb);

	/* ��ʼʱ���������� */
	lv_keyboard_set_textarea(kb, NULL);                     // δ����������������
	lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);                // ����ʾ����������

	/* ���̵�ȷ���¼� */
	lv_obj_add_event_cb(kb, kb_event_cb, LV_EVENT_READY , NULL);
}

void left_layout(lv_obj_t * obj){
	//�м����pid
	lv_obj_t * label1 = lv_label_create(obj);
	lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP);     /*Break the long lines*/
	lv_label_set_text(label1, "Position  PID:");
	lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label1, LV_ALIGN_CENTER, 0, -40);
	//λ��ʽPID����
	position_pid(obj);
	
	//�²�����pid
	lv_obj_t * label2 = lv_label_create(obj);
	lv_label_set_long_mode(label2, LV_LABEL_LONG_WRAP);     /*Break the long lines*/
	lv_label_set_text(label2, "Incremental  PID:");
	lv_obj_set_style_text_align(label2, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label2, LV_ALIGN_CENTER, 0, -40);
	//����ʽPID����
	incremental_pid(obj);
	
	//�Ҳ��ϲ����
	pid_chart(obj);
}

void right_layout(lv_obj_t * obj){
	//����������
	lv_obj_t * encode_lay = lv_obj_create(obj);
	lv_obj_set_size(encode_lay, 210, 70);
	lv_obj_align(encode_lay, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_set_flex_flow(encode_lay, LV_FLEX_FLOW_ROW);
	
	//������
	lv_obj_t * label_encode = lv_label_create(encode_lay);
	lv_label_set_long_mode(label_encode, LV_LABEL_LONG_WRAP);     
	lv_label_set_text(label_encode, "encoder:");
	lv_obj_set_style_text_align(label_encode, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label_encode, LV_ALIGN_CENTER, 0, -40);
	//����������
	label_encode_data = lv_label_create(encode_lay);
	lv_label_set_long_mode(label_encode_data, LV_LABEL_LONG_WRAP);  
	sprintf(encoder_str,"%d",encoder);
	lv_label_set_text(label_encode_data, encoder_str);
	lv_obj_set_style_text_align(label_encode_data, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label_encode_data, LV_ALIGN_CENTER, 0, -40);
	
	
	//Ŀ��ֵ����
	lv_obj_t * target_lay = lv_obj_create(obj);
	lv_obj_set_size(target_lay, 210, 70);
	lv_obj_align(target_lay, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_set_flex_flow(target_lay, LV_FLEX_FLOW_ROW);
	//Ŀ��ֵ
	lv_obj_t * label_target = lv_label_create(target_lay);
	lv_label_set_long_mode(label_target, LV_LABEL_LONG_WRAP);     
	lv_label_set_text(label_target, "target:");
	lv_obj_set_style_text_align(label_target, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label_target, LV_ALIGN_CENTER, 0, -40);
	//Ŀ��ֵ����
	label_target_data = lv_label_create(target_lay);
	lv_label_set_long_mode(label_target_data, LV_LABEL_LONG_WRAP);  
	sprintf(target_str,"%f",target);	
	lv_label_set_text(label_target_data, target_str);
	lv_obj_set_style_text_align(label_target_data, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label_target_data, LV_ALIGN_CENTER, 0, -40);
	
	
	//pwm����
	lv_obj_t * pwm_lay = lv_obj_create(obj);
	lv_obj_set_size(pwm_lay, 210, 70);
	lv_obj_align(pwm_lay, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_set_flex_flow(pwm_lay, LV_FLEX_FLOW_ROW);
	//pwm
	lv_obj_t * label_pwm = lv_label_create(pwm_lay);
	lv_label_set_long_mode(label_pwm, LV_LABEL_LONG_WRAP);     
	lv_label_set_text(label_pwm, "pwm:");
	lv_obj_set_style_text_align(label_pwm, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label_pwm, LV_ALIGN_CENTER, 0, -40);
	//pwm����
	label_pwm_data = lv_label_create(pwm_lay);
	lv_label_set_long_mode(label_pwm_data, LV_LABEL_LONG_WRAP); 
	sprintf(pwm_str,"%d",pwm);	
	lv_label_set_text(label_pwm_data, pwm_str);
	lv_obj_set_style_text_align(label_pwm_data, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label_pwm_data, LV_ALIGN_CENTER, 0, -40);
	
	//��ѹ����
	lv_obj_t * voltage_lay = lv_obj_create(obj);
	lv_obj_set_size(voltage_lay, 210, 70);
	lv_obj_align(voltage_lay, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_set_flex_flow(voltage_lay, LV_FLEX_FLOW_ROW);
	//��ѹ
	lv_obj_t * label_voltage = lv_label_create(voltage_lay);
	lv_label_set_long_mode(label_voltage, LV_LABEL_LONG_WRAP);     
	lv_label_set_text(label_voltage, "voltage:");
	lv_obj_set_style_text_align(label_voltage, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label_voltage, LV_ALIGN_CENTER, 0, -40);
	//��ѹ����
	label_voltage_data = lv_label_create(voltage_lay);
	lv_label_set_long_mode(label_voltage_data, LV_LABEL_LONG_WRAP);  
	sprintf(voltage_str,"%f",voltage);		
	lv_label_set_text(label_voltage_data, voltage_str);
	lv_obj_set_style_text_align(label_voltage_data, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label_voltage_data, LV_ALIGN_CENTER, 0, -40);
	
	//���º�ʹ�ܲ���
	lv_obj_t * update_enable_lay = lv_obj_create(obj);
	lv_obj_set_size(update_enable_lay, 210, 90);
	lv_obj_align(update_enable_lay, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_set_flex_flow(update_enable_lay, LV_FLEX_FLOW_ROW);
	
	//���°�ť
	lv_obj_t * btn1 = lv_btn_create(update_enable_lay);
	lv_obj_set_size(btn1, 75, 40);
	lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_CLICKED, NULL);
	lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -40);
	
	lv_obj_t * label = lv_label_create(btn1);
	lv_label_set_text(label, "Update");
	lv_obj_center(label);
	//ʹ�ܰ�ť
	btn2 = lv_btn_create(update_enable_lay);
	lv_obj_set_size(btn2, 75, 40);
	lv_obj_add_event_cb(btn2, event_handler, LV_EVENT_VALUE_CHANGED , NULL);
	lv_obj_add_flag(btn2, LV_OBJ_FLAG_CHECKABLE);
	lv_obj_align(btn2, LV_ALIGN_CENTER, 0, -40);
	
	label2 = lv_label_create(btn2);
	lv_label_set_text(label2, "Disable");
	lv_obj_center(label2);
}

void show_pid_ui(void){
	/*��಼��*/
	lv_obj_t * left_lay = lv_obj_create(lv_scr_act());
	lv_obj_set_size(left_lay, 550, 470);
	lv_obj_align(left_lay, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_set_flex_flow(left_lay, LV_FLEX_FLOW_COLUMN);
	
	/*�Ҳ಼��*/
	lv_obj_t * right_lay = lv_obj_create(lv_scr_act());
	lv_obj_set_size(right_lay, 250, 470);
	lv_obj_align_to(right_lay, left_lay, LV_ALIGN_OUT_RIGHT_TOP, 0, 0);
	lv_obj_set_flex_flow(right_lay, LV_FLEX_FLOW_COLUMN);
	
	//���߲���
	left_layout(left_lay);
	right_layout(right_lay);
	
}