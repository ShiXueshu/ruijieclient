
AM_CFLAGS = -I $(XML_INCLUDE)

bin_PROGRAMS = ruijieclient
ruijieclient_SOURCES = codeconv.h myerr.c ruijieclient.h  \
		sendpacket.h myerr.h codeconv.c ruijieclient.c \
		 conn_monitor.c conn_monitor.h prase.c prase.h config.c 

ruijieclient_DEPENDENCIES = $(top_builddir)/packet/libruijie.a

ruijieclient_LDADD = $(EXT_LIBS) -lruijie -L$(top_builddir)/packet
		 
