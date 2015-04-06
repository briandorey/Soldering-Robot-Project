/* 
 * File:   main.h
 * Author: andrew
 *
 * Created on 05 March 2015, 17:46
 */

#ifndef MAIN_H
#define	MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif

#define FOSC    (8000000ULL)
#define FCY     (FOSC/2)


#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

void getTemperatureDefaults(void);
void updateTemperatures(void);
float calculateTemperature(float rawval);
void RS485Tasks(int uart_port);
void Trace(const uint8_t *writeBuffer);
