/*
 * song.c
 *
 *  Created on: Dec 2, 2022
 *      Author: rrlowe02
 */

#include "song.h"

void make_song(int song_number, int num_notes, unsigned char *notes, unsigned char *duration, oi_t* oi){
    oi_loadSong(song_number, num_notes, notes, duration);

//    int i;
//    for(i = 0; i < num_notes; i++){
//        oi_loadSong(song_number, 1, notes[i], duration);
//        oi_playSong(1);
//    }

}




