#include <sys/types.h>
#include <sys/wait.h>
#include "gui.h"
#include "err_sys.h"

/* Function initialize GUI and some of its elements */
void gui_init(int *argc, char ***argv)
{
    gtk_init(argc, argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "../lib/app.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    progress_label = GTK_LABEL(gtk_builder_get_object(builder, "progress_label"));
    output = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "output_file"));
    input = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "input_file"));
    radio_button = GTK_RADIO_BUTTON(gtk_builder_get_object(builder, "radiobutton1"));

    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);

    gtk_widget_show_all(window);
    gtk_main();
}

/* Function triggers when compress button is pressed */
void compress_click()
{
    int return_status;
    gchar *input_file = gtk_file_chooser_get_filename(input);
    gchar *output_file = gtk_file_chooser_get_filename(output);
    if(input_file && output_file)
    {
        gtk_label_set_text(progress_label, "Compressing...");

        /* Creating child which will execute program for compress - either huffman or lzw */
        pid_t pid = fork();
        if(pid == -1)
            err_sys("Failed to fork");
        if(pid > 0)
        {
            int status;
            return_status = waitpid(pid, &status, 0);
            if(return_status == -1)
                err_sys("Waitpid error");
            gtk_label_set_text(progress_label, "Compression done!");
        }
        else
        {
            if(huff_selected())
            {
                /* Huffman */
                return_status = execl("../huffman_compression/bin/run", "run", "-c", "-i", input_file, "-o", output_file, NULL);
                if(return_status == -1)
                    err_sys("Process error");
            }
            else
            {
                /* LZW */
                return_status = execl("../lzw_compression/bin/run", "run", "-c", "-i", input_file, "-o", output_file, NULL);
                if(return_status == -1)
                    err_sys("Process error");
            }
        }
    }
    else
    {
        gtk_label_set_text(progress_label, "Please select an input/output file!");
    }
}

/* Function triggers when decompress button is pressed */
void decompress_click()
{
    int return_status;
    gchar *input_file = gtk_file_chooser_get_filename(input);
    gchar *output_file = gtk_file_chooser_get_filename(output);
    if(input_file && output_file)
    {
        gtk_label_set_text(progress_label, "Decompressing...");
        
        /* Creating child which will execute program for compress - either huffman or lzw */
        pid_t pid = fork();
        if(pid == -1)
            err_sys("Failed to fork");
        if(pid > 0)
        {
            int status;
            return_status = waitpid(pid, &status, 0);
            if(return_status == -1)
                err_sys("Waitpid error");
            gtk_label_set_text(progress_label, "Decompression done!");
        }
        else
        {
            if(huff_selected())
            {
                /* Huffman */
                return_status = execl("../huffman_compression/bin/run", "run", "-d", "-i", input_file, "-o", output_file, NULL);
                if(return_status == -1)
                    err_sys("Process error");
            }
            else
            {
                /* LZW */
                return_status = execl("../lzw_compression/bin/run", "run", "-d", "-i", input_file, "-o", output_file, NULL);
                if(return_status == -1)
                    err_sys("Process error");
            }
        }
    }
    else
    {
        gtk_label_set_text(progress_label, "Please select an input/output file!");
    }

}

/* Checks whether huffman option is selected or not */
bool huff_selected()
{
    GSList *tmp_list = gtk_radio_button_get_group(radio_button);
    return gtk_toggle_button_get_active(tmp_list->data);
}

void exit_app()
{
    gtk_main_quit();
}
