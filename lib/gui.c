#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include "gui.h"
#include "err_sys.h"

/* Function initialize GUI and some of its elements */
void gui_init(int *argc, char ***argv)
{
    gtk_init(argc, argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "lib/app.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    progress_label = GTK_LABEL(gtk_builder_get_object(builder, "progress_label"));
    output = GTK_WIDGET(gtk_builder_get_object(builder, "output_file"));
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
    /* Path to file and directory */
    GFile *input_file = gtk_file_chooser_get_file(input);
    char  *input_file_path = g_file_get_path(input_file);
    GFile *parent_dir = g_file_get_parent(input_file);
    char  *parent_dir_path = g_file_get_path(parent_dir);
    /* Output_file */
    const gchar *output_file_name = gtk_entry_get_text(GTK_ENTRY(output));
    
    if(input_file && strlen(output_file_name) > 0)
    {
        gtk_label_set_text(progress_label, "Compressing...");
        
        /* Make filepath */
        size_t len_parent = strlen(parent_dir_path);
        size_t len_output = strlen(output_file_name);
        char * output_file_path = calloc(sizeof(char), len_parent + len_output + 7);

        if(huff_selected()){
            memcpy(output_file_path, parent_dir_path, len_parent);
            /* Add slash to path */
            output_file_path[len_parent] = '/';
            memcpy(output_file_path + len_parent + 1, output_file_name, len_output);
            memcpy(output_file_path + len_parent + len_output + 1, ".huff", 5);
        }
        else{
            memcpy(output_file_path, parent_dir_path, len_parent);
            /* Add slash to path */
            output_file_path[len_parent] = '/';
            memcpy(output_file_path + len_parent + 1, output_file_name, len_output);
            memcpy(output_file_path + len_parent + len_output + 1, ".lzw", 4);
        }

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
                return_status = execl("huffman_compression/bin/run", "run", "-c", "-i", input_file_path, "-o", output_file_path, NULL);
                if(return_status == -1)
                    err_sys("Process error");
            }
            else
            {
                /* LZW */
                return_status = execl("lzw_compression/bin/run", "run", "-c", "-i", input_file_path, "-o", output_file_path, NULL);
                if(return_status == -1)
                    err_sys("Process error");
            }
        }
        free(output_file_path);
    }
    else
    {
        gtk_label_set_text(progress_label, "Please enter an input/output file!");
    }
}

/* Function triggers when decompress button is pressed */
void decompress_click()
{
    int return_status;
    /* Path to file and directory */
    GFile *input_file = gtk_file_chooser_get_file(input);
    char  *input_file_path = g_file_get_path(input_file);
    GFile *parent_dir = g_file_get_parent(input_file);
    char  *parent_dir_path = g_file_get_path(parent_dir);
    /* Output_file */
    const gchar *output_file_name = gtk_entry_get_text(GTK_ENTRY(output));
    
    if(input_file && strlen(output_file_name) > 0)
    {
        gtk_label_set_text(progress_label, "Decompressing...");
        
        /* Make filepath */
        size_t len_parent = strlen(parent_dir_path);
        size_t len_output = strlen(output_file_name);
        char * output_file_path = calloc(sizeof(char), len_parent + len_output + 1);

        if(huff_selected()){
            memcpy(output_file_path, parent_dir_path, len_parent);
            /* Add slash to path */
            output_file_path[len_parent] = '/';
            memcpy(output_file_path + len_parent + 1, output_file_name, len_output);
        }
        else{
            memcpy(output_file_path, parent_dir_path, len_parent);
            /* Add slash to path */
            output_file_path[len_parent] = '/';
            memcpy(output_file_path + len_parent + 1, output_file_name, len_output);
        }

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
                return_status = execl("huffman_compression/bin/run", "run", "-d", "-i", input_file_path, "-o", output_file_path, NULL);
                if(return_status == -1)
                    err_sys("Process error");
            }
            else
            {
                /* LZW */
                return_status = execl("lzw_compression/bin/run", "run", "-d", "-i", input_file_path, "-o", output_file_path, NULL);
                if(return_status == -1)
                    err_sys("Process error");
            }
        }
        free(output_file_path);
    }
    else
    {
        gtk_label_set_text(progress_label, "Please enter an input/output file!");
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
