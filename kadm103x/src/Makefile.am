INCLUDES= $(all_includes)
METASOURCES = AUTO

lib_LTLIBRARIES = libkadm103x.la

libkadm103x_la_SOURCES = kadm103x.cpp
libkadm103x_la_LDFLAGS = -module -avoid-version $(all_libraries)
libkadm103x_la_LIBADD = $(LIB_KDEUI) 

kadm103x_DATA = kadm103x.desktop
kadm103xdir = $(kde_datadir)/kicker/applets

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kadm103x.pot
