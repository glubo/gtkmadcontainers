
#include <stdio.h>
#include <stdlib.h>
#include "gtk/gtk.h"
#include "gtkmadcirclebox.h"

/* This is a callback function. The data arguments are ignored
 * in this example. More on callbacks below. */
static void hello( GtkWidget *widget,
                   gpointer   data )
{
    g_print ("Hello World\n");
}


static void destroy( GtkWidget *widget,
                     gpointer   data )
{
    gtk_main_quit ();
}

int main( int   argc,
          char *argv[] )
{
    /* GtkWidget is the storage type for widgets */
    GtkWidget *window;
	 GtkWidget *mcb;
    GtkWidget *button;
	 int i;
    
    /* This is called in all GTK applications. Arguments are parsed
     * from the command line and are returned to the application. */
    gtk_init (&argc, &argv);
    
    /* create a new window */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    
    g_signal_connect (G_OBJECT (window), "delete_event",
		      G_CALLBACK (destroy), NULL);
    
    g_signal_connect (G_OBJECT (window), "destroy",
		      G_CALLBACK (destroy), NULL);
    
    /* Sets the border width of the window. */
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    
	 mcb = gtk_madcirclebox_new(TRUE, 0);
	 for(i=0; i<40; i++){
		 /* Creates a new button with the label "Hello World". */
		 button = gtk_button_new_with_label ("Hello");
		 
		 /* When the button receives the "clicked" signal, it will call the
		  * function hello() passing it NULL as its argument.  The hello()
		  * function is defined above. */
		 g_signal_connect (G_OBJECT (button), "clicked",
					G_CALLBACK (hello), NULL);
		 
		 /* This will cause the window to be destroyed by calling
		  * gtk_widget_destroy(window) when "clicked".  Again, the destroy
		  * signal could come from here, or the window manager. */
		 g_signal_connect_swapped (G_OBJECT (button), "clicked",
						G_CALLBACK (gtk_widget_destroy),
											G_OBJECT (window));
		 
		 /* This packs the button into the window (a gtk container). */
		 if(i%2){
		 	gtk_box_pack_start (GTK_BOX (mcb), button, 1, 0, 0);
		 }else{
		 	gtk_box_pack_end (GTK_BOX (mcb), button, 1, 0, 0);
		 }
	}
	 gtk_container_add (GTK_CONTAINER (window), mcb);
    
    /* The final step is to display this newly created widget. */
    gtk_widget_show (button);
    
    /* and the window */
    gtk_widget_show_all (window);
    
    /* All GTK applications must have a gtk_main(). Control ends here
     * and waits for an event to occur (like a key press or
     * mouse event). */
    gtk_main ();
    
    return 0;
}
