.PHONY: reload build watch test

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


test_build/Makefile: lcd/CMakeLists.txt
	mkdir -p test_build
	$(CMAKE) -B test_build -DIS_TESTING=Y -DCMAKE_BUILD_TYPE=Debug lcd 


test: test_build/Makefile
	${CMAKE} --build test_build
	test_build/test_display
	