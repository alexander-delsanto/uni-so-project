#ifndef OS_PROJECT_CONST_H
#define OS_PROJECT_CONST_H

#define SHM_DATA_GENERAL_KEY 0x1fffffff
#define SHM_DATA_PORTS_KEY 0x2fffffff
#define SHM_DATA_SHIPS_KEY 0x3fffffff
#define SHM_DATA_PORT_OFFER_KEY 0x4fffffff
#define SHM_DATA_DEMAND_KEY 0x5fffffff

#define SEM_PORTS_INITIALIZED_KEY 0x00ffffff
#define SEM_START_KEY 0x10ffffff
#define SEM_PORT_KEY 0x11ffffff

#define MSG_IN_PORT_KEY 0x100fffff
#define MSG_OUT_PORT_KEY 0x110fffff

#define SIGDAY SIGUSR1
#define SIGSWELL SIGUSR2
#define SIGSTORM SIGUSR2
#define SIGMAELSTROM SIGTERM

#define NUM_CONST 16

#endif
