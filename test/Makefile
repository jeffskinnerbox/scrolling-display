
#-------------------------------------------------------------------------------
# Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
# Version:      1.0.0
#
# USAGE
#	To build the executable and flash the device
#		make
#		make all
#
#	To build the executable
#	    make build
#
#	To flash the device
#	    make upload
#
#	makefile syntax
#	    = set the varialbe
#	    ?= set the variable only if it's not set/doesn't have a value
#	    := set variable as the output an executed shell script
#	    += is used for appending more text to variables
#
#   make targets
#       .PHONY is for targets which you want to be built every time the makefile runs,
#       regardless of whether it exists or not or is out of date.
#
#  SOURCES
#   	The Makefile was created with the help of this website:
#   	https://stackoverflow.com/questions/17834582/run-make-in-each-subdirectory
#
#  DOCUMENTATION
#       https://www.gnu.org/software/make/manual/html_node/Recursion.html
#       https://www.gnu.org/software/make/manual/make.html#Empty-Recipes
#-------------------------------------------------------------------------------


TOPTARGETS := all clean

SUBDIRS := $(wildcard */.)

#-------------------------------------------------------------------------------

$(TOPTARGETS): $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TOPTARGETS) $(SUBDIRS)
