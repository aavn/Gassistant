#ifndef _GAS_SCALE_SERVICE_H
#define _GAS_SCALE_SERVICE_H

const char *  readSettingInfo(void);
void  sendGasinfo(void);
void  notifyOutOfGas(void);
void  notifyLeakingRisk(void);
void  notifyScaleSettingsUpdated(void);
void  sendGasInfoToRestService(void);
void  saveSettingInfoToMem(void);
int   Main(void);

#endif

