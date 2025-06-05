#include "headfile.h"



#define BUFFER_SIZE 10    // 滤波缓冲区大小
#define GRAPH_WIDTH 240   // 曲线图宽度
#define GRAPH_HEIGHT 100  // 曲线图高度
#define GRAPH_OFFSET 50   // 曲线图Y轴偏移

// 线条样式定义
typedef enum {
    LINE_SOLID = 0,
    LINE_DOTTED
} LineStyle;

// 量程定义
typedef enum {
    RANGE_3V3 = 0,
    RANGE_5V,
    RANGE_10V,
    RANGE_COUNT
} VoltageRange;

// 全局变量
float voltage_buffer[GRAPH_WIDTH];  // 存储历史电压值用于绘图
uint8_t buffer_index ;           // 当前缓冲区索引
float filtered_voltage ;      // 滤波后的电压值
VoltageRange current_range = RANGE_3V3;  // 当前量程

// 滑动平均滤波
float sliding_average_filter(float new_value) {
    static float buffer[BUFFER_SIZE] = {0};
    static uint8_t index = 0;
    static float sum = 0;
    
    sum -= buffer[index];       // 减去最旧的值
    buffer[index] = new_value;  // 存储新值
    sum += new_value;           // 加上新值
    
    index = (index + 1) % BUFFER_SIZE;
    
    return sum / BUFFER_SIZE;
}

// 获取电压值(带量程处理)
float get_voltage() {
    float raw_voltage = get_vol1();  // 获取原始ADC电压
    
    // 根据量程调整
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

// 切换量程
void switch_range() {
    current_range = (VoltageRange)((uint8_t)current_range + 1) % (uint8_t)RANGE_COUNT;
}

// 绘制坐标轴
void draw_axes() {
    // 绘制X轴
    LCD_DrawLine(0, GRAPH_OFFSET + GRAPH_HEIGHT, GRAPH_WIDTH, Horizontal);
    // 绘制Y轴
    LCD_DrawLine(0, GRAPH_OFFSET, GRAPH_HEIGHT, Vertical);
    
    // 绘制刻度标记
    for(int i=0; i<=GRAPH_WIDTH; i+=30) {
        LCD_DrawLine(i, GRAPH_OFFSET + GRAPH_HEIGHT, 5, Vertical);
    }
    
    for(int i=0; i<=GRAPH_HEIGHT; i+=20) {
        LCD_DrawLine(0, GRAPH_OFFSET + i, 5, Horizontal);
    }
}

// 绘制电压曲线
void draw_voltage_graph() {
    static uint16_t prev_x = 0, prev_y = 0;
    
    // 计算当前电压对应的Y坐标
    float max_voltage;
    switch(current_range) {
        case RANGE_3V3: max_voltage = 3.3f; break;
        case RANGE_5V: max_voltage = 5.0f; break;
        case RANGE_10V: max_voltage = 10.0f; break;
    }
    
    uint16_t current_y = GRAPH_OFFSET + GRAPH_HEIGHT - (uint16_t)((filtered_voltage / max_voltage) * GRAPH_HEIGHT);
    
    // 绘制曲线
    if(buffer_index > 0) {
        // 简化绘制，只画点
        LCD_SetCursor(prev_x, prev_y);
        LCD_WriteRAM_Prepare();
        LCD_WriteRAM(White);
        
        LCD_SetCursor(buffer_index, current_y);
        LCD_WriteRAM_Prepare();
        LCD_WriteRAM(White);
    }
    
    prev_x = buffer_index;
    prev_y = current_y;
    
    // 更新缓冲区索引
    buffer_index = (buffer_index + 1) % GRAPH_WIDTH;
    
    // 如果缓冲区已满，清屏并重绘坐标轴
    if(buffer_index == 0) {
        LCD_Clear(Black);
        draw_axes();
    }
}

// 显示电压值和量程信息
void display_voltage_info() {
    char info_str[30];
    
    // 显示当前电压值
    snprintf(info_str, sizeof(info_str), "Voltage:");
    LCD_DisplayStringLine(Line2, (uint8_t *)info_str);
	    snprintf(info_str, sizeof(info_str), "%.2f V", filtered_voltage);
    LCD_DisplayStringLine(Line3, (uint8_t *)info_str);
    
    // 显示当前量程
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


