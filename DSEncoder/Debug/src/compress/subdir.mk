################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/compress/PForDelta.cpp \
../src/compress/Simple16.cpp \
../src/compress/Simple9.cpp \
../src/compress/VariableByte.cpp 

OBJS += \
./src/compress/PForDelta.o \
./src/compress/Simple16.o \
./src/compress/Simple9.o \
./src/compress/VariableByte.o 

CPP_DEPS += \
./src/compress/PForDelta.d \
./src/compress/Simple16.d \
./src/compress/Simple9.d \
./src/compress/VariableByte.d 


# Each subdirectory must supply rules for building sources it contributes
src/compress/%.o: ../src/compress/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -p -pg -Wall -c -fmessage-length=0 -std=gnu++0x -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


