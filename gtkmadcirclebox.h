#ifndef __GTK_MADCIRCLEBOX_H__
#define __GTK_MADCIRCLEBOX_H__
/* 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */



#include <gdk/gdk.h>
#include <gtk/gtkbox.h>


G_BEGIN_DECLS

#define GTK_TYPE_MADCIRCLEBOX            (gtk_madcirclebox_get_type ())
#define GTK_MADCIRCLEBOX(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_MADCIRCLEBOX, GtkMadCircleBox))
#define GTK_MADCIRCLEBOX_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_MADCIRCLEBOX, GtkMadCircleBoxClass))
#define GTK_IS_MADCIRCLEBOX(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_MADCIRCLEBOX))
#define GTK_IS_MADCIRCLEBOX_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_MADCIRCLEBOX))
#define GTK_MADCIRCLEBOX_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_MADCIRCLEBOX, GtkMadCircleBoxClass))


typedef struct _GtkMadCircleBox	      GtkMadCircleBox;
typedef struct _GtkMadCircleBoxClass  GtkMadCircleBoxClass;

struct _GtkMadCircleBox
{
  GtkBox box;
  gint minradius;
  gint maxsize;
};

struct _GtkMadCircleBoxClass
{
  GtkBoxClass parent_class;
};


GType	   gtk_madcirclebox_get_type (void) G_GNUC_CONST;
GtkWidget* gtk_madcirclebox_new	     (gboolean homogeneous,
			      gint spacing);


G_END_DECLS

#endif /* __GTK_MADCIRCLEBOX_H__ */
