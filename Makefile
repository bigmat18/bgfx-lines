EXE_NAME = main


setup:
	cmake -S . -B build

build:
	cmake --build build

run:
	build/${EXE_NAME}