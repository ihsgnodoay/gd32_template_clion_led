#include "pid_ui.h"

//更新按钮点击回调指针
void (*update_func)(float pos_p,float pos_i,float pos_d,float inc_p,float inc_i,float inc_d);
//使能状态回调函数
void (*state_func)(int state);

//使能按钮
lv_obj_t * btn2;
lv_obj_t * label2;
//波形图
lv_obj_t * chart;
lv_chart_series_t * ser;
//pwm波形图数据
lv_coord_t pwm_datas[100];

//编码器转换之后数据
char encoder_str[20];
//目标值数据
char target_str[20];
//pwm数据
char pwm_str[20];
//电压数据
char voltage_str[20];

//增量式PID和位置式PID的输入控件
lv_obj_t * pos_ta_kp;
lv_obj_t * pos_ta_ki;
lv_obj_t * pos_ta_kd;
lv_obj_t * inc_ta_kp;
lv_obj_t * inc_ta_ki;
lv_obj_t * inc_ta_kd;

//输入的增量式PID和位置式PID的值
float pos_p;
float pos_i;
float pos_d;
float inc_p;
float inc_i;
float inc_d;

//键盘
static lv_obj_t * kb;
//编码值
int encoder;
//目标值
float target;
//pwm
int pwm;
//电压
float voltage;

//编码器label
lv_obj_t * label_encode_data;
//目标值label
lv_obj_t * label_target_data;
//pwm值label
lv_obj_t * label_pwm_data;
//电压值label
lv_obj_t * label_voltage_data;
//更新回调
void set_update_callback(void (*func)(float pos_p,float pos_i,float pos_d,float inc_p,float inc_i,float inc_d)){
	update_func = func;
}
//设置使能状态回调
void set_enable_state_callback(void (*func)(int state)){
	state_func = func;
}	

/**
 * @brief 文本事件
 * @param e 事件对象
 */
static void ta_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);            // 获取事件编码
    lv_obj_t * ta = lv_event_get_target(e);                 // 获取文本对象
    lv_obj_t * kb = (lv_obj_t *)lv_event_get_user_data(e);  // 获取键盘对象
    /* 获取文本点击事件 */
    if(code == LV_EVENT_CLICKED) {
        lv_keyboard_set_textarea(kb, ta);                   // 将键盘与文本进行关联
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);          // 清除键盘的隐身标志，显示键盘
        lv_obj_add_state(ta, LV_STATE_FOCUSED);             // 将文本框设置为聚焦状态
    }
    /* 获取文本聚焦事件 */
//    if(code == LV_EVENT_FOCUSED) {
//        lv_keyboard_set_textarea(kb, ta);                   // 将键盘与文本进行关联
//        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);          // 清除键盘的隐身标志，显示键盘
 
//    }
    /* 获取文本失去焦点事件 */
    if(code == LV_EVENT_DEFOCUSED) {
        lv_keyboard_set_textarea(kb, NULL);                 // 取消键盘的关联
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);            // 添加键盘隐藏标志，隐藏键盘
    }
}
 
/**
 * @brief 键盘确认事件
 * @param e 事件对象
 */
static void kb_event_cb(lv_event_t * e)
{
    lv_obj_t * kb = lv_event_get_target(e);             // 获取当前事件对象，也就是键盘对象
 
    lv_obj_t * ta = lv_keyboard_get_textarea(kb);      // 获取与键盘绑定的输入缓冲区的对象
 
    lv_keyboard_set_textarea(kb, NULL);                 // 取消键盘的关联
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);            // 添加键盘隐藏标志，隐藏键盘
 
    lv_obj_clear_state(ta, LV_STATE_FOCUSED);           // 清除文本框的聚焦状态
}
//获取输入的pid数据
static void get_pid_data(void){
	pos_p = atof(lv_textarea_get_text(pos_ta_kp));
	pos_i = atof(lv_textarea_get_text(pos_ta_ki));
	pos_d = atof(lv_textarea_get_text(pos_ta_kd));
	inc_p = atof(lv_textarea_get_text(inc_ta_kp));
	inc_i = atof(lv_textarea_get_text(inc_ta_ki));
	inc_d = atof(lv_textarea_get_text(inc_ta_kd));
	
}
//按钮点击事件处理
static void event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
				//获取输入值
				get_pid_data();
				//执行回调
				if(update_func!=NULL){
					update_func(pos_p,pos_i,pos_d,inc_p,inc_i,inc_d);
				}
				
				//执行操作
				printf("%f %f %f %f %f %f\n",pos_p,pos_i,pos_d,inc_p,inc_i,inc_d);
			return;
    }
		if(code == LV_EVENT_VALUE_CHANGED) {
				lv_state_t state = lv_obj_get_state(btn2);
				//选中
				if(state==3){
					if(state_func!=NULL){
						state_func(1);
					}
					lv_label_set_text(label2, "Enable");
					printf("checked\n");
				}else if(state==2){//未选中
					printf("not checked\n");
					if(state_func!=NULL){
						state_func(0);
					}
					lv_label_set_text(label2, "Disable");
				}
    }
}
//获取使能按钮状态
uint16_t get_enable_state(){
	lv_state_t state = lv_obj_get_state(btn2);
	return state;
}

//更新编码器数据
void update_encoder(int data){
	encoder = data;
	sprintf(encoder_str,"%d",encoder);
	lv_label_set_text(label_encode_data, encoder_str);
}
//更新目标数据
void update_target(float data){
	target = data;
	sprintf(target_str,"%.3f",target);
	lv_label_set_text(label_target_data, target_str);
}
//更新pwm值
void update_pwm(int data){
	pwm= data;
	sprintf(pwm_str,"%d",pwm);
	lv_label_set_text(label_pwm_data, pwm_str);
}
//更新电压数据
void update_voltage(float data){
	voltage = data;
	sprintf(voltage_str,"%.3f",voltage);
	lv_label_set_text(label_voltage_data, voltage_str);
}
//更新波形图
void update_oscillogram(){
		uint32_t pcnt = sizeof(pwm_datas) / sizeof(pwm_datas[0]);
		memcpy(pwm_datas,ecg_sample,pcnt*sizeof(pwm_datas[0]));
    lv_chart_set_point_count(chart, pcnt);
    lv_chart_set_ext_y_array(chart, ser, (lv_coord_t *)pwm_datas);
}
//更新pid的值
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
		//更新波形图
    update_oscillogram();
		//隐藏图表
		lv_obj_add_flag(chart, LV_OBJ_FLAG_HIDDEN); 
}


void position_pid(lv_obj_t * obj){
	//键盘
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
	//kp输入
	pos_ta_kp = lv_textarea_create(pid_lay);
	lv_obj_set_size(pos_ta_kp, 100, 100);
	lv_textarea_set_one_line(pos_ta_kp, true);

	//ki
	lv_obj_t * label_ki = lv_label_create(pid_lay);
	lv_label_set_long_mode(label_ki, LV_LABEL_LONG_WRAP);     
	lv_label_set_text(label_ki, "ki:");
	lv_obj_set_style_text_align(label_ki, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label_ki, LV_ALIGN_CENTER, 0, -40);
	//ki输入
	/* 创建一个文本区域，用于键盘输入后的信息 */
	pos_ta_ki = lv_textarea_create(pid_lay);
	lv_obj_set_size(pos_ta_ki, 100, 100);
	lv_textarea_set_one_line(pos_ta_ki, true);
	
	//kd
	lv_obj_t * label_kd = lv_label_create(pid_lay);
	lv_label_set_long_mode(label_kd, LV_LABEL_LONG_WRAP);     
	lv_label_set_text(label_kd, "kd:");
	lv_obj_set_style_text_align(label_kd, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label_kd, LV_ALIGN_CENTER, 0, -40);
	//kd输入
	/* 创建一个文本区域，用于键盘输入后的信息 */
	pos_ta_kd = lv_textarea_create(pid_lay);
	lv_obj_set_size(pos_ta_kd, 100, 100);
	lv_textarea_set_one_line(pos_ta_kd, true);
	
	
	/* 文本控件的所有事件*/
	lv_obj_add_event_cb(pos_ta_kp, ta_event_cb, LV_EVENT_ALL, kb);
	lv_obj_add_event_cb(pos_ta_ki, ta_event_cb, LV_EVENT_ALL, kb);
	lv_obj_add_event_cb(pos_ta_kd, ta_event_cb, LV_EVENT_ALL, kb);
	
	/* 初始时将键盘隐藏 */
	lv_keyboard_set_textarea(kb, NULL);                     // 未将键盘与输入区绑定
	lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);                // 将表示将键盘隐藏

	/* 键盘的确认事件 */
	lv_obj_add_event_cb(kb, kb_event_cb, LV_EVENT_READY , NULL);
}

void incremental_pid(lv_obj_t * obj){
	//键盘
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
	//kp输入
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
	//ki输入
	/* 创建一个文本区域，用于键盘输入后的信息 */
	inc_ta_ki = lv_textarea_create(pid_lay);
	lv_obj_set_size(inc_ta_ki, 100, 100);
	lv_textarea_set_one_line(inc_ta_ki, true);
	
	//kd
	lv_obj_t * label_kd = lv_label_create(pid_lay);
	lv_label_set_long_mode(label_kd, LV_LABEL_LONG_WRAP);     
	lv_label_set_text(label_kd, "kd:");
	lv_obj_set_style_text_align(label_kd, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label_kd, LV_ALIGN_CENTER, 0, -40);
	//kd输入
	/* 创建一个文本区域，用于键盘输入后的信息 */
	inc_ta_kd = lv_textarea_create(pid_lay);
	lv_obj_set_size(inc_ta_kd, 100, 100);
	lv_textarea_set_one_line(inc_ta_kd, true);
	
	
	/* 文本控件的所有事件*/
	lv_obj_add_event_cb(inc_ta_kp, ta_event_cb, LV_EVENT_ALL, kb);
	lv_obj_add_event_cb(inc_ta_ki, ta_event_cb, LV_EVENT_ALL, kb);
	lv_obj_add_event_cb(inc_ta_kd, ta_event_cb, LV_EVENT_ALL, kb);

	/* 初始时将键盘隐藏 */
	lv_keyboard_set_textarea(kb, NULL);                     // 未将键盘与输入区绑定
	lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);                // 将表示将键盘隐藏

	/* 键盘的确认事件 */
	lv_obj_add_event_cb(kb, kb_event_cb, LV_EVENT_READY , NULL);
}

void left_layout(lv_obj_t * obj){
	//中间调节pid
	lv_obj_t * label1 = lv_label_create(obj);
	lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP);     /*Break the long lines*/
	lv_label_set_text(label1, "Position  PID:");
	lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label1, LV_ALIGN_CENTER, 0, -40);
	//位置式PID参数
	position_pid(obj);
	
	//下部调节pid
	lv_obj_t * label2 = lv_label_create(obj);
	lv_label_set_long_mode(label2, LV_LABEL_LONG_WRAP);     /*Break the long lines*/
	lv_label_set_text(label2, "Incremental  PID:");
	lv_obj_set_style_text_align(label2, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label2, LV_ALIGN_CENTER, 0, -40);
	//增量式PID参数
	incremental_pid(obj);
	
	//右侧上部表格
	pid_chart(obj);
}

void right_layout(lv_obj_t * obj){
	//编码器布局
	lv_obj_t * encode_lay = lv_obj_create(obj);
	lv_obj_set_size(encode_lay, 210, 70);
	lv_obj_align(encode_lay, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_set_flex_flow(encode_lay, LV_FLEX_FLOW_ROW);
	
	//编码器
	lv_obj_t * label_encode = lv_label_create(encode_lay);
	lv_label_set_long_mode(label_encode, LV_LABEL_LONG_WRAP);     
	lv_label_set_text(label_encode, "encoder:");
	lv_obj_set_style_text_align(label_encode, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label_encode, LV_ALIGN_CENTER, 0, -40);
	//编码器数据
	label_encode_data = lv_label_create(encode_lay);
	lv_label_set_long_mode(label_encode_data, LV_LABEL_LONG_WRAP);  
	sprintf(encoder_str,"%d",encoder);
	lv_label_set_text(label_encode_data, encoder_str);
	lv_obj_set_style_text_align(label_encode_data, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label_encode_data, LV_ALIGN_CENTER, 0, -40);
	
	
	//目标值布局
	lv_obj_t * target_lay = lv_obj_create(obj);
	lv_obj_set_size(target_lay, 210, 70);
	lv_obj_align(target_lay, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_set_flex_flow(target_lay, LV_FLEX_FLOW_ROW);
	//目标值
	lv_obj_t * label_target = lv_label_create(target_lay);
	lv_label_set_long_mode(label_target, LV_LABEL_LONG_WRAP);     
	lv_label_set_text(label_target, "target:");
	lv_obj_set_style_text_align(label_target, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label_target, LV_ALIGN_CENTER, 0, -40);
	//目标值数据
	label_target_data = lv_label_create(target_lay);
	lv_label_set_long_mode(label_target_data, LV_LABEL_LONG_WRAP);  
	sprintf(target_str,"%f",target);	
	lv_label_set_text(label_target_data, target_str);
	lv_obj_set_style_text_align(label_target_data, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label_target_data, LV_ALIGN_CENTER, 0, -40);
	
	
	//pwm布局
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
	//pwm数据
	label_pwm_data = lv_label_create(pwm_lay);
	lv_label_set_long_mode(label_pwm_data, LV_LABEL_LONG_WRAP); 
	sprintf(pwm_str,"%d",pwm);	
	lv_label_set_text(label_pwm_data, pwm_str);
	lv_obj_set_style_text_align(label_pwm_data, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label_pwm_data, LV_ALIGN_CENTER, 0, -40);
	
	//电压布局
	lv_obj_t * voltage_lay = lv_obj_create(obj);
	lv_obj_set_size(voltage_lay, 210, 70);
	lv_obj_align(voltage_lay, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_set_flex_flow(voltage_lay, LV_FLEX_FLOW_ROW);
	//电压
	lv_obj_t * label_voltage = lv_label_create(voltage_lay);
	lv_label_set_long_mode(label_voltage, LV_LABEL_LONG_WRAP);     
	lv_label_set_text(label_voltage, "voltage:");
	lv_obj_set_style_text_align(label_voltage, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label_voltage, LV_ALIGN_CENTER, 0, -40);
	//电压数据
	label_voltage_data = lv_label_create(voltage_lay);
	lv_label_set_long_mode(label_voltage_data, LV_LABEL_LONG_WRAP);  
	sprintf(voltage_str,"%f",voltage);		
	lv_label_set_text(label_voltage_data, voltage_str);
	lv_obj_set_style_text_align(label_voltage_data, LV_TEXT_ALIGN_LEFT, 0);
	lv_obj_align(label_voltage_data, LV_ALIGN_CENTER, 0, -40);
	
	//更新和使能布局
	lv_obj_t * update_enable_lay = lv_obj_create(obj);
	lv_obj_set_size(update_enable_lay, 210, 90);
	lv_obj_align(update_enable_lay, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_set_flex_flow(update_enable_lay, LV_FLEX_FLOW_ROW);
	
	//更新按钮
	lv_obj_t * btn1 = lv_btn_create(update_enable_lay);
	lv_obj_set_size(btn1, 75, 40);
	lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_CLICKED, NULL);
	lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -40);
	
	lv_obj_t * label = lv_label_create(btn1);
	lv_label_set_text(label, "Update");
	lv_obj_center(label);
	//使能按钮
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
	/*左侧布局*/
	lv_obj_t * left_lay = lv_obj_create(lv_scr_act());
	lv_obj_set_size(left_lay, 550, 470);
	lv_obj_align(left_lay, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_set_flex_flow(left_lay, LV_FLEX_FLOW_COLUMN);
	
	/*右侧布局*/
	lv_obj_t * right_lay = lv_obj_create(lv_scr_act());
	lv_obj_set_size(right_lay, 250, 470);
	lv_obj_align_to(right_lay, left_lay, LV_ALIGN_OUT_RIGHT_TOP, 0, 0);
	lv_obj_set_flex_flow(right_lay, LV_FLEX_FLOW_COLUMN);
	
	//两边布局
	left_layout(left_lay);
	right_layout(right_lay);
	
}