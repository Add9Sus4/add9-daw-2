################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../audio_file.cc \
../folder.cc \
../main_window.cc \
../test.cc \
../vertical_scroll_bar.cc \
../window_area.cc 

CC_DEPS += \
./audio_file.d \
./folder.d \
./main_window.d \
./test.d \
./vertical_scroll_bar.d \
./window_area.d 

OBJS += \
./audio_file.o \
./folder.o \
./main_window.o \
./test.o \
./vertical_scroll_bar.o \
./window_area.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -Wno-deprecated-declarations -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


