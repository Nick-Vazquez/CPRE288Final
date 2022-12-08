/*
 * song.h
 *
 *  Created on: Dec 2, 2022
 *      Author: rrlowe02
 */

#ifndef SONG_H_
#define SONG_H_

#include "open_interface.h"
#include "Components/lcd.h"
#include <inc/tm4c123gh6pm.h>

void make_song(int song_number, int num_notes, unsigned char *notes, unsigned char *duration, oi_t* oi);


#endif /* SONG_H_ */
