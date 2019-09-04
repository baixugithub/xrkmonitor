# Linux makefile for $(TARGET)
# 编译完成后生成的文件在 Dist 目录下

# This file can be generated by ./gensrclist.sh
include Makefile.srcs
include ../../make_env

# General configuration variables:
INCDIR = $(MTLIB_INCLUDE_PATH)/mtreport_api
LIBDIR = $(MTLIB_LIB_PATH)

TARGET  = mtreport_api
LIBRARIES = 

# Converts cr/lf to just lf
DOS2UNIX = dos2unix

MODULES = $(SRCS:.c=.o)
MODULES := $(MODULES:.cpp=.o)

CFLAGS = $(CFLAGS_LIB)
CXXFLAGS = $(CXXFLAGS_LIB)

CFLAGS += $(INCLUDE)
CXXFLAGS += $(INCLUDE)

STATICLIB = lib$(TARGET).a
SHAREDLIB = lib$(TARGET)-$(VER_MAJOR).$(VER_MINOR).so
LIBNAME	= lib$(TARGET).so
VERLIBNAME = $(LIBNAME).$(VER_MAJOR)

default: all

all: dist

dist: $(TARGET)

dos2unix:
	@$(DOS2UNIX) $(SRCS) $(INCLS)

$(TARGET): $(SHAREDLIB) $(STATICLIB)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(STATICLIB): $(MODULES)
	$(AR) r $@ $(MODULES)

$(SHAREDLIB): $(MODULES)
	$(CC) -s -shared -Wl,-soname,$(VERLIBNAME) $(LDFLAGS) -o $@ $(MODULES) $(LIBRARIES)

install:
	install -d $(INCDIR) $(LIBDIR)
	install -m 644 $(INSTALL_INC) $(INCDIR)
	install -m 644 $(STATICLIB) $(LIBDIR)
	install -m 755 $(SHAREDLIB) $(LIBDIR)
	ln -sf $(SHAREDLIB) $(LIBDIR)/$(VERLIBNAME)
	ln -sf $(VERLIBNAME) $(LIBDIR)/$(LIBNAME)	
	cp libmtreport_api-1.1.0.so mtagent_api_lib
	cp libmtreport_api.a mtagent_api_lib
	cp mt_report.h mtagent_api_lib

clean:
	rm -f  $(MODULES) $(STATICLIB) $(SHAREDLIB) $(LIBNAME)

