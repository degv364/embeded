/**
 Copyright 2017 Daniel Garcia Vaglio <degv364@gmail.com> Esteban Zamora Alvarado <estebanzacr.20@gmail.com>

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 **/

#ifndef FILE_NAMES
#define FILE_NAMES

#define MUSIC_FILE_NAME_PREFIX "../media/music/"
#define ICON_FILE_NAME_PREFIX "../icons/"

#define MAX_FILE_NAME_SIZE 80

#define ALL_STAR_TRIO_STR "All-Star_Trio_-_01_-_Oh_By_Jingo.mp3"
#define ART_OF_ESCAPISM_STR "Art_Of_Escapism_-_Star_of_the_Vikings.mp3"
#define TYPICAL_DAY_STR  "Borrtex_-_01_-_Typical_Day.mp3"
#define ALL_BEGINS_HERE_STR "Borrtex_-_02_-_It_All_Begins_Here.mp3"
#define SHES_A_GIFT_STR "Borrtex_-_06_-_Shes_A_Gift.mp3"
#define WELCOME_STR "Borrtex_-_08_-_Welcome.mp3"
#define NIGHT_OWL_STR  "Broke_For_Free_-_01_-_Night_Owl.mp3"
#define WITH_EASE_STR "Dee_Yan-Key_-_02_-_With_Ease.mp3"
#define BEING_FINE_STR "Dee_Yan-Key_-_03_-_Being_Fine.mp3"
#define MOODY_BREAKFAST_STR "Dee_Yan-Key_-_06_-_Moody_Breakfast.mp3"
#define THE_LAST_ONES_STR "Jahzzar_-_01_-_The_last_ones.mp3"
#define GOOD_GRIEF_STR "Ryan_Little_-_Good_Grief.mp3"
#define PIANO_ROLL_STR "Scott_Joplin_-_08_-_Pine_Apple_Rag_Scott_Joplin_piano_roll.mp3"
#define ENTHUSIAST_STR "Tours_-_01_-_Enthusiast.mp3"

typedef enum file_e {
  ALL_STAR_TRIO = 0,
  ART_OF_ESCAPISM ,
  TYPICAL_DAY  ,
  ALL_BEGINS_HERE,
  SHES_A_GIFT, 
  WELCOME, 
  NIGHT_OWL, 
  WITH_EASE ,
  BEING_FINE ,
  MOODY_BREAKFAST, 
  THE_LAST_ONES ,
  GOOD_GRIEF ,
  PIANO_ROLL ,
  ENTHUSIAST,
  LAST_FILE
}file_e;

#endif
