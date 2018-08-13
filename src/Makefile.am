##
# MoboTV
# src/Makefile.am
#

bin_PROGRAMS    = mobotv

mobotv_SOURCES  = main.c		 					\
				  application.c application.h		\
				  ui.c ui.h							\
				  conf_win.c conf_win.h				\
				  about_win.c about_win.h			\
				  chmg_win.c chmg_win.h				\
				  video_ctrl.c video_ctrl.h			\
				  video_player.c video_player.h		\
				  user_conf.c user_conf.h			\
				  channel.c channel.h				\
				  ch_info.c ch_info.h				\
				  ch_list.c ch_list.h				\
				  ch_view.c ch_view.h				\
				  input_dialog.c input_dialog.h     \
				  conffile.c conffile.h

mobotv_LDADD    = $(GTK_LIBS) $(LIBXML2_LIBS) $(GST_LIBS)

mobotv_CPPFLAGS = -Wall $(GTK_CFLAGS) $(LIBXML2_CFLAGS) $(GST_CFLAGS) \
				  -DPIXMAPS_DIR=\""$(datadir)/pixmaps"\"

