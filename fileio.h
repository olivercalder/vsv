
#ifndef __FILEIO_H
#define __FILEIO_H

#include "sheet.h"

typedef enum {
    ERR_OK,
    ERR_FILENAME,
    ERR_PERMS,
    ERR_SHEET,
} io_err_t;

io_err_t load_csv(sheet_t *sheet, char *filename, char ifs);

io_err_t write_csv(sheet_t *sheet, char *filename, char ofs);

#endif /* fileio.h */
