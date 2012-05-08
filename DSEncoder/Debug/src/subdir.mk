################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/DSDecoder.cpp \
../src/DSEncoder.cpp \
../src/Main.cpp \
../src/TestCase.cpp 

OBJS += \
./src/DSDecoder.o \
./src/DSEncoder.o \
./src/Main.o \
./src/TestCase.o 

CPP_DEPS += \
./src/DSDecoder.d \
./src/DSEncoder.d \
./src/Main.d \
./src/TestCase.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/kxiao/Downloads/maropu-integer_encoding_library-73583ce/include -O0 -g3 -p -pg -Wall -c -fmessage-length=0 -std=gnu++0x -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


