/**
 *****************************************************************************
 * @file:          microphone.h
 * @author         Tian Feng
 * @email          fengtian@topqizhi.com
 * @version        V1.0.0
 * @data
 * @Brief          sequeue header file.
 ******************************************************************************
*/

#ifndef __MICROPHONE_H__
#define __MICROPHONE_H__

#ifdef __cplusplus
extern "C" {
#endif/* __cplusplus */

#include <stdbool.h>

void * microphone_wakeup_poll_thread(void *);
void set_mic_enable(bool flag);
bool get_mic_status();

#ifdef __cplusplus
}
#endif/* __cplusplus */

#endif /* __MICROPHONE_H__ */
