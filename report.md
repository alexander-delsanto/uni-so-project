# Transito navale di merci - Project report
## Master
The "master.c" module serves as the main controller for the simulation system, managing ports, ships, and weather, 
through inter-process communication. 
It utilizes shared memory and semaphores for coordination between different processes. 

The simulation involves the movement of ships between ports, cargo generation, trade activities and weather conditions management.

The struct **state** encapsulates the shared data structures and the weather process identifier. 
It includes pointers to shared data structures for the general configuration, ports, ships, and cargo. 
- **shm_general_t type**: used for general configuration;
- **shm_port_t**: shared memory segments for ports;
- **shm_ship_t**: shared memory segments for ships;
- **shm_cargo_t**: shared memory segments for cargo.

The `main()` initializes the signal handlers, reads the configuration from a file, and sets up shared memory and 
semaphores. 
Then it forks processes for ports, ships, and weather. 

The simulation starts after synchronizing the processes using semaphores.

### Process initialization
`run_ports()`, `run_ships()`, and `run_weather()` fork processes for ports, ships, and weather, respectively. 
These functions use the `run_process()` helper function for creating child processes.

### Signal handlers
`signal_handler_init()` sets up signal handlers for various signals such as SIGALRM, SIGSEGV, SIGTERM, 
and SIGINT. 
The signals are used for daily reporting, error handling, and graceful termination.

### Reporting functions
`print_daily_report()` and `print_final_report()` display daily and final simulation reports, respectively. 
These reports include information about cargo, ships, ports, and weather conditions.

### Terminating the simulation
When we reach the end of the simulation (after SO_DAYS seconds), ships and ports processes detach themselves from shared memory and terminate.

The master process waits for child processes termination and then it cleans up the shared memory and semaphores.
- `check_ships_all_dead()` determines whether all ships are dead. 
- `close_all()` terminates the simulation, sending signals to all relevant processes, deleting IPC resources, and printing the final report.

## Shared memory
`lib/shm.h` is a helper library that has been used as a facilitation to create/attach/detach/destroy 
shared memory segment on the aforementioned `shm_*` header files dedicated to the shared sturctures.

## Semaphore
`lib/semaphore.h` is a helper library that has been used as a facilitation to create/handle/destroy arrays of semaphores.

// DETTAGLIO DI COME USIAMO I SEMAFORI NEI VARI CASI

## Signal
- **SIGDAY**: defined as SIGUSR1, used by master to signal a new day which triggers new cargo generations and daily reports;
- **SIGSWELL**: defined as SIGUSR2, used by weather to signal if a SWELL occurs to a port;
- **SIGSTORM**: defined as SIGUSR2, used by weather to signal if a STORM occurs to a ship;
- **SIGMAELSTROM**: defined as SIGTERM, used by weather to signal if a MAELSTROM occurs to a ship that terminates after it.

## Message
`src/msg_commerce.h` contains structures and functions to handle messages between ports and ships.

Every message has a status used to decode the request and brings a support structure containing all the possible informations.

From port to ship message status: 
- **STATUS_ACCEPTED**: port accepts all the offer proposed from the ship;
- **STATUS_PARTIAL**: port accepts a part of the offer proposed from the ship, depending on the request;
- **STATUS_REFUSED**: port refuses the ship's offer.

From ship to port message status:
- **STATUS_SELL**: ship sends goodies to port
- **STATUS_BUY**: ship take goodies from port

## Port
The port interacts with ships, manages cargo, and participates in commerce through offers and demands.
### Functionality
- `main()` initializes the state, attaches to shared memory, generates coordinates, and enters the main loop.
- `loop()` represents the main operational logic of the port, handling daily tasks and processing incoming commerce messages.
- `respond_ship_msg()` manages the response to commerce messages, including buying and selling cargo.
- `signal_handler()` handle various signals, including simulating swell effects and responding to termination signals.

## Ship
The ship moves between ports, trades cargo, and responds to signals such as storms and maelstroms.
### Functionality
- `main()` initializes the state, attaches to shared memory, generates initial location, and enters the main loop.
- `loop()` moves to a randomly chosen port and starts trading.
- `trade()` manages the trade process, including buying and selling cargo.
- `sell()` initiates the process of selling cargo to the current port sending a commerce message to the port and processes the response.
- `buy()` initiates the process of buying cargo from the current port sending a commerce message to the port and processes the response.
- `signal_handler()` handles various signals, including simulating storms and maelstroms and responding to termination signals.

## Weather
At the beginning of each simulation's day the weather process receives the SIGDAY signal from the master process. 
It then proceeds to send the SIGSTORM and SIGSWELL signals to random ships and ports respectively. 
It also implements an itimer in order to be able to send the SIGMAELTROM signal to random ship every SO_MAELTROM simulated hours.