#ifndef __BTTEST_CONSTANTS_H__
#define __BTTEST_CONSTANTS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
/* Librairies Bluetooth */
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>
/* En têtes personels */
#include "constants.h"

typedef char btdname[248];
typedef char btdaddr[18];

#ifdef __cplusplus
}
#endif

#endif /*__BTTEST_CONSTANTS_H__ */
