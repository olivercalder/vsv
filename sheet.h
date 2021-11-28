
#ifndef __SHEET_H
#define __SHEET_H

#include <ncurses.h>

typedef struct cell cell_t;
typedef struct sheet sheet_t;
typedef struct session session_t;

typedef (void *) pointer_t;

typedef enum sheet_status_t {
    ERR_OK,
    ERR_NOMEM,
};

typedef enum cell_type_t {
    CELL_STRING,
    CELL_INT,
    CELL_FLOAT,
};

typedef enum change_type_t {
    CHANGE_SHEET,
    CHANGE_CELL,
};

struct cell {
    cell_type_t type;
    cell_value_t value;
    union {
        char *s;
        long long i;
        double f;
    };
};

struct sheet {
    unsigned int num_rows, num_cols;
    cell_t *cells[];
    unsigned int row_heights[];
    unsigned int col_widths[];
};

struct change {
    struct change *prev_change;
    struct change *next_change;
    change_type_t type;
    pointer_t *location;
    pointer_t orig_value;
    pointer_t new_value;
};

struct session {
    char *filename;
    change_t *changes;
    sheet_t *sheet;
    WINDOW *window;
    unsigned int default_cell_width, default_cell_height;
};


cell_t *cell_new(cell_type_t type, cell_value_t value);

void cell_cleanup(cell_t *cell);


sheet_t *sheet_new(unsigned int num_rows, unsigned int num_cols);

sheet_t *sheet_insert_row(sheet_t *orig, int row_index);

sheet_t *sheet_remove_row(sheet_t *orig, int row_index);

sheet_t *sheet_insert_col(sheet_t *orig, int col_index);

sheet_t *sheet_remove_col(sheet_t *orig, int col_index);

void sheet_cleanup(sheet_t *sheet);


change_t *change_new(change_type_t type, pointer_t *location, pointer_t orig_value, pointer_t new_value, change_t *prev_change);


int session_init(session_t *session, char *filename);

#endif /* sheet.h */
