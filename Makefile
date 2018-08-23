
all:
	cd libs_recognition && make -j4
	cd train && make -j4
	cd image_recognition_test && make -j4




clean:
	cd libs_recognition && make clean
	cd train && make clean
	cd image_recognition_test && make clean
