##########################################
 ########################################
 ###   NE PAS MODIFIER CE FICHIER...  ###
 ########################################
##########################################



## Compilateur
CC=gcc



## Options du compilateur et du linker ##
CFLAGS=-O2 -Wall -Ilib
LIBS=-lm
obj_files= toolbox.o



## Options du programme compilé ##
EXECUTABLE=demineur
SOURCES=$(EXECUTABLE).c
##################################

## Options dépendant du système d'exploitation
ifeq ($(OS),Windows_NT)
  EXEEXT := .exe
  DLLEXT := .dll
else
  EXEEXT := 
  DLLEXT := .so
endif
#########################################



## Verification presence du module Graphique ##########
ifneq ($(wildcard graphics/graphics.h),)
	CFLAGS += `sdl2-config --cflags` -DWITH_GRAPHICS
	LIBS    = `sdl2-config --libs` -Llib -lm -lSDL2_ttf
	obj_files += graphics.o
endif
#######################################################



## Verification presence du module Audio ##
ifneq ($(wildcard audio/audio.h),)
	CFLAGS += -DWITH_AUDIO
	LIBS   += -lSDL2_mixer
	obj_files += audio.o
endif
###########################################



## Verification presence du module Reseau ##
ifneq ($(wildcard net/net.h),)
	CFLAGS += -DWITH_NET
	LIBS += -lSDL2_net
	obj_files += net.o
endif
############################################

all: $(SOURCES) $(EXECUTABLE)$(EXEEXT)


# Compilation de l'environnement SDL ############
toolbox.o: toolbox.c toolbox.h
	@echo " * Compilation de Toolbox v2.01"
	@$(CC) $(CFLAGS) $(LIBS) -c toolbox.c -o $@

graphics.o: graphics/graphics.c graphics/graphics.h
	@echo " * Compilation du module graphique"
	@$(CC) $(CFLAGS) $(LIBS) -c graphics/graphics.c -o $@

audio.o: audio/audio.c audio/audio.h
	@echo " * Compilation du module audio"
	@$(CC) $(CFLAGS) $(LIBS) -c audio/audio.c -o $@

net.o: net/net.c net/net.h
	@echo " * Compilation du module reseau"
	@$(CC) $(CFLAGS) $(LIBS) -c net/net.c -o $@
#################################################



## Cible generique
%: %.c $(obj_files)
#	@echo "CFLAGS = $(CFLAGS)"
#	@echo "LIBS = $(LIBS)"
#	@echo "Needed Files = $(obj_files)"
	@echo " * Compilation du programme '$@'"
	$(CC) $(CFLAGS) $(obj_files) $@.c -o $@ $(LIBS)
	@echo " "



run: $(ARGS)
	@echo " * Execution du programme '$@'"
	@./$(ARGS)



## Effacer les core dumps et les anciennes versions
clean:
	@echo " * Purge des executables (*.exe) et fichiers binaires (*.o)"
	@rm -f *.o
	@rm -f *core
	@rm -f *.dll *.so
	@rm -f $(EXECUTABLE)$(EXEEXT)
	@echo " "
