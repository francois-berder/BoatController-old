/* 
 * File:   status.h
 * Author: francois
 *
 * Created on 30 mai 2016, 13:21
 */

#ifndef STATUS_H
#define	STATUS_H

#ifdef	__cplusplus
extern "C" {
#endif

enum
{
    STATUS_OFF,
    STATUS_ON,
    STATUS_SLOW_BLINK,
    STATUS_FAST_BLINK
};

void STATUS_set_mode(char new_mode);
void STATUS_update(void);


#ifdef	__cplusplus
}
#endif

#endif	/* STATUS_H */

