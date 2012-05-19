################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/compress/table/decDelta.cpp \
../src/compress/table/decGamma.cpp \
../src/compress/table/decUnary.cpp 

OBJS += \
./src/compress/table/decDelta.o \
./src/compress/table/decGamma.o \
./src/compress/table/decUnary.o 

CPP_DEPS += \
./src/compress/table/decDelta.d \
./src/compress/table/decGamma.d \
./src/compress/table/decUnary.d 


# Each subdirectory must supply rules for building sources it contributes
src/compress/table/%.o: ../src/compress/table/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D__GXX_EXPERIMENTAL_CXX0X__ -O0 -g3 -Wall -c -fmessage-length=0 -std=gnu++0x -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


