#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "sysctl.h"
#include "hw_memmap.h"
#include "gpio.h"
#include "uart.h"
#include "../inc/tm4c123gh6pm.h"
#include "pin_map.h"


void UartSetup(void);		//sets up Uart for my configuration using library

//redirect for printf.
int fputc(int ch, FILE *f);

//redirect input from serial port
//read info edit by: Sir Hutson
int fgetc(FILE *f);


//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNNNNNNNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM8O+++?7IIII7ZNMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMD+++II7IIIIIII7IIODMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMN+?IIIIIIIIIIIIIIIIINMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMMMMMMMMMMMMMMMMMMN7IIIIIIIIIDIIIIIIIIII77NMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMMMMMMMMMMMMDNZI7IIIIIIIIIIIID,.NZ7IIIIIIIZMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMMMMMMMMMD77IIIIIIIIIIIIIIIDN.,DNNIIIIIIIIIDMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMMMMMMMMNOZOIIIIIIIIIIIIIIONNNNNNNOIIIIIIIIZMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMMMMMMMMDZZOZIIIIIIIIIIII7NNNNNNNDOIIIIIIIIINMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMMMMMMMMN8888ZND7IIIIIIIINZNNNNDD,7IIIIIIIIIDMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMMMMMMMMN88888888N$IIIIII7DOZZZZ+N7IIIIIIIIIMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMMMMMMMMMND88888888DZIIIIIII7ODN$77IIIIIIIINMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMMMMMMMMMMDD88888888O88IIIIIIIIIIIIIIIIIIIINMMMMMMMMMMMMMMM8MMO=MMMMMMMM
//MMMMMMMMMMMMMMMMMMMMMN888888D$$8IIIIIIIIIIIIIIIIII7MMMMMMMMMMMMMMMM7+++++MMMMMMM
//MMMMMMMMMMMMMMMMMMMMMMDN8O8O??$$$D8OIIIIIIIIIIIIIIOMMMMMMMMMMMMMMM$=+++=+7MMMMMM
//MMMMMMMMMMMMMNMMMMMMMMM88N?+???7$$8IIIIIIIIIIIIIIIDMMMMMMMMMMMMMMM=++++++=MMMMMM
//MMMMMMMMMMMMDIINMMMMMNN8D+?++??$NIIIIIIIIIIIIIIII7MMMMMMMMMMMMMMMM=+++++++=MMMMM
//MMMMMMMMMM8IIIIII?OMDD88O??O?IN7IIIIIIIIIIIIIIIIIDMMMMMMMMMMMMMN=+=+++777++OMMMM
//MMMMMMMMMMD7IIIIIII+NN887??D8IIIIIIIIIIIIIIIIIIIINM8DZ7IIIIIDMM=+++++?777777ZMMM
//MMMMMMMMM8$7IIIIIII7DZNND7IIIIIIIII77IIIIIII77NZ=+++IIIIIII$MMM++++++7777777MMMM
//MMMMMMMMMMMD7IIIIIIIIINN$III$ONNNO777IIII7ID=++++IIIIIIIIIIIIM$=+++++?77777ZMMMM
//MMMMMMMMMMMMM7IIIIIIIIIII77?N777777IIIIIID7I77IIIIIIIIIIIIIIDMM8=+???7777777MMMM
//MMMMMMMMMMMMMNIIIIIIIIIIII78NNNO77IIIIIIIIIIIIIIIIIIIIIIIIIZNMMM+=777777777ZMMMM
//MMMMMMMMMMMMMMM87IIIIIIIII$~~~~~=NIIIIIIIIIIIIIIIIIIIIIIIZNMMMMM?7$777I777$MMMMM
//MMMMMMMMMMMMMMMMD77IIIII$+~~~~~~~~I7IIIIIIIIIIIIIIIII77DMMMMMMMMMMMMN7ID7DMMMMMM
//MMMMMMMMMMMMMMMMMD$7III7=~~~~~~~~~~I7IIIIIIIIIIIII7788MMMMMMMMMMMMMMM7I$MMMMMMMM
//MMMMMMMMMMMMMMMMMMMN77IN~~~~~~~~~~~~87IIIIIIII777ONMMMMMMMMMMMMMMMMMM7IIMMMMMMMM
//MMMMMMMMMMMMMMMMMMMMMND~~~~~~~~~~~~~~ZIIIIIII8NO7I7MMMMMMMMMMMMMMMMMM7IINMMMMMMM
//MMMMMMMMMMMMMMMMMMMMMM:~~~~~~~~~~~~~~87IIIIIIIIIIIINMMMMMMMMMMMMMMMMN7I$NMMMMMMM
//MMMMMMMMMMMMMMMMMMMMMN~~~~~~~~~~~~~~~+IIIIIIIIIIIIINMMMMMMMMMMMMMMMMNII8NMMMMMMM
//MMMMMMMMMMMMMMMMMMMMM+~~~~~~~~~~~~~~~~Z7IIIIIIIIIIIOMMMMMMMMMMMMMMM87I7NNMMMMMMM
//MMMMMMMMMMMMMMMMMMMMD~~~~~~~~~~~~~~~~~NIIIIIIIIIIIIIDMMMMMMMMMMMMMNIII$~DMMMMMMM
//MMMMMMMMMMMMMMMMMMMMN~~~~~~~~~~~~~~~~~NIIIIIIIIIIIIINMMMMMMMMMMMMD$IIIN~MMMMMMMM
//MMMMMMMMMMMMMMMMMMMMN~~~~~~~~~~~~~~~~~NIIIIIIIIIIIIINMMMMMMMMMMMNIIII8~NMMMMMMMM
//MMMMMMMMMMMMMMMMMMMMN~~~~~~~~~~~~~~~~~Z7IIIIIIIIIIIIDMMMMMMMMMD$7III7?~DMMMMMMMM
//MMMMMMMMMMMMMN=+?IIIN~~~~~~~~~~~~~~~~=IIIIIIIIIIIIIIZMMMMMM8NIIIIIIID~NMMMMMMMMM
//MMMMMMMMMMMN7++IIIII7~~~~~~~~~~~~~~~~Z77IIIIIIIIIIII$NNN87IIIIIIII78~OMMMMMMMMMM
//MMMMMMMMMMM?++IIIIII7D~~~~~~~~~~~~~~~NIIIIIIIIIIIIII$7I7IIIIIIII7Z?+8MMMMMMMMMMM
//MMMMMMMMMMM+?IIIIII777~~~~~~~~~~~~~ZZIIIIIIIIIIIIIII87IIIIIIIII7D++NMMMMMMMMMMMM
//MMMMMMMMMM8IIIII777777D=~~~~~~~~~~D7IIIIIIIIIIIIIII7N777IIIIIID7+?DMMMMMMMMMMMMM
//NNNN777ONNNIII777777777O+~~~~~~~~NIIIIIIIIIIIIIIIII$Z7777778D+++ZDMMMMMMMMMMMMMM
//MD~,.7III7IZII7777777777O+++=~~~N7IIIIIIIIIIIIIIII7N7777ND++++=NMMMMMMMMMMMMMMMM
//MMMNNIIIIIIIII77777777777$D+++++Z777IIIIIIIIIIII777O$?++++++IDMMMMMMMMMMMMMMMMMM
//MMMMMNZ777777I777777777777Z8DI?+$777I7IIIIIIIII777O++++++?OMMMMMMMMMMMMMMMMMMMMM
//MMMMMMM877777777777777DNMMMMMMM8N777777IIIII777777O++?$NMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMN$77777777$NMMMMMMMMMMMM$777777777777777NNDMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMMMN$777777DMMMMMMMMMMMMMMD7777777777777DMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMMMMMN7777NMMMMMMMMMMMMMMMMN7777777777ODMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMMMMMMMMNDMMMMMMMMMMMMMMMMMMO$7777777DZMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMD77777777777DMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM7NDN7DN87NND8MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMNN=O7?~I7?~NMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMOMMM~DNN8MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM