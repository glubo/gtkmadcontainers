/* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the
* Free Software Foundation, Inc., 59 Temple Place - Suite 330,
* Boston, MA 02111-1307, USA.
*/


#include "gtkmadcirclebox.h"
#include <gtk/gtk.h>
#include "math.h"
#include <cairo.h>



static void gtk_madcirclebox_size_request (GtkWidget * widget, GtkRequisition * requisition);
static void gtk_madcirclebox_size_allocate (GtkWidget * widget, GtkAllocation * allocation);
gboolean gtk_madcirclebox_expose(GtkWidget *widget, GdkEventExpose *event, gpointer user_data);


gboolean gtk_madcirclebox_expose(GtkWidget *widget, GdkEventExpose *event, gpointer user_data){
	GtkAllocation * allocation;
	GtkBox *box;
	gint size;
	gint radius;
	gint width;
	gint height;
	gint x,y;
	gint nvis_children;
	GtkBoxChild *child;
	GList *children;
	cairo_t *cr;
	GtkStyle *style;


	box = GTK_BOX (widget);
	allocation = &widget->allocation;

	nvis_children = 0;
	children = box->children;

	while (children) {
		child = children->data;
		children = children->next;

		if (GTK_WIDGET_VISIBLE (child->widget)) {
			nvis_children += 1;
		}
	}
	width = (allocation->width - GTK_CONTAINER (box)->border_width * 2 );
	height = (allocation->height - GTK_CONTAINER (box)->border_width * 2 );
	size = MIN(width, height);

	x = allocation->x + (allocation->width-size)/2;
	y = allocation->y + (allocation->height-size)/2;

	radius = size/(1.0+sin(M_PI/(double)nvis_children))/2;

	style = gtk_widget_get_style(widget);
	cr = gdk_cairo_create(widget->window);
	gdk_cairo_set_source_color(cr, &style->fg[0]);
	cairo_set_line_width (cr, 0.5);
	cairo_arc(cr, x+size/2, y+size/2, radius, 0, M_PI*2.0);
	cairo_stroke(cr);
	gdk_cairo_set_source_color(cr, &style->fg[2]);
	cairo_arc(cr, x+size/2, y+size/2, radius+0.5, 0, M_PI*2.0);
	cairo_stroke(cr);

	cairo_destroy(cr);


	return FALSE;
};

G_DEFINE_TYPE (GtkMadCircleBox, gtk_madcirclebox, GTK_TYPE_BOX)
		 static void gtk_madcirclebox_class_init (GtkMadCircleBoxClass * class)
{
	GtkWidgetClass *widget_class;

	widget_class = (GtkWidgetClass *) class;

	widget_class->size_request = gtk_madcirclebox_size_request;
	widget_class->size_allocate = gtk_madcirclebox_size_allocate;
}

static void
gtk_madcirclebox_init (GtkMadCircleBox * madcirclebox)
{
	g_signal_connect (G_OBJECT (madcirclebox), "expose-event",	G_CALLBACK (gtk_madcirclebox_expose), NULL);
}

GtkWidget *
gtk_madcirclebox_new (gboolean homogeneous, gint spacing)
{
	GtkMadCircleBox *madcirclebox;

	madcirclebox = g_object_new (GTK_TYPE_MADCIRCLEBOX, NULL);

	GTK_BOX (madcirclebox)->spacing = spacing;
	GTK_BOX (madcirclebox)->homogeneous = homogeneous ? TRUE : FALSE;

	return GTK_WIDGET (madcirclebox);
}


static void
gtk_madcirclebox_size_request (GtkWidget * widget, GtkRequisition * requisition)
{
	GtkBox *box;
	GtkMadCircleBox *mcbox;
	GtkBoxChild *child;
	GList *children;
	gint nvis_children;
	gint size;
	gint maxsize;
	gint radius;

	box = GTK_BOX (widget);
	requisition->width = 0;
	requisition->height = 0;
	nvis_children = 0;
	maxsize = 0;
	radius = 0;

	children = box->children;
	while (children) {
		child = children->data;
		children = children->next;

		if (GTK_WIDGET_VISIBLE (child->widget)) {
			GtkRequisition child_requisition;

			gtk_widget_size_request (child->widget, &child_requisition);

			size = child_requisition.height * child_requisition.height + child_requisition.width * child_requisition.width;
			maxsize = MAX (maxsize, size);

			nvis_children += 1;
		}
	}

	if (nvis_children > 0) {
		maxsize = (int) sqrt (maxsize/2);
		radius = (int) ((double) maxsize / sin (M_PI / (double) nvis_children));
		requisition->width += 2 * radius + 2 * maxsize;
		requisition->height += 2 * radius + 2 * maxsize;
	}

	requisition->width += GTK_CONTAINER (box)->border_width * 2;
	requisition->height += GTK_CONTAINER (box)->border_width * 2;
	
	mcbox->minradius = radius;
	mcbox->maxsize = maxsize;
}

static void
gtk_madcirclebox_size_allocate (GtkWidget * widget, GtkAllocation * allocation)
{
	GtkBox *box;
	GtkBoxChild *child;
	GList *children;
	GtkAllocation child_allocation;
	gint nvis_children;
	gint child_width;
	gint child_height;
	gint width;
	gint height;
	gint x;
	gint y;
	gint cx;
	gint cy;
	gint i;
	gint childsize;
	gint size;
	gint radius;
	GtkTextDirection direction;

	box = GTK_BOX (widget);
	widget->allocation = *allocation;

	direction = gtk_widget_get_direction (widget);

	nvis_children = 0;
	children = box->children;

	while (children) {
		child = children->data;
		children = children->next;

		if (GTK_WIDGET_VISIBLE (child->widget)) {
			nvis_children += 1;
		}
	}

	if (nvis_children > 0) {
		width = (allocation->width - GTK_CONTAINER (box)->border_width * 2 );
		height = (allocation->height - GTK_CONTAINER (box)->border_width * 2 );
		size = MIN(width, height);

		x = allocation->x + (allocation->width-size)/2;
		y = allocation->y + (allocation->height-size)/2;
		printf("%lf\n",sin(M_PI/(double)nvis_children));
		radius = size/(1.0+sin(M_PI/(double)nvis_children))/2.0;
		childsize = sqrt(2.0)*radius * sin(M_PI/(double)nvis_children); 
		printf("w:%d h:%d r:%d cs:%d\n",width, height, radius, childsize);


		children = box->children;
		i = 0;
		while (children) {
			child = children->data;
			children = children->next;

			if ((child->pack == GTK_PACK_START)
		&& GTK_WIDGET_VISIBLE (child->widget)) {
				GtkRequisition child_requisition;

				gtk_widget_get_child_requisition (child->widget, &child_requisition);

				child_width = child_requisition.width;
				child_height = child_requisition.height;

				if (child->expand) {
					child_width = childsize;
					child_height = childsize;
				}

				cx = x + 0.5*size + radius*sin((double)i*2.0*M_PI/(double)nvis_children);
				cy = y + 0.5*size - radius*cos((double)i*2.0*M_PI/(double)nvis_children);

				child_allocation.width = child_width;
				child_allocation.height = child_height;
				child_allocation.x = cx - (child_width)*0.5 ;
				child_allocation.y = cy - (child_height)*0.5 ;

				gtk_widget_size_allocate (child->widget, &child_allocation);
				i += 1;
			}
		}

		children = box->children;
		i = -1;
		while (children) {
			child = children->data;
			children = children->next;

			if ((child->pack == GTK_PACK_END) && GTK_WIDGET_VISIBLE (child->widget)) {
				GtkRequisition child_requisition;

				gtk_widget_get_child_requisition (child->widget, &child_requisition);

				child_width = child_requisition.width;
				child_height = child_requisition.height;

				if (child->expand) {
					child_width = childsize;
					child_height = childsize;
				}

				cx = x + 0.5*size + radius*sin((double)i*2.0*M_PI/(double)nvis_children);
				cy = y + 0.5*size - radius*cos((double)i*2.0*M_PI/(double)nvis_children);

				child_allocation.width = child_width;
				child_allocation.height = child_height;
				child_allocation.x = cx - (child_width)*0.5 ;
				child_allocation.y = cy - (child_height)*0.5 ;

				gtk_widget_size_allocate (child->widget, &child_allocation);
				i -= 1;
			}
		}
	}
}
