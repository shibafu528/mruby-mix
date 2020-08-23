export MRUBY_CONFIG := $(CURDIR)/build_config.rb

all :
	$(MAKE) -C mruby
.PHONY : all

test : all
	$(MAKE) -C mruby test
.PHONY : test

clean :
	$(MAKE) -C mruby clean
.PHONY : clean
