################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../audio_file.cc \
../run_daw.cc 

CPP_SRCS += \
../arrange_window.cpp \
../audio_clip.cpp \
../audio_track.cpp \
../files_window.cpp \
../folder.cpp \
../menu.cpp \
../menu_option.cpp \
../section.cpp \
../vertical_scrollbar.cpp \
../window.cpp 

CC_DEPS += \
./audio_file.d \
./run_daw.d 

OBJS += \
./arrange_window.o \
./audio_clip.o \
./audio_file.o \
./audio_track.o \
./files_window.o \
./folder.o \
./menu.o \
./menu_option.o \
./run_daw.o \
./section.o \
./vertical_scrollbar.o \
./window.o 

CPP_DEPS += \
./arrange_window.d \
./audio_clip.d \
./audio_track.d \
./files_window.d \
./folder.d \
./menu.d \
./menu_option.d \
./section.d \
./vertical_scrollbar.d \
./window.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -Wno-deprecated-declarations -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -Wno-deprecated-declarations -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


