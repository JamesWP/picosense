.PHONY: reload build

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
	echo "Reading messages from serial output"
	cat /dev/ttyACM0

