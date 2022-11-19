#include <stdio.h>
#include <stdlib.h>

/* Macro for errors */
#ifndef ERR_SYS_H

#define err_sys(mess) { fprintf(stderr,"Error: %s.\n", mess); exit(1); }

#endif
