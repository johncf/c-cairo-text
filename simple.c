#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cairo.h>
#include <cairo-ft.h>

#define FONT_SIZE 36
#define MARGIN (FONT_SIZE * .5)

int
main(int argc, char **argv)
{
    const char *fontfile;
    const char *text;

    if (argc < 3)
    {
        fprintf (stderr, "usage: hello-harfbuzz font-file.ttf text\n");
        exit (1);
    }

    fontfile = argv[1];
    text = argv[2];

    /* Initialize FreeType and create FreeType font face. */
    FT_Library ft_library;
    FT_Face ft_face;
    FT_Error ft_error;

    if ((ft_error = FT_Init_FreeType (&ft_library)))
        abort();
    if ((ft_error = FT_New_Face (ft_library, fontfile, 0, &ft_face)))
        abort();
    if ((ft_error = FT_Set_Char_Size (ft_face, FONT_SIZE*64, FONT_SIZE*64, 0, 0)))
        abort();

    /* Draw, using cairo. */
    /* Set up cairo surface. */
    cairo_surface_t *cairo_surface;
    cairo_surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 128, 72);
    cairo_t *cr;
    cr = cairo_create (cairo_surface);
    cairo_set_source_rgba (cr, 1., 1., 1., 1.);
    cairo_paint (cr);
    cairo_set_source_rgba (cr, 0., 0., 0., 1.);
    cairo_translate (cr, MARGIN, MARGIN);

    /* Set up cairo font face. */
    cairo_font_face_t *cairo_face;
    cairo_face = cairo_ft_font_face_create_for_ft_face (ft_face, 0);
    cairo_set_font_face (cr, cairo_face);
    cairo_set_font_size (cr, FONT_SIZE);

    cairo_scaled_font_t* scaled_face = cairo_get_scaled_font(cr);

    // get glyphs for the text
    cairo_glyph_t* glyphs = NULL;
    int glyph_count;

    cairo_status_t stat = cairo_scaled_font_text_to_glyphs(
            scaled_face, 0, 0,
            text, strlen(text),
            &glyphs, &glyph_count,
            NULL, NULL, NULL);

    if (stat == CAIRO_STATUS_SUCCESS) {
        cairo_translate(cr, 0, FONT_SIZE);

        cairo_show_glyphs (cr, glyphs, glyph_count);
        cairo_glyph_free (glyphs);

        cairo_surface_write_to_png (cairo_surface, "out-s.png");
    }

    cairo_font_face_destroy (cairo_face);
    cairo_destroy (cr);
    cairo_surface_destroy (cairo_surface);

    FT_Done_Face (ft_face);
    FT_Done_FreeType (ft_library);

    return 0;
}
