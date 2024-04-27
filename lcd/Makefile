
FONT2C=../../font2c/src/font2c

.PHONY: all
all: font-7x13.xbm font-10x20.xbm robo-70.xbm

font-7x13.xbm:
	$(FONT2C) /usr/share/fonts/X11/misc/7x13-ISO8859-1.pcf.gz Font7x13 > font-7x13.xbm

font-10x20.xbm:
	$(FONT2C) /usr/share/fonts/X11/misc/10x20-ISO8859-1.pcf.gz Font10x20 > font-10x20.xbm

RobotoMono-Regular.ttf:
	curl https://raw.githubusercontent.com/googlefonts/RobotoMono/main/fonts/ttf/RobotoMono-Regular.ttf --output RobotoMono-Regular.ttf

robo-70.xbm: RobotoMono-Regular.ttf
	$(FONT2C) RobotoMono-Regular.ttf ROBO 70 > robo-70.xbm


