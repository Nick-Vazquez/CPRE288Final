///*
// * main.c
// *
// *  Created on: Nov 29, 2022
// *      Author: rrlowe02
// */
//
//#include <cliff_sensors.h>
//#include <song.h>
//
//#define NOTES_ROOM "ADAD"
//#define NOTES_END "aZaZ"
//#define NOTES_CLIFF "dede"
//
//int main(){
//    lcd_init();
//    lcd_home();
//    oi_t* oi = oi_alloc();
//    oi_init(oi);
//    unsigned char duration[5] = "aZAa";
//    make_song(1, 6, NOTES_ROOM, duration, oi);
//    make_song(2, 6, NOTES_END, duration, oi);
//    make_song(3, 6, NOTES_CLIFF, duration, oi);
//
////    oi_play_song(1);
//    while(1){
//        set_cliff_values(oi);
//        if(get_cliff(oi)){
//            lcd_printf("Cliff detected");
//            oi_play_song(3);
//            timer_waitMillis(1000);
//        }
//        else{
//            lcd_clear();
//        }
//    }
//    return 0;
//}
//
//
//
