.PHONY: reload build watch

CMAKE=cmake

build:
	echo "Remaking application"
	$(CMAKE) --build build

load:
	@echo "Loading application to device"
	picotool load -f build/picosense.uf2
	echo "Allowing device to restart"
	sleep 2
	
reload:
	$(MAKE) build
	$(MAKE) load

watch:
	echo "Reading messages from serial output"
	bash -c "cat /dev/ttyACM0 | tee -a traces/$$(date +'%Y.%d.%m').$$$$.log"

minicom:
	minicom -b 115200 -o -D /dev/ttyACM0 
