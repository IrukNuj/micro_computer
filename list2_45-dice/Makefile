# ここにビルドに必要なCソースファイル名を並べる
# （例： SRCS = src1.c src2. src3.c と書く）
SRCS = test.c

# GUIモニタを使うときは ON にする
#（ユーザプログラムでシリアル通信を行う場合はOFFにすること）
MON = OFF

#--------------------------------------------
# ここから下は理由がない限り書き換えない

# プロジェクト名にはカレントディレクトリ名を割当
PROJECT =   $(notdir $(CURDIR))

SDEV = /dev/emb88
BAUD = 38400

OBJECTS = $(SRCS:.c=.o)
ifeq (${MON},ON)
	MONITOR_OBJ = /usr/local/lib/avr/lib/onchip_mon.o
else
	MONITOR_OBJ =
endif

MPU = atmega88pa
F_CPU = 8000000UL

MAPFILE = ${PROJECT}.map
LSSFILE = ${PROJECT}.lss
ELFFILE = ${PROJECT}.elf
HEXFILE = ${PROJECT}.hex
EEPFILE = ${PROJECT}.eep

CC = avr-gcc
CFLAGS = -mmcu=${MPU} -W -Wall -Werror-implicit-function-declaration -DF_CPU=${F_CPU} -Os -g2
#CFLAGS += -save-temps
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -fdiagnostics-color 
#CFLAGS += -fno-tree-vrp 
LDFLAGS = -mmcu=${MPU} -Wl,-Map=${MAPFILE}

WORK_LOG = ${HOME}/.laba_mywork.log

.PHONY: all clean distclean

all: clean ${HEXFILE} dl

${HEXFILE}: ${OBJECTS}
	avr-gcc ${LDFLAGS} ${OBJECTS} ${MONITOR_OBJ} -o ${ELFFILE}
	avr-objcopy -j.text -j.data -O ihex ${ELFFILE} ${HEXFILE}
	avr-objcopy -j.eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 --no-change-warnings -O ihex ${ELFFILE} ${EEPFILE}
	avr-objdump -h -S ${ELFFILE} > ${LSSFILE}
	@/bin/echo -e "\n================"
	@avr-size -C --mcu=${MPU} ${ELFFILE}

distclean: clean
	@rm -f ${HEXFILE}

clean:
	@/bin/echo -e "\\033c"
	@rm -f ${OBJECTS} ${MAPFILE} ${LSSFILE} ${ELFFILE} ${EEPFILE} ${PROJECT}.[iso]

dl: ${HEXFILE}
	@killall -q -STOP e8mon || true
	@[ -c ${SDEV} ] && (/bin/echo -e "================\nHEX Programmer\n----------------"; con2com ${SDEV} ${BAUD} ${HEXFILE} / ; echo `date +"%s,  0, $<"` >> ${WORK_LOG}) || true
	@killall -q -CONT e8mon || true

%.o:	%.c Makefile
	${CC}  -c  $< -o $@ ${CFLAGS} && (echo `date +"%s,+1, $<"` >> ${WORK_LOG}) || (echo `date +"%s,-1, $<"` >> ${WORK_LOG}; false)
