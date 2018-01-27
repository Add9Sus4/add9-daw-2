################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../Audio/audio_file.cc 

CC_DEPS += \
./Audio/audio_file.d 

OBJS += \
./Audio/audio_file.o 


# Each subdirectory must supply rules for building sources it contributes
Audio/%.o: ../Audio/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -Wno-deprecated-declarations -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


