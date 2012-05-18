################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/DSDecoder.cpp \
../src/DSEncoder.cpp \
../src/Main.cpp \
../src/TestCase.cpp \
../src/decoders.cpp 

OBJS += \
./src/DSDecoder.o \
./src/DSEncoder.o \
./src/Main.o \
./src/TestCase.o \
./src/decoders.o 

CPP_DEPS += \
./src/DSDecoder.d \
./src/DSEncoder.d \
./src/Main.d \
./src/TestCase.d \
./src/decoders.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -O3 -Wall -c -fmessage-length=0 -std=gnu++0x -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


