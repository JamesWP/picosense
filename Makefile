.PHONY: reload build watch

CMAKE=cmake

build:
	echo "Remaking application"
	$(CMAKE) --build build
	
reload: build
	@echo "Loading application to device"
	picotool load -f build/picosense.uf2
	echo "Allowing device to restart"
	sleep 2
	echo "Assuming device has restarted"
	$(MAKE) watch

watch:
	echo "Reading messages from serial output"
	bash -c "cat /dev/ttyACM0 | tee -a traces/$$(date +'%Y.%d.%m').$$$$.log"