#ifndef _TRAN_H
#define _TRAN_H

void draw_axes(void);
float get_voltage(void);
float sliding_average_filter(float new_value);
void display_voltage_info(void);
void draw_voltage_graph(void);
void switch_range() ;
extern float filtered_voltage ; 
extern uint8_t buffer_index;
extern float voltage_buffer[240];
#endif
