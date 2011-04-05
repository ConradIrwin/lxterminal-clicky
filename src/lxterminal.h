/**
 *      Copyright 2008 Fred Chien <cfsghost@gmail.com>
 *      Copyright (c) 2010 LxDE Developers, see the file AUTHORS for details.
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *      MA 02110-1301, USA.
 */

#ifndef LXTERMINAL_H
#define LXTERMINAL_H

#include "setting.h"

#define NEW_WINDOW_ACCEL "<CTRL><SHIFT>N"
#define NEW_TAB_ACCEL "<CTRL><SHIFT>T"
#define CLOSE_TAB_ACCEL "<CTRL><SHIFT>W"
#define QUIT_ACCEL "<CTRL><SHIFT>Q"
#define COPY_ACCEL "<CTRL><SHIFT>C"
#define PASTE_ACCEL "<CTRL><SHIFT>V"
#define NAME_TAB_ACCEL "<CTRL><SHIFT>I"
#define PREVIOUS_TAB_ACCEL "<CTRL>Page_Up"
#define NEXT_TAB_ACCEL "<CTRL>Page_Down"
#define MOVE_TAB_LEFT_ACCEL "<CTRL><SHIFT>Page_Up"
#define MOVE_TAB_RIGHT_ACCEL "<CTRL><SHIFT>Page_Down"

/* steal from tilda-0.09.6/src/tilda_terminal.c:36 */
#define DINGUS1 "(\\ |[^\n\r\t ]+)"
#define DINGUS2 "(((news|telnet|nttp|file|http|ftp|https)://)|(www|ftp)[-A-Za-z0-9]*\\.)[-A-Za-z0-9\\.]+(:[0-9]*)?/[-A-Za-z0-9_\\$\\.\\+\\!\\*\\(\\),;:@&=\\?/~\\#\\%]*[^]'\\.}>\\) ,\\\"]"

/* Top level application context. */
typedef struct _lxtermwindow {
    Setting * setting;				/* Pointer to current user preferences */
    GPtrArray * windows;			/* Array of pointers to LXTerminal structures */
} LXTermWindow;

/* Representative of a toplevel window. */
typedef struct _lxterminal {
    LXTermWindow * parent;			/* Back pointer to top level context */
    gint index;					/* Index of this element in parent->windows */
    GtkWidget * window;				/* Toplevel window */
    GtkWidget * box;				/* Vertical box, child of toplevel window */
    GtkWidget * menu;				/* Menu bar, child of vertical box */
    GtkActionGroup *action_group;   /* Action group on this window */
    GtkAccelGroup * accel_group;		/* Accelerator group for accelerators on this window */
    GtkWidget * notebook;			/* Notebook, child of vertical box */
    GPtrArray * terms;				/* Array of pointers to Term structures */
    Setting * setting;				/* A copy of parent->setting */
    GdkGeometry geometry;			/* Geometry hints (see XGetWMNormalHints) */
    GdkWindowHints geometry_mask;		/* Mask of valid data in geometry hints */
    gboolean fixed_size;			/* True if the terminal is fixed size */
    gboolean rgba;				/* True if colormap is RGBA */
    GdkColor background;			/* User preference background color converted to GdkColor */
    GdkColor foreground;			/* User preference foreground color converted to GdkColor */
    gint tab_position;				/* Tab position as an integer value */
} LXTerminal;

/* Representative of a tab within a toplevel window. */
typedef struct _term {
    LXTerminal * parent;			/* Back pointer to LXTerminal */
    gint index;					/* Index of this element in parent->terms */
    GtkWidget * tab;				/* Toplevel of the tab */
    GtkWidget * label;				/* Label of the tab, child of the toplevel */
    gboolean user_specified_label;		/* User did "Name Tab", so we will never overwrite this with the window title */
    GtkWidget * close_button;			/* Close button for the tab, child of the toplevel */
    GtkWidget * box;				/* Horizontal box, child of notebook */
    GtkWidget * vte;				/* VteTerminal, child of horizontal box */
    GtkWidget * scrollbar;			/* Scroll bar, child of horizontal box */
    pid_t pid;					/* Process ID of the process that has this as its terminal */
    GClosure * closure;				/* Accelerator structure */
} Term;

/* Output of lxterminal_process_arguments. */
typedef struct _command_arguments {
    char * executable;				/* Value of argv[0]; points into argument vector */
    gchar * command;				/* Value of -e, --command; memory allocated by glib */
    int geometry_columns;			/* Value of --geometry */
    int geometry_rows;
    char * title;				/* Value of -t, -T, --title; points into argument vector */
    char * working_directory;			/* Value of --working-directory; points into argument vector */
} CommandArguments;

extern gboolean lxterminal_process_arguments(gint argc, gchar * * argv, CommandArguments * arguments);
extern LXTerminal * lxterminal_initialize(LXTermWindow * lxtermwin, CommandArguments * arguments, Setting * setting);
extern void terminal_settings_apply_to_all(LXTerminal * terminal);

#endif
