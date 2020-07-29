
# The library must be copied to the MySQL/MariaDB plugin directory.
# Check your Linux distribution for the correct path.
# Confirm the correct path in your Linux distro.
# -- show variables like '%plugin%';
#
PATH_MARIADB	= /usr/lib64/mariadb/plugin
PATH_MYSQL = /usr/lib64/mysql/plugin
DEST = $(PATH_MYSQL)

INCL	= -I/usr/include/mysql
CXX=g++

# When compiling for MariaDB set USING_MARIADB=1, otherwise set
# to USING_MARIADB=0.
CXXFLAGS=-Wall -O2 -fPIC -std=c++11 --shared $(INCL) -DUSING_MARIADB=1 -DHAVE_DLOPEN=1
LDFLAGS=-shared

TARGET = lib/libutilities.so
SRCS =  src/common.cc \
  src/count_if.cc \
	src/sum_if.cc \
	src/avg_if.cc

OBJS = $(%.cc=%.o)

all: $(TARGET)

install: $(TARGET)
	@echo
	@echo	"Copying library."
	cp -f $(TARGET) $(DEST)

clean:
	$(RM) $(OBJS) $(TARGET) a.out

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(SRCS) $(OBJS) -o $(TARGET)
