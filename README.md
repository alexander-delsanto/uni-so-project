# os-project

## Requisiti implementativi:
- utilizzare almeno memoria condivisa, semaforie un meccanismo di comunicazione fra processi a scelta fra code di messaggi o pipe;
- sfruttare tecniche di divisione in moduli del codice (i vari processi devono essere lanciati da eseguibili diversi con `execve()`;
- compilare mediante utility `make`;
- massimizzare il grado di concorrenza fra processi;
- deallocare risorse IPC che sono state allocate dai processi al termine del gioco;
- compilare con almeno le seguenti opzioni di compilazione: `gcc -std=c89 -Wpedantic`;
- poter eseguire correttamente su una macchina che presenta parallelismo.

**Ricordarsi di definire la macro `GNU_SOURCE` o compilare il progetto con il flag `-D_GNU_SOURCE`.**

#### ANSI C standard:
- dichiarare le variabili all'inizio del blocco;
- commenti:  `/* commento */`    NON `// commento`

#### Coding Style:
https://www.kernel.org/doc/html/v4.10/process/coding-style.html
