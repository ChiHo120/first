#include "headfile.h"



#define BUFFER_SIZE 10    // �˲���������С
#define GRAPH_WIDTH 240   // ����ͼ���
#define GRAPH_HEIGHT 100  // ����ͼ�߶�
#define GRAPH_OFFSET 50   // ����ͼY��ƫ��

// ������ʽ����
typedef enum {
    LINE_SOLID = 0,
    LINE_DOTTED
} LineStyle;

// ���̶���
typedef enum {
    RANGE_3V3 = 0,
    RANGE_5V,
    RANGE_10V,
    RANGE_COUNT
} VoltageRange;

// ȫ�ֱ���
float voltage_buffer[GRAPH_WIDTH];  // �洢��ʷ��ѹֵ���ڻ�ͼ
uint8_t buffer_index ;           // ��ǰ����������
float filtered_voltage ;      // �˲���ĵ�ѹֵ
VoltageRange current_range = RANGE_3V3;  // ��ǰ����

// ����ƽ���˲�
float sliding_average_filter(float new_value) {
    static float buffer[BUFFER_SIZE] = {0};
    static uint8_t index = 0;
    static float sum = 0;
    
    sum -= buffer[index];       // ��ȥ��ɵ�ֵ
    buffer[index] = new_value;  // �洢��ֵ
    sum += new_value;           // ������ֵ
    
    index = (index + 1) % BUFFER_SIZE;
    
    return sum / BUFFER_SIZE;
}

// ��ȡ��ѹֵ(�����̴���)
float get_voltage() {
    float raw_voltage = get_vol1();  // ��ȡԭʼADC��ѹ
    
    // �������̵���
    switch(current_range) {
        case RANGE_3V3:
            return raw_voltage;
        case RANGE_5V:
            return raw_voltage * (5.0f / 3.3f);
        case RANGE_10V:
            return raw_voltage * (10.0f / 3.3f);
        default:
            return raw_voltage;
    }
}

// �л�����
void switch_range() {
    current_range = (VoltageRange)((uint8_t)current_range + 1) % (uint8_t)RANGE_COUNT;
}

// ����������
void draw_axes() {
    // ����X��
    LCD_DrawLine(0, GRAPH_OFFSET + GRAPH_HEIGHT, GRAPH_WIDTH, Horizontal);
    // ����Y��
    LCD_DrawLine(0, GRAPH_OFFSET, GRAPH_HEIGHT, Vertical);
    
    // ���ƿ̶ȱ��
    for(int i=0; i<=GRAPH_WIDTH; i+=30) {
        LCD_DrawLine(i, GRAPH_OFFSET + GRAPH_HEIGHT, 5, Vertical);
    }
    
    for(int i=0; i<=GRAPH_HEIGHT; i+=20) {
        LCD_DrawLine(0, GRAPH_OFFSET + i, 5, Horizontal);
    }
}

// ���Ƶ�ѹ����
void draw_voltage_graph() {
    static uint16_t prev_x = 0, prev_y = 0;
    
    // ���㵱ǰ��ѹ��Ӧ��Y����
    float max_voltage;
    switch(current_range) {
        case RANGE_3V3: max_voltage = 3.3f; break;
        case RANGE_5V: max_voltage = 5.0f; break;
        case RANGE_10V: max_voltage = 10.0f; break;
    }
    
    uint16_t current_y = GRAPH_OFFSET + GRAPH_HEIGHT - (uint16_t)((filtered_voltage / max_voltage) * GRAPH_HEIGHT);
    
    // ��������
    if(buffer_index > 0) {
        // �򻯻��ƣ�ֻ����
        LCD_SetCursor(prev_x, prev_y);
        LCD_WriteRAM_Prepare();
        LCD_WriteRAM(White);
        
        LCD_SetCursor(buffer_index, current_y);
        LCD_WriteRAM_Prepare();
        LCD_WriteRAM(White);
    }
    
    prev_x = buffer_index;
    prev_y = current_y;
    
    // ���»���������
    buffer_index = (buffer_index + 1) % GRAPH_WIDTH;
    
    // ����������������������ػ�������
    if(buffer_index == 0) {
        LCD_Clear(Black);
        draw_axes();
    }
}

// ��ʾ��ѹֵ��������Ϣ
void display_voltage_info() {
    char info_str[30];
    
    // ��ʾ��ǰ��ѹֵ
    snprintf(info_str, sizeof(info_str), "Voltage:");
    LCD_DisplayStringLine(Line2, (uint8_t *)info_str);
	    snprintf(info_str, sizeof(info_str), "%.2f V", filtered_voltage);
    LCD_DisplayStringLine(Line3, (uint8_t *)info_str);
    
    // ��ʾ��ǰ����
    const char *range_str;
    switch(current_range) {
        case RANGE_3V3: range_str = "0-3.3V"; break;
        case RANGE_5V: range_str = "0-5V   "; break;
        case RANGE_10V: range_str = "0-10V "; break;
    }
		char  text[20];
		sprintf(text,"Range:");
		LCD_DisplayStringLine(Line6,(uint8_t*)text);
    LCD_DisplayStringLine(Line7, (uint8_t *)range_str);
}


