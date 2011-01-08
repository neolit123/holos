// http://ricardo.ecn.wfu.edu/~cottrell/gtk_win32/
// slightly modified by ccernn

// MS native code and GTK

// GTK (www.gtk.org) is an excellent free, open-source GUI toolkit.
// With Tor Lillqvist's win32 port, it provides a fine cross-platform solution.
// I regularly write GTK-based code under Linux, and as well as compiling it
// for that platform I am able to cross-compile it for win32
// (see my cross-gtk page).

// While you can get perfectly acceptable results using essentially the same
// source code for the Linux/unix and win32 versions of your program, it's
// possible to tweak the win32 version a little so that it conforms better to
// what Windows users expect, by using the win32 API directly for some
// functions (bracketed off by #ifdef). A couple of examples of this are shown
// below:

// 1. Using "native" Microsoft file selection dialogs in place of gtkfilesel.
// 2. Using the default windows font for menus and so on.

// MS native file selection dialogs

// There follows a snippet of code showing how one can set up a file selection
// function with a common prototype, implemented in two versions, one for X11
// and one for win32. Some of the material is specific to the program of mine
// from which the code is extracted, but it should give you some idea of how to
// proceed. For further info, see the MS documentation for the win32 API.
// For instance, a google search on GetOpenFileName will quickly take you to
// the appropriate page on msdn.microsoft.com.

//----------------------------------------------------------------------

/*
 *  Copyright (c) by Allin Cottrell
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

/* fileselect.c for gretl -- use the gtkextra file selector under X11,
   the native MS file selector under MS Windows */

#include "gretl.h"

extern void errbox (const char *msg); /* gretl function */

/* ........................................................... */

#ifdef G_OS_WIN32  /* native MS */

// #include  ???

struct win32_filtermap
{
  int action;
  char *filter;
};

static char *get_filters (int action)
{
  int i;
  char *filter;
  static struct win32_filtermap map[] =
  {
    {SAVE_DATA, "gretl data files (*.dat)\0*.dat\0all files\0*\0"},
    {SAVE_GZDATA, "compressed data files (*.gz)\0*.gz\0all files\0*\0"},
    {SAVE_BIN1, "gretl data files (*.dat)\0*.dat\0all files\0*\0"},
    {SAVE_BIN2, "gretl data files (*.dat)\0*.dat\0all files\0*\0"},
    {SAVE_CMDS, "gretl command files (*.inp)\0*.inp\0all files\0*\0"},
    {SAVE_SCRIPT, "gretl script files (*.inp)\0*.inp\0all files\0*\0"},
    {SAVE_CONSOLE, "gretl command files (*.inp)\0*.inp\0all files\0*\0"},
    {SAVE_MODEL, "text files (*.txt)\0*.txt\0all files\0*\0"},
    {SAVE_SESSION, "session files (*.gretl)\0*.gretl\0all files\0*\0"},
    {SAVE_LAST_GRAPH, "all files\0*\0"},
    {SAVE_GP_CMDS, "gnuplot files (*.gp)\0*.gp\0all files\0*\0"},
    {EXPORT_CSV, "CSV files (*.csv)\0*.csv\0all files\0*\0"},
    {EXPORT_R, "GNU R files (*.R)\0*.R\0all files\0*\0"},
    {EXPORT_R_ALT, "GNU R files (*.R)\0*.R\0all files\0*\0"},
    {EXPORT_OCTAVE, "GNU Octave files (*.m)\0*.m\0all files\0*\0"},
    {SAVE_OUTPUT, "text files (*.txt)\0*.txt\0all files\0*\0"},
    {SAVE_TEX_TAB, "TeX files (*.tex)\0*.tex\0all files\0*\0"},
    {SAVE_TEX_EQ, "TeX files (*.tex)\0*.tex\0all files\0*\0"},
    {OPEN_DATA, "gretl data files (*.dat)\0*.dat*\0all files\0*\0"},
    {OPEN_SCRIPT, "gretl script files (*.inp)\0*.inp\0all files\0*\0"},
    {OPEN_SESSION, "session files (*.gretl)\0*.gretl\0all files\0*\0"},
    {OPEN_CSV,  "CSV files (*.csv)\0*.csv\0all files\0*\0"},
    {OPEN_BOX, "BOX data files (*.box)\0*.box\0all files\0*\0"}
  };
  for (i=0; i< sizeof map/sizeof *map; i++)
  {
    if (action == map[i].action)
    {
	    filter = map[i].filter;
	    break;
    }
  }
  return filter;
}

/* ........................................................... */

void file_selector (char *msg, char *startdir, int action, gpointer data) 
{
  OPENFILENAME of;
  int retval;
  char fname[MAXLEN];
  fname[0] = '\0';
  /* initialize file dialog info struct */
  memset(&of, 0, sizeof of);
  #ifdef OPENFILENAME_SIZE_VERSION_400
    of.lStructSize = OPENFILENAME_SIZE_VERSION_400;
  #else
    of.lStructSize = sizeof of;
  #endif
  of.hwndOwner = NULL;
  of.lpstrFilter = get_filters(action);
  of.lpstrCustomFilter = NULL;
  of.nFilterIndex = 1;
  of.lpstrFile = fname;
  of.nMaxFile = sizeof fname;
  of.lpstrFileTitle = NULL;
  of.lpstrInitialDir = startdir;
  of.lpstrTitle = msg;
  of.lpstrDefExt = NULL;
  of.Flags = OFN_HIDEREADONLY;
  if (action < END_OPEN) retval = GetOpenFileName(&of);
  else retval = GetSaveFileName(&of); /* a file save action */
  if (!retval)
  {
    if (CommDlgExtendedError()) errbox("File dialog box error");
	  return;
  }
  /* you now have the selected filename in "fname" and can
     do whatever is appropriate in response */
}

#else /* X11, not win32: use gtkextra */

struct extmap
{
  int action;
  char *ext;
};

static char *get_filters (int action)
{
  int i;
  char *s = "*";
  static struct extmap map[] =
  {
    {SAVE_DATA, "*.dat"},
    {SAVE_GZDATA, "*.gz"},
    {SAVE_BIN1, "*.dat"},
    {SAVE_BIN2, "*.dat"},
    {SAVE_CMDS, "*.inp"},
    {SAVE_SCRIPT, "*.inp"},
    {SAVE_CONSOLE, "*.inp"},
    {SAVE_MODEL, "*.txt"},
    {SAVE_SESSION, "*.gretl"},
    {SAVE_GP_CMDS, "*.gp"},
    {EXPORT_CSV, "*.csv"},
    {EXPORT_R, "*.R"},
    {EXPORT_R_ALT, "*.R"},
    {EXPORT_OCTAVE, "*.m"},
    {SAVE_OUTPUT, "*.txt"},
    {SAVE_TEX_TAB, "*.tex"},
    {SAVE_TEX_EQ, "*.tex"},
    {OPEN_DATA, "*.dat*"},
    {OPEN_SCRIPT, "*.inp"},
    {OPEN_SESSION, "*.gretl"},
    {OPEN_CSV,  "*.csv"},
    {OPEN_BOX, "*.box"}
  };
  for (i=0; i < sizeof map / sizeof *map; i++)
  {
	  if (action == map[i].action)
    {
	    s = map[i].ext;
	    break;
	  }
  }
  return s;
}

/* ........................................................... */

static void filesel_callback (GtkWidget *w, gpointer data) 
{
  GtkIconFileSel *fs = GTK_ICON_FILESEL(data);
  gint action = GPOINTER_TO_INT(gtk_object_get_data(GTK_OBJECT(data), "action"));
  char fname[MAXLEN];
  char *test, *path;
  test = gtk_entry_get_text(GTK_ENTRY(fs->file_entry));
  if (test == NULL || *test == '\0') return;    
  path = gtk_file_list_get_path(GTK_FILE_LIST(fs->file_list));
  sprintf(fname, "%s%s", path, test);
  /* do something appropriate with the filename ... */
  gtk_widget_destroy(GTK_WIDGET(fs));
}

/* ........................................................... */

void file_selector (char *msg, char *startdir, int action, gpointer data) 
{
  GtkWidget *filesel;
  filesel = gtk_icon_file_selection_new(msg);
  if (strstr(startdir, "/."))
    gtk_icon_file_selection_show_hidden(GTK_ICON_FILESEL(filesel), TRUE);
    
  gtk_object_set_data(GTK_OBJECT(filesel), "action", GINT_TO_POINTER(action));
   gtk_icon_file_selection_set_filter(GTK_ICON_FILESEL(filesel), get_filters(action));
  gtk_signal_connect(GTK_OBJECT(GTK_ICON_FILESEL(filesel)->ok_button), "clicked", (GtkSignalFunc) filesel_callback, filesel);
  gtk_icon_file_selection_open_dir(GTK_ICON_FILESEL(filesel), startdir);
  gtk_signal_connect_object(GTK_OBJECT(GTK_ICON_FILESEL(filesel)->cancel_button), "clicked", (GtkSignalFunc)gtk_widget_destroy, GTK_OBJECT(filesel));
  gtk_widget_show(filesel);
}

#endif /* end of non-win32 stuff */

// MS default font

// The following code snippet shows how to grab the default Windows font and
// use it as the default font in your GTK program. The first version is for
// GTK 1.2; a version for GTK 2.0 follows.

#ifdef G_OS_WIN32

gchar *default_windows_menu_fontspec (void)
{
  gchar *fontspec = NULL;
  NONCLIENTMETRICS ncm;
  memset(&ncm, 0, sizeof ncm);
  ncm.cbSize = sizeof ncm;
  if (SystemParametersInfo(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0))
  {
    HDC screen = GetDC(0);
    double y_scale = 72.0 / GetDeviceCaps(screen, LOGPIXELSY);
    int pixel_size = (int) (ncm.lfMenuFont.lfHeight * y_scale);
    /* you may need to experiment with the size field */
    if (pixel_size < 0) pixel_size = -pixel_size;
    fontspec = g_strdup_printf("-*-%s-*-*-*-*-%i-*-*-*-p-*-iso8859-1",
               ncm.lfMenuFont.lfFaceName,
				       pixel_size);
    ReleaseDC(0, screen);
  }
  return fontspec;
}

#endif

static GtkWidget *make_main_window (void) 
{
  GtkWidget *mwin;
  #ifdef G_OS_WIN32
    gchar *winfont;
  #endif
  mwin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  #ifdef G_OS_WIN32
    winfont = default_windows_menu_fontspec();
    if (winfont != NULL)
    {
      GtkStyle *style = gtk_widget_get_style(mwin);
      style->font = gdk_font_load(winfont);
      if (style->font)
      {
        gtk_widget_set_style(mwin, style);
      }
      g_free(winfont);
    }
  #endif
  /* continue building main window ... */
}

// End of GTK 1.2 version, now for GTK 2.0. (Ulf Lamping tells me this
// requires gtk 2.2.0 or higher to work properly.)

#ifdef G_OS_WIN32
  static char appfontname[128] = "tahoma 8"; /* fallback value */
#else
  static char appfontname[128] = "Sans 10";
#endif

void set_app_font (const char *fontname)
{
  GtkSettings *settings;
  if (fontname != NULL && *fontname == 0) return;
  settings = gtk_settings_get_default();
  if (fontname == NULL)
  {
    g_object_set(G_OBJECT(settings), "gtk-font-name", appfontname, NULL);
  }
  else
  {
    GtkWidget *w;
    PangoFontDescription *pfd;
    PangoContext *pc;
    PangoFont *pfont;
    w = gtk_label_new(NULL);
    pfd = pango_font_description_from_string(fontname);
    pc = gtk_widget_get_pango_context(w);
    pfont = pango_context_load_font(pc, pfd);
    if (pfont != NULL)
    {
	    strcpy(appfontname, fontname);
	    g_object_set(G_OBJECT(settings), "gtk-font-name", appfontname, NULL);
    }
    gtk_widget_destroy(w);
    pango_font_description_free(pfd);
  }
}

#ifdef G_OS_WIN32

char *default_windows_menu_fontspec (void)
{
  gchar *fontspec = NULL;
  NONCLIENTMETRICS ncm;
  memset(&ncm, 0, sizeof ncm);
  ncm.cbSize = sizeof ncm;
  if (SystemParametersInfo(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0))
  {
    HDC screen = GetDC(0);
    double y_scale = 72.0 / GetDeviceCaps(screen, LOGPIXELSY);
    int point_size = (int) (ncm.lfMenuFont.lfHeight * y_scale);
    if (point_size < 0) point_size = -point_size;
    fontspec = g_strdup_printf("%s %d", ncm.lfMenuFont.lfFaceName,point_size);
    ReleaseDC(0, screen);
  }
  return fontspec;
}

static void try_to_get_windows_font (void)
{
  gchar *fontspec = default_windows_menu_fontspec();
  if (fontspec != NULL)
  {
    int match = 0;
    PangoFontDescription *pfd;
    PangoFont *pfont;
    PangoContext *pc;
    GtkWidget *w;
    pfd = pango_font_description_from_string(fontspec);
    w = gtk_label_new(NULL);
    pc = gtk_widget_get_pango_context(w);
    pfont = pango_context_load_font(pc, pfd);
    match = (pfont != NULL);
    pango_font_description_free(pfd);
    g_object_unref(G_OBJECT(pc));
    gtk_widget_destroy(w);
    if (match) set_app_font(fontspec);
    g_free(fontspec);
  }
}

#endif /* G_OS_WIN32 */

// Allin Cottrell, Wake Forest University
// cottrell@wfu.edu
// Last updated: January, 2004 
