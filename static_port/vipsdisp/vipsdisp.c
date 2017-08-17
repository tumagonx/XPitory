/* Tiny display-an-image demo program. 
 *
 * This is not supposed to be a complete image viewer, it's just to 
 * show how to display a VIPS image (or the result of a VIPS computation) in a
 * window.
 *
 * Compile with:

 gcc -g -Wall vipsdisp.c `pkg-config vips gtk+-3.0 --cflags --libs` -o vipsdisp

 */

#include <stdio.h>

#include <gtk/gtk.h>
#include <vips/vips.h>
#include <windows.h>
#include <limits.h>
#include <shellapi.h>

int export_file = 0;
/* Just to demo progress feedback. This should be used to update a widget
 * somewhere.
 */
static void
vipsdisp_preeval( VipsImage *image, 
	VipsProgress *progress, const char *filename )
{
	printf( "load starting for %s ...\n", filename );
}

static void
vipsdisp_eval( VipsImage *image, 
	VipsProgress *progress, const char *filename )
{
	static int previous_precent = -1;

	if( progress->percent != previous_precent ) {
		printf( "%s %s: %d%% complete\r", 
			g_get_prgname(), filename, progress->percent );
		previous_precent = progress->percent;
	}
}

static void
vipsdisp_posteval( VipsImage *image, 
	VipsProgress *progress, const char *filename )
{
	printf( "\nload done in %g seconds\n", 
		g_timer_elapsed( progress->start, NULL ) );
}

static VipsImage *
vipsdisp_load( const char *filename )
{
	VipsImage *image;

	if( !(image = vips_image_new_from_file( filename, NULL )))
	return NULL;

	/* Attach an eval callback: this will tick down if we open this image
	 * via a temp file.
	 */
	vips_image_set_progress( image, TRUE ); 
	g_signal_connect( image, "preeval",
		G_CALLBACK( vipsdisp_preeval ), (void *) filename);
	g_signal_connect( image, "eval",
		G_CALLBACK( vipsdisp_eval ), (void *) filename);
	g_signal_connect( image, "posteval",
		G_CALLBACK( vipsdisp_posteval ), (void *) filename);

	return image;
}

typedef struct _Update {
	GtkWidget *drawing_area;
	VipsRect rect;
} Update;

/* The main GUI thread runs this when it's idle and there are tiles that need
 * painting. 
 */
static gboolean
render_cb( Update *update )
{
  gtk_widget_queue_draw_area( update->drawing_area,
			      update->rect.left, update->rect.top,
			      update->rect.width, update->rect.height );

  g_free( update );

  return( FALSE );
}

/* Come here from the vips_sink_screen() background thread when a tile has been
 * calculated. We can't paint the screen directly since the main GUI thread
 * might be doing something. Instead, we add an idle callback which will be
 * run by the main GUI thread when it next hits the mainloop.
 */
static void
render_notify( VipsImage *image, VipsRect *rect, void *client )
{
	GtkWidget *drawing_area = GTK_WIDGET( client );
	Update *update = g_new( Update, 1 );

	update->rect = *rect;
	update->drawing_area = drawing_area;

	g_idle_add( (GSourceFunc) render_cb, update );
}

/* Make the image for display from the raw disc image. Could do
 * anything here, really. Uncomment sections to try different effects. Convert
 * to 8-bit RGB would be a good idea.
 */

static VipsImage *
vipsdisp_display_image( VipsImage *in, GtkWidget *drawing_area, gint width, gint height, char *outimg )
{
	VipsImage *image;
	VipsImage *t[1];
	VipsImage *x;
	gdouble hscale = 1;
	char fn_buf[VIPS_PATH_MAX];

	/* Edit these to add or remove things from the pipeline we build. These
	 * should be wired up to something in a GUI.
	 */
	

	/* image represents the head of the pipeline. Hold a ref to it as we
	 * work.
	 */
	image = in;
	printf ("uwaaaaaahhhhh\n");
	g_object_ref( image ); 
	if( in->Type == VIPS_INTERPRETATION_CMYK &&
		in->Coding == VIPS_CODING_NONE &&
		(in->BandFmt == VIPS_FORMAT_UCHAR ||
		 in->BandFmt == VIPS_FORMAT_USHORT) &&
		vips_image_get_typeof( in, VIPS_META_ICC_NAME) ) {
		if( vips_icc_import( image, &t[0], 
			"embedded", TRUE,
			"pcs", VIPS_PCS_XYZ,
			NULL ) )
			return( NULL );
		g_object_unref( image );
		image = t[0];
    }
#ifdef MANGA

	if (in->Ysize > height) 
		hscale = (gdouble)height/(gdouble)in->Ysize; //fit screen height
	if( vips_resize( image, &x, hscale, "kernel", VIPS_KERNEL_CUBIC, "centre", TRUE, NULL ) ) {
		g_object_unref( image );
		return( NULL ); 
	}
	g_object_unref( image );
	image = x;
#endif

	/* This won't work for CMYK, you need to mess about with ICC profiles
	 * for that, but it will do everything else.
	 */
	if( vips_colourspace( image, &x, VIPS_INTERPRETATION_sRGB, NULL ) ) {
		g_object_unref( image );
		return( NULL ); 
	}
	g_object_unref( image );
	image = x;
	/* Drop any alpha.
	 */
	if( vips_extract_band( image, &x, 0, "n", 3, NULL ) ) {
		g_object_unref( image );
		return( NULL ); 
	}
	g_object_unref( image );
	image = x;
	if (export_file)
	{
		strcpy(fn_buf, outimg);
#ifdef DEEPZOOM
		if (vips_dzsave(image, outimg, "container", VIPS_FOREIGN_DZ_CONTAINER_FS, NULL) == 0)
			vips_info ("written deepzoom: %s\n", outimg);
#endif
		if (vips_tiffsave(image, strcat(outimg,".tiff"), "compression", VIPS_FOREIGN_TIFF_COMPRESSION_JPEG, "bigtiff", TRUE, "tile", TRUE, "tile_width", 512, "tile-height", 512, NULL) == 0)
			vips_info ("written tiled tiff: %s\n", outimg);
		if (in->Ysize > 256) 
			hscale = 256.00/(gdouble)in->Ysize;
		if( vips_resize( image, &x, hscale, "centre", TRUE, NULL ) ) {
			g_object_unref( image );
			return( NULL ); 
		}
		g_object_unref( image );
		image = x;
		if (vips_pngsave(image, strcat(fn_buf,".png"), NULL) == 0)
			vips_info ("written png thumbnail: %s\n", fn_buf);
		g_object_unref( image );
		g_object_unref( x );
		return(NULL);
	}
	x = vips_image_new();
	if( vips_sink_screen( image, x, NULL, 256, 256, 400, 0, 
		render_notify, drawing_area ) ) {
		g_object_unref( image );
		g_object_unref( x );
		return( NULL );
	}
	g_object_unref( image );
	image = x;

	return( image );
}

static void
vipsdisp_draw_rect( GtkWidget *drawing_area, 
	cairo_t *cr, VipsRegion *region, VipsRect *expose )
{
	VipsRect image;
	VipsRect clip;
	unsigned char *cairo_buffer;
	int x, y;
	cairo_surface_t *surface;

	printf( "vipsdisp_draw_rect: "
		"left = %d, top = %d, width = %d, height = %d\n%d\n",
		expose->left, expose->top,
		expose->width, expose->height,region->im->Xsize );

	/* Clip against the image size ... we don't want to try painting 
	 * outside the image area.
	 */
	image.left = 0;
	image.top = 0;
	image.width = region->im->Xsize;
	image.height = region->im->Ysize;
	vips_rect_intersectrect( &image, expose, &clip );
	if( vips_rect_isempty( &clip ) ||
		vips_region_prepare( region, &clip ) )
		return;

	/* libvips is RGB, cairo is ARGB, we have to repack the data.
	 */
	cairo_buffer = g_malloc( clip.width * clip.height * 4 );

	for( y = 0; y < clip.height; y++ ) {
		VipsPel *p = 
			VIPS_REGION_ADDR( region, clip.left, clip.top + y );
		unsigned char *q = cairo_buffer + clip.width * 4 * y;

		for( x = 0; x < clip.width; x++ ) {
			q[0] = p[2];
			q[1] = p[1];
			q[2] = p[0];
			q[3] = 0;

			p += 3;
			q += 4;
		}
	}

	surface = cairo_image_surface_create_for_data( cairo_buffer, 
		CAIRO_FORMAT_RGB24, clip.width, clip.height, clip.width * 4 );

	cairo_set_source_surface( cr, surface, clip.left, clip.top );

	cairo_paint( cr );

	g_free( cairo_buffer ); 

	cairo_surface_destroy( surface ); 
}

static void
vipsdisp_draw( GtkWidget *drawing_area, cairo_t *cr, VipsRegion *region )
{
	cairo_rectangle_list_t *rectangle_list = 
		cairo_copy_clip_rectangle_list( cr );

	printf( "vipsdisp_draw:\n" ); 

	if( rectangle_list->status == CAIRO_STATUS_SUCCESS ) { 
		int i;

		for( i = 0; i < rectangle_list->num_rectangles; i++ ) {
			VipsRect expose;

			expose.left = rectangle_list->rectangles[i].x;
			expose.top = rectangle_list->rectangles[i].y;
			expose.width = rectangle_list->rectangles[i].width;
			expose.height = rectangle_list->rectangles[i].height;

			vipsdisp_draw_rect( drawing_area, cr, region, &expose );
		}
	}

	cairo_rectangle_list_destroy( rectangle_list );
}

int
main( int argc, char **argv )
{
	VipsImage *image;
	VipsImage *display;
	VipsRegion *region;

	GtkWidget *window;
	GtkWidget *scrolled_window;
	GtkWidget *drawing_area;
	GtkAdjustment *hadjustment;
	gdouble scrollend;
	gint width;
	gint height;
	gchar *img;
	gchar *outimg;
	gchar tmpimg[VIPS_PATH_MAX];
	gchar imgDrive[_MAX_DRIVE];
	gchar imgDir[_MAX_DIR * 3];
	gchar imgFName[_MAX_FNAME + 5];
	gchar imgExt[_MAX_EXT];
	wchar_t** wargv = CommandLineToArgvW( GetCommandLineW(), &argc);


	if( VIPS_INIT( argv[0] ) )
		vips_error_exit( "unable to start VIPS" );

	gtk_init( &argc, &argv );

	if( argc < 2 )
		vips_error_exit( "usage: %s <filename>", argv[0] );

	img = g_utf16_to_utf8(wargv[1], -1, NULL, NULL, NULL);
	_splitpath(img, imgDrive, imgDir, imgFName, imgExt);
#ifndef NODCR
	const gchar *tmp = g_getenv ("TEMP");
	strcpy (tmpimg, tmp);
	strcat (tmpimg, "\\vipsimage.tmp");
	if (g_file_test (tmpimg, G_FILE_TEST_EXISTS))
		g_unlink (tmpimg);

	extern int dcmain (int, const char **);
	if (!stricmp(imgExt, ".arw") || !stricmp(imgExt, ".raw") || 
		!stricmp(imgExt, ".mos") || !stricmp(imgExt, ".crw") || 
		!stricmp(imgExt, ".cr2") || !stricmp(imgExt, ".mrw") || 
		!stricmp(imgExt, ".erf") || !stricmp(imgExt, ".raf") || 
		!stricmp(imgExt, ".3fr") || !stricmp(imgExt, ".kdc") || 
		!stricmp(imgExt, ".mdc") || !stricmp(imgExt, ".mef") || 
		!stricmp(imgExt, ".nrw") || !stricmp(imgExt, ".nef") || 
		!stricmp(imgExt, ".orf") || !stricmp(imgExt, ".rw2") || 
		!stricmp(imgExt, ".pef") || !stricmp(imgExt, ".x3f") || 
		!stricmp(imgExt, ".srw") || !stricmp(imgExt, ".sr2")) 
	{
		const char *dcfakeargv[6] = {argv[0], "-o", "1", "-O", tmpimg, img};
		//dcraw patched to output filename
		dcmain (6, dcfakeargv);
		if (!g_file_test (tmpimg, G_FILE_TEST_EXISTS))
			vips_error_exit( "unable to load %s", img );
		if( !(image = vipsdisp_load( tmpimg )) )
			vips_error_exit( "unable to load %s", tmpimg );
	}
	else 
#endif
	if (stricmp(imgExt, ".tiff") && stricmp(imgExt, ".tif") && 
		stricmp(imgExt, ".png") && stricmp(imgExt, ".jpg") && 
		stricmp(imgExt, ".jpeg") 
#ifndef NODCR
		&& stricmp(imgExt, ".ppm")
#endif
		)
		vips_error_exit( "unsupported image");
	else if( !(image = vipsdisp_load( img )) )
		vips_error_exit( "unable to load %s\n", img );
	if( argc == 3 ) {
		outimg = g_utf16_to_utf8(wargv[2], -1, NULL, NULL, NULL);
		if (!g_file_test (outimg, G_FILE_TEST_EXISTS))
			export_file = 1;
	}
	window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
	gtk_window_set_title (GTK_WINDOW (window), g_path_get_basename(img));
#ifdef MANGA
	width = gdk_screen_get_width(gtk_window_get_screen (GTK_WINDOW(window)));
	height = gdk_screen_get_height(gtk_window_get_screen (GTK_WINDOW(window)));
#else
	width = gdk_screen_get_width(gtk_window_get_screen (GTK_WINDOW(window))) - 56;
	height = gdk_screen_get_height(gtk_window_get_screen (GTK_WINDOW(window))) - 64;
#endif
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
	g_signal_connect (window, "delete_event", G_CALLBACK (gtk_main_quit), NULL);

	scrolled_window = gtk_scrolled_window_new( NULL, NULL );
	gtk_container_add( GTK_CONTAINER( window ), scrolled_window );

	drawing_area = gtk_drawing_area_new();
	if( !(display = vipsdisp_display_image( image, drawing_area, width, height, outimg )) ||
		!(region = vips_region_new( display )) )
		vips_error_exit( "unable to build display image" );
	g_signal_connect( drawing_area, "draw", 
		G_CALLBACK( vipsdisp_draw ), region );
	gtk_widget_set_size_request( drawing_area, 
		display->Xsize, display->Ysize );
#if GTK_CHECK_VERSION(3,10,0)
	gtk_container_add( GTK_CONTAINER( scrolled_window ), drawing_area );
#else
	gtk_scrolled_window_add_with_viewport( GTK_SCROLLED_WINDOW( scrolled_window ), drawing_area );
#endif
	if (display->Xsize < width)
		width = display->Xsize + 24;
	if (display->Ysize < height)
		height = display->Ysize + 24;
	gtk_window_set_default_size( GTK_WINDOW( window ), width, height);
	gtk_widget_show_all( window );
#ifdef MANGA
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW (scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_NEVER);
	gtk_window_fullscreen( GTK_WINDOW( window ));
	hadjustment = gtk_scrolled_window_get_hadjustment(GTK_SCROLLED_WINDOW (scrolled_window));
	scrollend = gtk_adjustment_get_upper(hadjustment);
	gtk_adjustment_set_value(hadjustment, scrollend);
	gtk_scrolled_window_set_hadjustment(GTK_SCROLLED_WINDOW (scrolled_window), hadjustment);
#endif
	gtk_main();

	g_object_unref( region ); 
	g_object_unref( display ); 
	g_object_unref( image ); 
#ifndef NODCR
	vips_shutdown();  //vips plug this atexit(), need it earlier to do cleanup
	if (g_file_test (tmpimg, G_FILE_TEST_EXISTS))
		g_unlink (tmpimg);
#endif
	return( 0 );
}
