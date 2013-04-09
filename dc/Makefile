RONIN = /usr/local/ronin

#RELEASE_BUILD = 1

AS = sh-elf-as -little
CC = sh-elf-gcc -ml -m4-single-only
LD = sh-elf-ld -EL

RM = rm -f
MKDIR = mkdir -p

OBJBASE	= ../obj
BINBASE	= ../bin

OBJ = $(OBJBASE)/dc
BIN = $(BINBASE)/dc

BASE	= ..
COMMON	= ../common
Z80C	= ../z80c
Z80H	= ../z80hdc
IO	= ../io
SOUND	= ../sound
VRAM	= ../vram
FDD	= ../fdd
FONT	= ../font
GENERIC	= ../generic
MENU	= ./menu
TARGET	= .
SHASM	= ./sh
PATCH	= ./patch


HEADERS	= $(TARGET)/compiler.h $(BASE)/common.h $(COMMON)/milstr.h 	\
	$(COMMON)/_memory.h 						\
	$(COMMON)/rect.h $(COMMON)/lstarray.h 				\
	$(TARGET)/trace.h

Z80CORES = $(Z80H)/z80core.h $(Z80H)/z80mem.h $(Z80H)/z80dmap.h

PCCORES	= $(BASE)/ievent.h $(BASE)/nevent.h $(BASE)/statsave.h

IOCORES	= $(IO)/iocore.h $(IO)/cmt.h $(IO)/cgrom.h 			\
	$(IO)/crtc.h $(IO)/dipsw.h $(IO)/dmac.h $(IO)/ctc.h 		\
	$(IO)/fdc.h $(IO)/memio.h $(IO)/pcg.h $(IO)/ppi.h 		\
	$(IO)/sio.h $(IO)/sndboard.h $(IO)/subcpu.h $(IO)/vramio.h

FMBOARDS = $(SOUND)/sound.h  						\
	$(SOUND)/opmgen.h $(SOUND)/psggen.h 				\
	$(SOUND)/x1f.h $(SOUND)/sndctrl.h $(SOUND)/sndcsec.h

COPT	= -Wall -O8 -fomit-frame-pointer -fno-exceptions -I$(RONIN)/include -I$(TARGET)			\
	-I$(BASE) -I$(COMMON) -I$(Z80H) -I$(IO) -I$(VRAM) -I$(SOUND) -I$(FDD) -I$(FONT) -I$(GENERIC)	\
	-I$(MENU)

ifdef RELEASE_BUILD
COPT	+= -DNOSERIAL -DNODISP
endif

AOPT	=
LOPT	= -Wl,-Ttext,0x8c010000 -nostartfiles $(RONIN)/lib/crt0.o

Z80COBJ	= $(OBJ)/z80c.o	$(OBJ)/z80c_mn.o $(OBJ)/z80c_sb.o	\
	$(OBJ)/z80c_cb.o $(OBJ)/z80c_ix.o $(OBJ)/z80c_iy.o	\
	$(OBJ)/z80mem.o	$(OBJ)/z80dmapc.o

Z80AOBJ = $(OBJ)/z80h.o $(OBJ)/z80mem.oa $(OBJ)/z80h_cb.oa		\
	$(OBJ)/z80h_i.oa $(OBJ)/z80h_s.oa $(OBJ)/z80dmap.o 		\
	$(OBJ)/z80h_mn.oa						\

DCOBJ	= $(OBJ)/_memory.o $(OBJ)/bmpdata.o $(OBJ)/lstarray.o		\
	$(OBJ)/milstr.o	$(OBJ)/parts.oa	$(OBJ)/profile.o		\
	$(OBJ)/strres.o	$(OBJ)/textfile.o $(OBJ)/wavefile.o		\
	$(Z80AOBJ)							\
	$(OBJ)/ievent.o $(OBJ)/keystat.o $(OBJ)/nevent.o		\
	$(OBJ)/cgrom.o $(OBJ)/cmt.o $(OBJ)/crtc.o 			\
	$(OBJ)/ctc.o	$(OBJ)/dipsw.o	$(OBJ)/dmac.o			\
	$(OBJ)/fdc.o	$(OBJ)/iocore.o	$(OBJ)/memio.o			\
	$(OBJ)/pcg.o	$(OBJ)/ppi.o	$(OBJ)/sio.o			\
	$(OBJ)/sndboard.o $(OBJ)/subcpu.o $(OBJ)/vramio.o		\
	$(OBJ)/make15.o	$(OBJ)/make24.o $(OBJ)/makeatr.o 		\
	$(OBJ)/makechr.o $(OBJ)/makemix.o $(OBJ)/makescrn.o		\
	$(OBJ)/makesub.o $(OBJ)/maketxth.o $(OBJ)/maketxtl.o	\
	$(OBJ)/vram.o $(OBJ)/palettes.o $(OBJ)/scrnsave.o		\
	$(OBJ)/scrndraw.o $(OBJ)/sdraw.o						\
	$(OBJ)/opmgenc.o $(OBJ)/opmgeng.oa $(OBJ)/psggenc.o		\
	$(OBJ)/psggeng.o $(OBJ)/sndctrl.o $(OBJ)/sound.o		\
	$(OBJ)/x1f.o $(OBJ)/diskdrv.o $(OBJ)/fdd_2d.o			\
	$(OBJ)/fdd_d88.o $(OBJ)/fdd_mtr.o $(OBJ)/fddfile.o		\
	$(OBJ)/font.o $(OBJ)/fontdata.o $(OBJ)/fontmake.o		\
	$(OBJ)/fontx1.o $(OBJ)/newdisk.o $(OBJ)/cmndraw.o		\
	$(OBJ)/dosio.o $(OBJ)/fontmng.o $(OBJ)/ini.o			\
	$(OBJ)/joymng.o $(OBJ)/mousemng.o $(OBJ)/scrnmng.o		\
	$(OBJ)/soundmng.o $(OBJ)/sysmng.o $(OBJ)/timemng.o		\
	$(OBJ)/trace.o $(OBJ)/dckbd.o $(OBJ)/xmil.o			\
	$(OBJ)/taskmng.o $(OBJ)/calendar.o $(OBJ)/debugsub.o		\
	$(OBJ)/pccore.o $(OBJ)/statsave.o $(OBJ)/timing.o		\
	$(OBJ)/event.o $(OBJ)/ramsave.o $(OBJ)/tmpfile.o 		\
	$(OBJ)/dccore.o $(OBJ)/vmu.o $(OBJ)/dc_softkbd.o		\
	$(OBJ)/sysmenu.o $(OBJ)/dlgabout.o $(OBJ)/filesel.o		\
	$(OBJ)/dlgcfg.o $(OBJ)/icon.o $(OBJ)/ui.o


ifdef RELEASE_BUILD
DCLIB	= -L$(RONIN)/lib -lronin-noserial -lz -lm
else
DCLIB	= -L$(RONIN)/lib -lronin -lz -lm
endif

DCELF	= $(BIN)/xmil.elf


all: $(OBJBASE) $(BINBASE) $(OBJ) $(BIN) xmil.strip.elf


$(OBJBASE):
	$(MKDIR) $(OBJBASE)

$(BINBASE):
	$(MKDIR) $(BINBASE)

$(OBJ):
	$(MKDIR) $(OBJ)

$(BIN):
	$(MKDIR) $(BIN)


$(DCELF): $(DCOBJ) 
	$(CC) $(LOPT) $(DCOBJ) $(DCLIB) -o $@

xmil.strip.elf: $(DCELF)
	sh-elf-strip -o $@ $<

xmil.bin: $(DCELF)
	sh-elf-objcopy -S -R .stack -O binary $< $@

1TURBOZ.BIN: xmil.bin
	scramble $< $@

ip.txt: ip.txt.in
	if [ $(shell echo $(XMILVER) | sed -e 's/[^0-9]//')  -ge 026 ]; then \
	 ver=V$(XMILVER); \
	else ver=git; fi; \
	sed -e 's/[@]VERSION[@]/'"$$ver"/ -e 's/[@]DATE[@]/$(shell date '+%Y%m%d')/' < $< 

IP.BIN: ip.txt
	makeip ip.txt IP.BIN

dist: 1TURBOZ.BIN IP.BIN
	plain=xmil-$(shell date '+%Y%m%d')-dreamcast-plainfiles \
	source=xmil-$(shell date '+%Y%m%d')-dreamcast-source \
	$(MKDIR) disttmp/$(plain) && \
	cp README.TXT disttmp/$(plain) && \
	cp LICENCE.TXT disttmp/$(plain) && \
	cp IP.BIN disttmp/$(plain) && \
	cp 1TURBOZ.BIN disttmp/$(plain) && \
	(cd disttmp && tar cvf - $(plain)) | bzip2 -c > $(plain).tar.bz2
	rm -rf disttmp/$(plain)
	$(MKDIR) disttmp/$(source) && \
	cp *.cpp *.h *.res *.str Makefile disttmp/$(source)/dc && \
	cp -r common disttmp/$(source)/dc && \
	cp -r fdd disttmp/$(source)/dc && \
	cp -r menu disttmp/$(source)/dc && \
	cp -r sh disttmp/$(source)/dc && \
	cp ../*.c ../*.h ../*.tbl disttmp/$(source) && \
	cp -r ../common disttmp/$(source) && \
	cp -r ../fdd disttmp/$(source) && \
	cp -r ../font disttmp/$(source) && \
	cp -r ../io disttmp/$(source) && \
	cp -r ../sound disttmp/$(source) && \
	cp -r ../vram disttmp/$(source) && \
	cp -r ../z80hdc disttmp/$(source) && \
	(cd disttmp && tar cvf - $(source) | bzip2 -c > $(source).tar.bz2
	rm -rf disttmp

distclean:
	$(RM) $(OBJBASE) $(BINBASE)

clean:
	$(RM) $(DCOBJ) $(DCELF) xmil.strip.elf 1TURBOZ.BIN IP.BIN ip.txt


$(OBJ)/strres.o		:	$(COMMON)/strres.c $(HEADERS) $(COMMON)/strres.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/milstr.o		:	$(COMMON)/milstr.c $(HEADERS) $(COMMON)/milstr.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/_memory.o	:	$(COMMON)/_memory.c $(HEADERS) $(TARGET)/dosio.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/textfile.o	:	$(COMMON)/textfile.c $(HEADERS) $(TARGET)/dosio.h \
						$(COMMON)/textfile.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/profile.o	:	$(TARGET)/common/profile.c $(HEADERS) $(TARGET)/dosio.h \
						$(COMMON)/textfile.h $(COMMON)/profile.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/rect.o		:	$(COMMON)/rect.c $(HEADERS) $(COMMON)/rect.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/lstarray.o	:	$(COMMON)/lstarray.c $(HEADERS) $(COMMON)/lstarray.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/bmpdata.o	:	$(COMMON)/bmpdata.c $(HEADERS) $(COMMON)/bmpdata.h
	$(CC) $(COPT) -c $< -o $@


$(OBJ)/parts.o		:	$(COMMON)/parts.c $(HEADERS) $(COMMON)/parts.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/parts.oa		:	$(SHASM)/parts.s
	$(AS) $(AOPT) $< -o $@

$(OBJ)/resize.o		:	$(COMMON)/resize.c $(HEADERS) $(COMMON)/resize.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/wavefile.o		:	$(COMMON)/wavefile.c $(HEADERS) $(COMMON)/wavefile.h
	$(CC) $(COPT) -c $< -o $@



$(OBJ)/z80c.o:		$(Z80C)/z80c.c $(HEADERS) $(Z80CORES) $(PCCORES) \
					$(IOCORES) $(COMMON)/parts.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/z80c_mn.o:	$(Z80C)/z80c_mn.c $(HEADERS) $(Z80CORES) $(PCCORES) \
					$(IOCORES) $(COMMON)/parts.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/z80c_sb.o:	$(Z80C)/z80c_sb.c $(HEADERS) $(Z80CORES) $(PCCORES) \
					$(IOCORES) $(COMMON)/parts.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/z80c_cb.o:	$(Z80C)/z80c_cb.c $(HEADERS) $(Z80CORES) $(PCCORES) \
					$(IOCORES) $(COMMON)/parts.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/z80c_ix.o:	$(Z80C)/z80c_ix.c $(HEADERS) $(Z80CORES) $(PCCORES) \
					$(IOCORES) $(COMMON)/parts.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/z80c_iy.o:	$(Z80C)/z80c_iy.c $(HEADERS) $(Z80CORES) $(PCCORES) \
					$(IOCORES) $(COMMON)/parts.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/z80mem.o:	$(Z80C)/z80mem.c $(HEADERS) $(Z80CORES) $(PCCORES) \
					$(IOCORES) $(COMMON)/parts.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/z80dmapc.o:	$(Z80C)/z80dmap.c $(HEADERS) $(Z80CORES) $(PCCORES) \
					$(IOCORES) $(COMMON)/parts.h
	$(CC) $(COPT) -c $< -o $@



$(OBJ)/z80h.o		:	$(Z80H)/z80h.c  $(HEADERS) $(Z80CORES) $(PCCORES) \
					$(IOCORES) $(COMMON)/parts.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/z80h_mn.oa		:	$(Z80H)/z80h_mn.s $(Z80H)/z80h.inc $(Z80H)/z80h_mn.inc
	$(AS) $(AOPT) $< -o $@

$(OBJ)/z80h_cb.oa		:	$(Z80H)/z80h_cb.s $(Z80H)/z80h.inc
	$(AS) $(AOPT) $< -o $@

$(OBJ)/z80h_s.oa		:	$(Z80H)/z80h_s.s $(Z80H)/z80h.inc $(Z80H)/z80h_mn.inc $(Z80H)/z80h_s.inc
	$(AS) $(AOPT) $< -o $@

$(OBJ)/z80h_i.oa		:	$(Z80H)/z80h_i.s $(Z80H)/z80h.inc $(Z80H)/z80h_mn.inc $(Z80H)/z80h_i.inc
	$(AS) $(AOPT) $< -o $@

$(OBJ)/z80mem.oa		:	$(Z80H)/z80mem.s $(Z80H)/z80h.inc
	$(AS) $(AOPT) $< -o $@

$(OBJ)/z80dmap.o		:	$(Z80H)/z80dmap.c $(HEADERS) $(Z80CORES) $(PCCORES) \
					$(IOCORES) $(BASE)/ievent.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/iocore.o		:	$(IO)/iocore.c $(HEADERS) $(PCCORES) \
						$(IOCORES)
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/cmt.o		:	$(IO)/cmt.c $(HEADERS) $(PCCORES) \
						$(IOCORES)
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/ctc.o		:	$(IO)/ctc.c $(HEADERS) $(PCCORES) $(CPUCORE) \
						$(IOCORES)
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/cgrom.o		:	$(IO)/cgrom.c $(HEADERS) $(PCCORES) \
						$(IOCORES) $(FONT)/font.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/crtc.o		:	$(IO)/crtc.c $(HEADERS) $(PCCORES) $(IOCORES) \
				$(VRAM)/vram.h $(VRAM)/palettes.h $(VRAM)/makescrn.h \
				$(TARGET)/scrnmng.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/dipsw.o		:	$(IO)/dipsw.c $(HEADERS) $(PCCORES) \
						$(IOCORES)
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/dmac.o		:	$(IO)/dmac.c $(HEADERS) $(PCCORES) $(IOCORES) \
						$(CPUCORE)
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/fdc.o		:	$(IO)/fdc.c $(HEADERS) $(PCCORES) $(IOCORES) \
				$(CPUCORE) $(FDD)/fdd_2d.h $(FDD)/fdd_d88.h $(FDD)/fdd_mtr.h\
				$(TARGET)/dosio.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/memio.o		:	$(IO)/memio.c $(HEADERS) $(PCCORES) $(IOCORES) \
				$(CPUCORE) $(IO)/memio.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/pcg.o		:	$(IO)/pcg.c $(HEADERS) $(PCCORES) $(IOCORES) \
				$(CPUCORE) $(IO)/pcg.h $(FONT)/font.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/ppi.o		:	$(IO)/ppi.c $(HEADERS) $(PCCORES) $(IOCORES) \
				$(VRAM)/makescrn.h $(IO)/ppi.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/sio.o		:	$(IO)/sio.c $(HEADERS) $(PCCORES) $(IOCORES) \
				$(CPUCORE) $(TARGET)/mousemng.h $(IO)/sio.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/sndboard.o		:	$(IO)/sndboard.c $(HEADERS) $(CPUCORES) $(PCCORES) \
						$(IOCORES) $(TARGET)/joymng.h $(SOUND)/sound.h \
						$(FMBOARDS) $(BASE)/keystat.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/subcpu.o		:	$(IO)/subcpu.c $(HEADERS) $(PCCORES) $(IOCORES) \
				$(CPUCORE) $(TARGET)/timemng.h $(BASE)/keystat.h $(BASE)/calendar.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/vramio.o		:	$(IO)/vramio.c $(HEADERS) $(PCCORES) $(IOCORES) \
				$(VRAM)/vram.h $(VRAM)/makescrn.h
	$(CC) $(COPT) -c $< -o $@



$(OBJ)/make15.o		:	$(VRAM)/make15.c $(HEADERS) $(CPUCORES) \
				$(VRAM)/vram.h $(VRAM)/scrndraw.h $(VRAM)/makescrn.h \
				$(VRAM)/makesub.h $(FONT)/font.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/make24.o		:	$(VRAM)/make24.c $(HEADERS) $(CPUCORES) \
				$(VRAM)/vram.h $(VRAM)/scrndraw.h $(VRAM)/makescrn.h \
				$(VRAM)/makesub.h $(FONT)/font.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/makemix.o	:	$(VRAM)/makemix.c $(HEADERS) $(CPUCORES) $(PCCORES) \
				$(IOCORES) $(VRAM)/vram.h $(VRAM)/scrndraw.h \
				$(VRAM)/makesub.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/makescrn.o	:	$(VRAM)/makescrn.c $(HEADERS) $(CPUCORES) $(PCCORES) \
				$(IOCORES) $(VRAM)/makescrn.h $(VRAM)/vram.h $(VRAM)/scrndraw.h \
				$(VRAM)/makesub.h $(VRAM)/palettes.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/makesub.o	:	$(VRAM)/makesub.c $(HEADERS) $(CPUCORES) $(PCCORES) \
				$(IOCORES) $(VRAM)/makesub.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/maketxth.o	:	$(VRAM)/maketxth.c $(HEADERS) $(CPUCORES) $(PCCORES) \
				$(IOCORES) $(VRAM)/scrndraw.h \
				$(VRAM)/makescrn.h $(VRAM)/makesub.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/maketxtl.o	:	$(VRAM)/maketxtl.c $(HEADERS) $(CPUCORES) $(PCCORES) \
				$(IOCORES) $(VRAM)/scrndraw.h \
				$(VRAM)/makescrn.h $(VRAM)/makesub.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/makechr.o	:	$(VRAM)/makechr.c $(HEADERS) $(CPUCORES) $(PCCORES) \
				$(IOCORES) $(VRAM)/scrndraw.h \
				$(VRAM)/makescrn.h $(VRAM)/makesub.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/makeatr.o	:	$(VRAM)/makeatr.c $(HEADERS) $(CPUCORES) $(PCCORES) \
				$(IOCORES) $(VRAM)/scrndraw.h \
				$(VRAM)/makescrn.h $(VRAM)/makesub.h
	$(CC) $(COPT) -c $< -o $@




$(OBJ)/palettes.o	:	$(VRAM)/palettes.c $(HEADERS) $(PCCORES) $(IOCORES) \
				$(TARGET)/scrnmng.h $(VRAM)/scrndraw.h \
				$(VRAM)/palettes.h $(VRAM)/makescrn.h
	$(CC) $(COPT) -c $< -o $@


$(OBJ)/scrndraw.o	:	$(VRAM)/scrndraw.c $(HEADERS) $(PCCORES) $(IOCORES) \
				$(TARGET)/scrnmng.h $(VRAM)/scrndraw.h \
				$(VRAM)/sdraw.h $(VRAM)/makescrn.h $(VRAM)/palettes.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/sdraw.o	:	$(VRAM)/sdraw.c $(HEADERS) $(PCCORES) $(IOCORES) \
				$(TARGET)/scrnmng.h $(VRAM)/scrndraw.h \
				$(VRAM)/sdraw.h $(VRAM)/sdraw.mcr $(VRAM)/palettes.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/vram.o		:	$(VRAM)/vram.c $(HEADERS) $(CPUCORES) $(VRAM)/vram.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/scrnsave.o	:	$(VRAM)/scrnsave.c $(HEADERS) $(PCCORES) $(IOCORES) \
						$(COMMON)/bmpdata.h $(VRAM)/scrndraw.h \
						$(VRAM)/palettes.h $(VRAM)/makescrn.h \
						$(VRAM)/scrnsave.h
	$(CC) $(COPT) -c $< -o $@




$(OBJ)/opmgenc.o	:	$(SOUND)/opmgenc.c $(HEADERS) $(PCCORES) $(IOCORES) \
				$(FMBOARDS)
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/opmgeng.o	:	$(SOUND)/opmgeng.c $(HEADERS) $(PCCORES) $(IOCORES) \
				$(FMBOARDS)
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/opmgeng.oa	:	$(SHASM)/opmgeng.s
	$(AS) $(AOPT) $< -o $@

$(OBJ)/psggenc.o	:	$(SOUND)/psggenc.c $(HEADERS) $(PCCORES) $(IOCORES) \
				$(FMBOARDS)
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/psggeng.o	:	$(SOUND)/psggeng.c $(HEADERS) $(PCCORES) $(IOCORES) \
				$(FMBOARDS)
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/sound.o		:	$(SOUND)/sound.c $(HEADERS) $(CPUCORES) $(PCCORES) \
				$(IOCORES) $(FMBOARDS) $(TARGET)/soundmng.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/x1f.o		:	$(SOUND)/x1f.c $(HEADERS) $(CPUCORES) $(PCCORES) \
				$(IOCORES) $(FMBOARDS) $(TARGET)/dosio.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/sndctrl.o		:	$(SOUND)/sndctrl.c $(HEADERS) $(CPUCORES) $(PCCORES) \
				$(IOCORES) $(FMBOARDS) $(TARGET)/soundmng.h
	$(CC) $(COPT) -c $< -o $@


$(OBJ)/diskdrv.o	:	$(FDD)/diskdrv.c $(HEADERS) $(PCCORES) $(IOCORES) \
				$(TARGET)/sysmng.h \
				$(FDD)/diskdrv.h $(FDD)/fddfile.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/fdd_2d.o	:	$(TARGET)/fdd/fdd_2d.c $(HEADERS) $(PCCORES) $(IOCORES) \
			$(TARGET)/dosio.h $(FDD)/fddfile.h $(FDD)/fdd_2d.h $(FDD)/fdd_mtr.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/fdd_d88.o	:	$(TARGET)/fdd/fdd_d88.c $(HEADERS) $(PCCORES) $(IOCORES) \
				$(TARGET)/dosio.h $(FDD)/fddfile.h $(FDD)/fdd_d88.h $(FDD)/fdd_mtr.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/fdd_mtr.o	:	$(FDD)/fdd_mtr.c $(HEADERS) $(PCCORES) \
				$(FDD)/fdd_mtr.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/fddfile.o	:	$(FDD)/fddfile.c $(HEADERS) $(PCCORES) $(IOCORES) \
				$(COMMON)/strres.h $(TARGET)/dosio.h \
				$(TARGET)/sysmng.h $(FDD)/fddfile.h \
				$(FDD)/fdd_2d.h $(FDD)/fdd_d88.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/newdisk.o	:	$(FDD)/newdisk.c $(HEADERS) $(TARGET)/dosio.h \
						$(FDD)/newdisk.h $(FDD)/fddfile.h
	$(CC) $(COPT) -c $< -o $@




$(OBJ)/font.o		:	$(FONT)/font.c $(HEADERS) $(CPUCORES) \
				$(COMMON)/strres.h $(TARGET)/dosio.h $(FONT)/font.h \
				$(FONT)/fontdata.h $(FONT)/fontmake.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/fontdata.o	:	$(FONT)/fontdata.c $(HEADERS) $(CPUCORES) \
				$(FONT)/font.h $(FONT)/fontdata.h
	$(CC) $(COPT) -c $< -o $@


$(OBJ)/fontmake.o	:	$(FONT)/fontmake.c $(HEADERS) $(COMMON)/bmpdata.h \
				$(COMMON)/parts.h $(TARGET)/dosio.h \
				$(FONT)/font.h \
				$(FONT)/fontdata.h $(FONT)/fontmake.h
	$(CC) $(COPT) -c $< -o $@


$(OBJ)/fontx1.o		:	$(FONT)/fontx1.c $(HEADERS) $(CPUCORES) \
				$(COMMON)/parts.h $(TARGET)/dosio.h $(FONT)/font.h \
				$(FONT)/fontdata.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/cmndraw.o	:	$(GENERIC)/cmndraw.c $(HEADERS) $(COMMON)/bmpdata.h \
				$(TARGET)/scrnmng.h $(GENERIC)/cmndraw.h $(GENERIC)/minifont.res
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/xmil.o		:	$(TARGET)/xmil.cpp $(HEADERS) $(PCCORES) $(IOCORES) \
				$(COMMON)/strres.h $(TARGET)/xmil.h \
				$(TARGET)/dosio.h  \
				$(BASE)/timing.h $(BASE)/statsave.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/dosio.o		:	$(TARGET)/dosio.cpp $(HEADERS) $(TARGET)/dosio.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/trace.o		:	$(TARGET)/trace.cpp $(HEADERS) $(COMMON)/strres.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/fontmng.o	:	$(TARGET)/fontmng.cpp $(HEADERS) \
						$(TARGET)/fontmng.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/joymng.o		:	$(TARGET)/joymng.cpp $(HEADERS) $(TARGET)/joymng.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/mousemng.o	:	$(TARGET)/mousemng.cpp $(HEADERS) $(TARGET)/mousemng.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/scrnmng.o	:	$(TARGET)/scrnmng.cpp $(HEADERS) \
						$(TARGET)/scrnmng.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/soundmng.o	:	$(TARGET)/soundmng.cpp $(HEADERS) $(COMMON)/parts.h \
						$(TARGET)/soundmng.h \
						$(SOUND)/sound.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/sysmng.o		:	$(TARGET)/sysmng.cpp $(HEADERS) $(TARGET)/sysmng.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/timemng.o	:	$(TARGET)/timemng.cpp $(HEADERS) $(TARGET)/timemng.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/dckbd.o		:	$(TARGET)/dckbd.cpp $(HEADERS) $(PCCORES) \
						$(IOCORES) $(TARGET)/xmil.h $(TARGET)/dckbd.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/ini.o		:	$(TARGET)/ini.cpp $(HEADERS) $(PCCORES) \
				$(TARGET)/xmil.h $(TARGET)/dosio.h $(TARGET)/ini.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/ievent.o		:	$(BASE)/ievent.c $(HEADERS) $(CPUCORES) $(PCCORES)
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/nevent.o		:	$(BASE)/nevent.c $(HEADERS) $(CPUCORES) $(PCCORES)
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/calendar.o	:	$(BASE)/calendar.c $(HEADERS) $(PCCORES) \
						$(COMMON)/parts.h $(TARGET)/timemng.h \
						$(BASE)/calendar.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/timing.o		:	$(BASE)/timing.c $(HEADERS) $(PCCORES) \
				$(FDD)/diskdrv.h $(FDD)/fdd_mtr.h $(BASE)/timing.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/keystat.o	:	$(BASE)/keystat.c $(HEADERS) $(PCCORES) $(IOCORES) \
				$(BASE)/keystat.h $(BASE)/keystat.tbl
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/statsave.o	:	$(BASE)/statsave.c $(HEADERS) $(CPUCORES) $(PCCORES) \
				$(IOCORES) $(FMBOARDS) $(COMMON)/strres.h \
				$(TARGET)/dosio.h  \
				$(TARGET)/scrnmng.h $(TARGET)/soundmng.h \
				$(TARGET)/timemng.h \
				$(SOUND)/sound.h \
				$(FONT)/font.h $(FDD)/fddfile.h \
				$(BASE)/calendar.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/debugsub.o	:	$(BASE)/debugsub.c $(HEADERS) $(CPUCORES) $(PCCORES) \
				$(IOCORES) $(COMMON)/strres.h $(TARGET)/dosio.h \
				$(BASE)/debugsub.h $(VRAM)/vram.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/pccore.o	:	$(BASE)/pccore.c $(HEADERS) $(CPUCORES) $(PCCORES) \
				$(IOCORES) $(FMBOARDS) $(COMMON)/strres.h \
				$(TARGET)/dosio.h  \
				$(TARGET)/scrnmng.h $(TARGET)/soundmng.h \
				$(TARGET)/timemng.h $(TARGET)/sysmng.h \
				$(SOUND)/sound.h \
				$(FONT)/font.h $(FDD)/fddfile.h \
				$(BASE)/calendar.h $(BASE)/xmilver.h \
				$(BASE)/keystat.h $(BASE)/timing.h $(VRAM)/makescrn.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/taskmng.o		:	 $(TARGET)/taskmng.cpp $(TARGET)/taskmng.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/dccore.o		:	 $(TARGET)/dccore.cpp $(TARGET)/dccore.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/ramsave.o		:	 $(TARGET)/ramsave.cpp $(TARGET)/ramsave.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/event.o		:	 $(TARGET)/event.cpp $(TARGET)/event.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/icon.o		:	 $(TARGET)/icon.cpp $(TARGET)/icon.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/tmpfile.o		:	 $(TARGET)/tmpfile.cpp $(TARGET)/tmpfile.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/ui.o		:	 $(TARGET)/ui.cpp $(TARGET)/ui.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/dc_softkbd.o		:	 $(TARGET)/dc_softkbd.cpp $(TARGET)/dc_softkbd.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/vmu.o		:	 $(TARGET)/vmu.cpp $(TARGET)/vmu.h $(TARGET)/m4x8.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/sysmenu.o		:	 $(TARGET)/sysmenu.cpp $(TARGET)/sysmenu.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/dlgcfg.o		:	 $(MENU)/dlgcfg.cpp $(MENU)/dlgcfg.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/dlgabout.o		:	 $(MENU)/dlgabout.cpp $(MENU)/dlgabout.h
	$(CC) $(COPT) -c $< -o $@

$(OBJ)/filesel.o		:	 $(MENU)/filesel.cpp $(MENU)/filesel.h
	$(CC) $(COPT) -c $< -o $@
