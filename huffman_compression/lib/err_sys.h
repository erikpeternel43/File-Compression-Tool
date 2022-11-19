#ifndef ERR_SYS_H
    #define err_sys(mess) { fprintf(stderr,"Error: %s.\n", mess); exit(1); }
#endif