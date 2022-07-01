#ifndef SENSOR_H
#define	SENSOR_H

/**
 * sensor_init
 *
 *  Initializes the sensor subsystem.
 *
 *  Parameters:
 *    none.
 *
 *  Returns:
 *    zero if initialized successfully, non-zero otherwise.
 */
extern uint8_t sensor_init(void);

/**
 * sensor_read
 *
 *  Reads the temperature.
 *
 *  Parameters:
 *    none.
 *
 *  Returns:
 *    temperature value in 0.0625°C increments
 */
extern int16_t sensor_read(void);

#endif	/* SENSOR_H */

