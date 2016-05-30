/* 
 * File:   log.h
 * Author: francois
 *
 * Created on 28 mai 2016, 16:30
 */

#ifndef LOG_H
#define	LOG_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdio.h>
    
enum LOG_LEVEL
{
    DEBUG,
    INFO,
    WARNING,
    ERROR
};
extern char log_level;

#define LOG(LOG_LEVEL, ...) \
    do { \
        if (log_level > LOG_LEVEL) \
            break; \
        if (log_level > DEBUG) \
            printf("[%s]: ", #LOG_LEVEL); \
        else \
            printf("[%s] %s,%d: ", #LOG_LEVEL, __FILE__, __LINE__);  \
        printf(__VA_ARGS__); \
        printf("\n"); \
    }while(0)

#define LOG_DBG(...)    LOG(DEBUG, __VA_ARGS__)
#define LOG_INFO(...)   LOG(INFO, __VA_ARGS__)
#define LOG_WARN(...)   LOG(WARNING, __VA_ARGS__)
#define LOG_ERR(...)    LOG(ERROR, __VA_ARGS__)

#ifdef	__cplusplus
}
#endif

#endif	/* LOG_H */

