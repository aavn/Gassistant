#ifndef _INIT_UTIL_H
#define _INIT_UTIL_H

void computeWifiScale(void );
void loadDataFromMem(void);
void computeEthernetScale(byte * macAddr);
/*
 * The data will be in the sequence:
 * - Arduino Id
 * - Tank weight
 * - Gross weight
 * - Risk duration
 * - Percentage to alert
 * - Amount to alert
 */
void useDefaultSetting(void);
void printDateTime();
void printDigits(int digits);
#endif

