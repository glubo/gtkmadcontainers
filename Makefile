
CC = gcc
BUILDDIR = build

CFLAGS = -Wall			 	\
	-DG_DISABLE_DEPRECATED 	 	\
	-DGDK_DISABLE_DEPRECATED 	\
	-DGDK_PIXBUF_DISABLE_DEPRECATED \
	-DGTK_DISABLE_DEPRECATED

test: test.c $(BUILDDIR)/gtkmadcirclebox.o gtkmadcirclebox.h
	$(CC) test.c $(BUILDDIR)/gtkmadcirclebox.o -o $(BUILDDIR)/test $(CFLAGS) `pkg-config gtk+-2.0 --cflags --libs`

$(BUILDDIR)/gtkmadcirclebox.o: gtkmadcirclebox.c gtkmadcirclebox.h
	$(CC) gtkmadcirclebox.c -c -o $(BUILDDIR)/gtkmadcirclebox.o $(CFLAGS) `pkg-config gtk+-2.0 --cflags `

clean: 
	rm -f $(BUILDDIR)/*.o $(BUILDDIR)/test
