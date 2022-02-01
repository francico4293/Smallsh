/*
* Author: Colin Francis
* ONID: francico
* Description: Global variable for sharing information between signal handlers and main program
*/
#include <signal.h>

volatile sig_atomic_t foregroundOnlyMode = 0;
