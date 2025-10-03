IP=10.12.154.169
#IP=192.168.0.162
BIN := $(wildcard /tmp/arduino-build/*.ino.bin)

bin:
	# arduino-cli compile --build-path /tmp/arduino-build -b esp32:esp32:esp32s3:CDCOnBoot=cdc,PSRAM=opi,PartitionScheme=app3M_fat9M_16MB .
	arduino-cli compile --build-path /tmp/arduino-build -b esp32:esp32:esp32s3:FlashSize=16M,PSRAM=opi,PartitionScheme=app3M_fat9M_16MB .

flash:
	make serial

serial:
	- @ pkill -9 -f microcom
	arduino-cli upload -p /dev/ttyACM0 --fqbn esp32:esp32:esp32s3 /tmp/arduino-build --input-dir /tmp/arduino-build
	- @ pkill -9 -f sleep

 ota:
	python3 "/home/holla/.arduino15/packages/esp32/hardware/esp32/2.0.11/tools/espota.py" -r -i $(IP) -p 3232 --auth=admin -f "$(BIN)"


con:
	 @ nc -kul 3333

debug:
	 @ nc -kul 3334

log:
	@ nc -kul 3333 | tee -a ../../testing/log-`date +%y%m%d-%H%M%S`.txt

clean:
	rm -rf /tmp/arduino-build/*
