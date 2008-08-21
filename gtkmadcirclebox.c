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



static void gtk_madcirclebox_size_request (GtkWidget * widget, GtkRequisition * requisition);
static void gtk_madcirclebox_size_allocate (GtkWidget * widget, GtkAllocation * allocation);


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
		maxsize = (int) sqrt (maxsize);
		radius = (int) ((double) maxsize / tan (M_PI / (double) nvis_children));
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
	gint nexpand_children;
	gint child_width;
	gint width;
	gint height;
	gint extra;
	gint x;
	gint y;
	gint childsize;
	gint size;
	gint radius;
	GtkTextDirection direction;

	box = GTK_BOX (widget);
	widget->allocation = *allocation;

	direction = gtk_widget_get_direction (widget);

	nvis_children = 0;
	nexpand_children = 0;
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
		radius = size/(1.0+tan(M_PI/(double)nvis_children));
		childsize = radius *tan(M_PI/(double)nvis_children);


		children = box->children;
		while (children) {
			child = children->data;
			children = children->next;

			if ((child->pack == GTK_PACK_START)
		&& GTK_WIDGET_VISIBLE (child->widget)) {
					child_width = childsize;
					child_height = childsize;
				if (box->homogeneous) {
					if (nvis_children == 1)
						child_width = width;
					else
						child_width = extra;

					nvis_children -= 1;
					width -= extra;
				}else {
					GtkRequisition child_requisition;

					gtk_widget_get_child_requisition (child->widget, &child_requisition);

					child_width = child_requisition.width + child->padding * 2;

					if (child->expand) {
						if (nexpand_children == 1)
							child_width += width;
						else
							child_width += extra;

						nexpand_children -= 1;
						width -= extra;
					}
				}

				if (child->fill) {
					child_allocation.width = MAX (1, (gint) child_width - (gint) child->padding * 2);
					child_allocation.x = x + child->padding;
				}else {
					GtkRequisition child_requisition;

					gtk_widget_get_child_requisition (child->widget, &child_requisition);
					child_allocation.width = child_requisition.width;
					child_allocation.x = x + (child_width - child_allocation.width) / 2;
				}

				if (direction == GTK_TEXT_DIR_RTL)
					child_allocation.x =
						allocation->x + allocation->width - (child_allocation.x - allocation->x) - child_allocation.width;

				gtk_widget_size_allocate (child->widget, &child_allocation);

				x += child_width + box->spacing;
			}
		}

		x = allocation->x + allocation->width - GTK_CONTAINER (box)->border_width;

		children = box->children;
		while (children) {
			child = children->data;
			children = children->next;

			if ((child->pack == GTK_PACK_END) && GTK_WIDGET_VISIBLE (child->widget)) {
				GtkRequisition child_requisition;
				gtk_widget_get_child_requisition (child->widget, &child_requisition);

				if (box->homogeneous) {
					child_width = childsize;
					child_height = childsize;

					nvis_children -= 1;
					width -= extra;
				}
				else {
					child_width = child_requisition.width + child->padding * 2;

					if (child->expand) {
						if (nexpand_children == 1)
							child_width += width;
						else
							child_width += extra;

						nexpand_children -= 1;
						width -= extra;
					}
				}

				if (child->fill) {
					child_allocation.width = MAX (1, (gint) child_width - (gint) child->padding * 2);
					child_allocation.x = x + child->padding - child_width;
				}
				else {
					child_allocation.width = child_requisition.width;
					child_allocation.x = x + (child_width - child_allocation.width) / 2 - child_width;
				}

				if (direction == GTK_TEXT_DIR_RTL)
					child_allocation.x =
						allocation->x + allocation->width - (child_allocation.x - allocation->x) - child_allocation.width;

				gtk_widget_size_allocate (child->widget, &child_allocation);

				x -= (child_width + box->spacing);
			}
		}
	}
}
