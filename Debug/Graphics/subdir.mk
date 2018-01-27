################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Graphics/window.cpp 

OBJS += \
./Graphics/window.o 

CPP_DEPS += \
./Graphics/window.d 


# Each subdirectory must supply rules for building sources it contributes
Graphics/%.o: ../Graphics/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -Wno-deprecated-declarations -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


