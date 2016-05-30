#ifndef _LOADCELL_UTIL_H
#define _LOADCELL_UTIL_H

void initLoadCellsConf(void);
double readGasWeight(void);
void updateGasinfo(double);
void printGasinfo(void);
char * formatTime(unsigned long);

#endif

