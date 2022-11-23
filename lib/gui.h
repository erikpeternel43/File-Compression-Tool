#include <gtk/gtk.h>
#include <stdbool.h>

#ifndef GUI_H
#define GUI_H

GtkWidget *window;
GtkLabel *progress_label;
GtkFileChooser *input;
GtkFileChooser *output;
GtkBuilder *builder;
GtkRadioButton *radio_button;

/* Functions */
void gui_init(int *argc, char ***argv);
void exit_app();
void compress_click();
void decompress_click();
void input_selected();
bool huff_selected();

#endif
