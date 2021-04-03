
#-------------------------------------------------------------------------------
#
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
#	    = set the variable
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
#
#-------------------------------------------------------------------------------


TOPTARGETS = build clean               # only goals are to do build or clean

SUBDIRS := $(wildcard */.)             # create list of all subdirectories as targets

#-------------------------------------------------------------------------------

# explicitly declare these target to be phony
.PHONY: $(TOPTARGETS) $(SUBDIRS)

all: build

$(TOPTARGETS): $(SUBDIRS)              # targets are subdirectories

$(SUBDIRS):                            # pass your goal to subdirectory makefile
	$(MAKE) -C $@ $(MAKECMDGOALS)
	#$(MAKE) -C $@ build
