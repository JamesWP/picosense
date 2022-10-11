.PHONY: reload build

build:
	echo "Remaking application"
	$(MAKE) -C build
	
reload: build
	@echo "Loading application to device"
	picotool load -f build/picosense.uf2
	echo "Allowing device to restart"
	sleep 2
	echo "Assuming device has restarted"
	echo "Reading messages from serial output"
	cat /dev/ttyACM0

