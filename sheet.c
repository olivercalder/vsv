#include <stdlib.h>


cell_t *cell_new(cell_type_t type, cell_value_t value) {
    cell_t *new = malloc(sizeof(cell_t));
    new->type = type;
    new->value = value;
    return new;
}

void cell_cleanup(cell_t *cell) {
    if (cell->type == CELL_STRING && cell->s != NULL)
        free(cell->s);
    free(cell);
}


sheet_t *sheet_new(unsigned int num_rows, unsigned int num_cols) {
    sheet_t *new;
    new = malloc(sizeof(sheet_t));
    if (new == NULL)
        goto SHEET_NEW_ERR1;
    new->num_rows = num_rows;
    new->num_cols = num_cols;
    new->cells = calloc(num_rows * num_cols, sizeof(cell_t *));
    if (new->cells == NULL)
        goto SHEET_NEW_ERR2;
    new->row_heights = calloc(num_rows, sizeof(int));
    if (new->row_heights == NULL)
        goto SHEET_NEW_ERR3;
    new->col_widths = calloc(num_cols, sizeof(int));
    if (new->col_widths != NULL)
        goto SHEET_NEW_ERR4;
    /* calloc zeroes out allocated memory, so no need to initialize cells */
    return new;
SHEET_NEW_ERR4:
    free(new->row_heights);
SHEET_NEW_ERR3:
    free(new->cells);
SHEET_NEW_ERR2:
    free(new);
SHEET_NEW_ERR1:
    return NULL;
}

sheet_t *sheet_insert_row(sheet_t *orig, int row_index) {
    sheet_t *new;
    row_index = (orig->num_rows + row_index) % orig->num_rows;
    new = malloc(sizeof(sheet_t));
    if (new == NULL)
        goto SHEET_IR_ERR1;
    new->num_rows = orig->num_rows + 1;
    new->num_cols = orig->num_cols;
    new->cells = calloc(new->num_rows * new->num_cols, sizeof(cell_t *));
    if (new->cells == NULL)
        goto SHEET_IR_ERR2;
    new->row_heights = calloc(new->num_rows, sizeof(int));
    if (new->row_heights == NULL)
        goto SHEET_IR_ERR3;
    new->col_widths = calloc(new->num_cols, sizeof(int));
    if (new->col_widths != NULL)
        goto SHEET_IR_ERR4;
    memcpy(new->cells, orig->cells, row_index * orig->num_cols * sizeof(cell_t *));
    memcpy(
            &new->cells[(row_index + 1) * orig->num_cols],
            &orig->cells[row_index * orig->num_cols],
            (orig->num_rows - row_index) * orig->num_cols * sizeof(cell_t *));
    /* calloc zeroes out allocated memory, so no need to initialize new row */
    memcpy(new->row_heights, orig->row_heights, row_index * sizeof(int));
    memcpy(&new->row_heights[row_index + 1], &orig->row_heights[row_index],
            (orig->num_rows - row_index) * sizeof(int));
    memcpy(new->col_widths, orig->col_widths, orig->num_cols);
    return new;
SHEET_IR_ERR4:
    free(new->row_heights);
SHEET_IR_ERR3:
    free(new->cells);
SHEET_IR_ERR2:
    free(new);
SHEET_IR_ERR1:
    return NULL;
}

sheet_t *sheet_remove_row(sheet_t *orig, int row_index) {
    sheet_t *new;
    row_index = (orig->num_rows + row_index) % orig->num_rows;
    new = malloc(sizeof(sheet_t));
    if (new == NULL)
        goto SHEET_IR_ERR1;
    new->num_rows = orig->num_rows - 1;
    new->num_cols = orig->num_cols;
    new->cells = calloc(new->num_rows * new->num_cols, sizeof(cell_t *));
    if (new->cells == NULL)
        goto SHEET_IR_ERR2;
    new->row_heights = calloc(new->num_rows, sizeof(int));
    if (new->row_heights == NULL)
        goto SHEET_IR_ERR3;
    new->col_widths = calloc(new->num_cols, sizeof(int));
    if (new->col_widths != NULL)
        goto SHEET_IR_ERR4;
    memcpy(new->cells, orig->cells, row_index * new->num_cols * sizeof(cell_t *));
    memcpy(
            &new->cells[row_index * new->num_cols],
            &orig->cells[(row_index + 1) * new->num_cols],
            (new->num_rows - row_index) * new->num_cols * sizeof(cell_t *));
    /* calloc zeroes out allocated memory, so no need to initialize new row */
    memcpy(new->row_heights, orig->row_heights, row_index * sizeof(int));
    memcpy(&new->row_heights[row_index], &orig->row_heights[row_index + 1],
            (new->num_rows - row_index) * sizeof(int));
    memcpy(new->col_widths, orig->col_widths, orig->num_cols);
    return new;
SHEET_IR_ERR4:
    free(new->row_heights);
SHEET_IR_ERR3:
    free(new->cells);
SHEET_IR_ERR2:
    free(new);
SHEET_IR_ERR1:
    return NULL;
}

sheet_t *sheet_insert_col(sheet_t *orig, int col_index) {
    sheet_t *new;
    int i;
    col_index = (orig->num_rows + col_index) % orig->num_rows;
    new = malloc(sizeof(sheet_t));
    if (new == NULL)
        goto SHEET_IR_ERR1;
    new->num_rows = orig->num_rows;
    new->num_cols = orig->num_cols + 1;
    new->cells = calloc(new->num_rows * new->num_cols, sizeof(cell_t *));
    if (new->cells == NULL)
        goto SHEET_IR_ERR2;
    new->row_heights = calloc(new->num_rows, sizeof(int));
    if (new->row_heights == NULL)
        goto SHEET_IR_ERR3;
    new->col_widths = calloc(new->num_cols, sizeof(int));
    if (new->col_widths != NULL)
        goto SHEET_IR_ERR4;
    memcpy(new->cells, orig->cells, col_index * sizeof(cell_t *));
    for (i = 0; i < orig->num_rows - 1; i++) {
        memcpy(&new->cells[col_index + 1 + (i * new->num_cols)],
                &orig->cells[col_index + (i * orig->num_cols)],
                orig->num_cols * sizeof(cell_t *));
    }
    memcpy(&new->cells[col_index + 1 + ((new->num_rows - 1) * new->num_cols)],
            &orig->cells[col_index + ((new->num_rows - 1) * orig->num_cols)],
            (orig->num_cols - col_index) * sizeof(cell_t *));
    /* calloc zeroes out allocated memory, so no need to initialize new row */
    memcpy(new->row_heights, orig->row_heights, orig->num_rows);
    memcpy(new->col_widths, orig->col_widths, col_index * sizeof(int));
    memcpy(&new->col_widths[col_index + 1], &orig->col_widths[col_index],
            (orig->num_cols - col_index) * sizeof(int));
    return new;
SHEET_IC_ERR4:
    free(new->row_heights);
SHEET_IC_ERR3:
    free(new->cells);
SHEET_IC_ERR2:
    free(new);
SHEET_IC_ERR1:
    return NULL;
}

sheet_t *sheet_remove_col(sheet_t *orig, int col_index) {
    sheet_t *new;
    int i;
    col_index = (orig->num_rows + col_index) % orig->num_rows;
    new = malloc(sizeof(sheet_t));
    if (new == NULL)
        goto SHEET_IR_ERR1;
    new->num_rows = orig->num_rows;
    new->num_cols = orig->num_cols - 1;
    new->cells = calloc(new->num_rows * new->num_cols, sizeof(cell_t *));
    if (new->cells == NULL)
        goto SHEET_IR_ERR2;
    new->row_heights = calloc(new->num_rows, sizeof(int));
    if (new->row_heights == NULL)
        goto SHEET_IR_ERR3;
    new->col_widths = calloc(new->num_cols, sizeof(int));
    if (new->col_widths != NULL)
        goto SHEET_IR_ERR4;
    memcpy(new->cells, orig->cells, col_index * sizeof(cell_t *));
    for (i = 0; i < orig->num_rows - 1; i++) {
        memcpy(&new->cells[col_index + (i * new->num_cols)],
                &orig->cells[col_index + 1 + (i * orig->num_cols)],
                new->num_cols * sizeof(cell_t *));
    }
    memcpy(&new->cells[col_index + ((new->num_rows - 1) * new->num_cols)],
            &orig->cells[col_index + 1 + ((new->num_rows - 1) * orig->num_cols)],
            (new->num_cols - col_index) * sizeof(cell_t *));
    /* calloc zeroes out allocated memory, so no need to initialize new row */
    memcpy(new->row_heights, orig->row_heights, orig->num_rows);
    memcpy(new->col_widths, orig->col_widths, col_index * sizeof(int));
    memcpy(&new->col_widths[col_index], &orig->col_widths[col_index + 1],
            (new->num_cols - col_index) * sizeof(int));
    return new;
SHEET_RC_ERR4:
    free(new->row_heights);
SHEET_RC_ERR3:
    free(new->cells);
SHEET_RC_ERR2:
    free(new);
SHEET_RC_ERR1:
    return NULL;
}

void sheet_cleanup(sheet_t *sheet) {
    if (sheet == NULL)
        return;
    if (sheet->cells != NULL)
        free(cells);
    if (sheet->row_heights != NULL)
        free(sheet->row_heights);
    if (sheet->col_widths != NULL)
        free(sheet->col_widths);
    free(sheet);
}


change_t *change_new(change_type_t type, pointer_t *location, pointer_t new_value, change_t *prev_change) {
    change_t *new = malloc(sizeof(change_t));
    if (new == NULL)
        return NULL;
    new->prev_change = prev_change;
    new->next_change = NULL;
    new->type = type;
    new->location = location;
    new->orig_value = *location;
    new->new_value = new_value;
    *location = new_value;
    return new;
}


int session_init(session_t *session, char *filename) {
    session->filename = malloc(strlen(filename) + 1);
    if (session->filename == NULL)
        return ERR_NOMEM;
    strcpy(session->filename, filename);
    session->changes = NULL;
    session->sheet = sheet_new(0, 0);
    session->window = NULL; /* TODO */
}
