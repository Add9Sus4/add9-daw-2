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
../clap_pattern.cpp \
../files_window.cpp \
../folder.cpp \
../hat_pattern.cpp \
../kick_pattern.cpp \
../menu.cpp \
../menu_option.cpp \
../pattern.cpp \
../section.cpp \
../snare_pattern.cpp \
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
./clap_pattern.o \
./files_window.o \
./folder.o \
./hat_pattern.o \
./kick_pattern.o \
./menu.o \
./menu_option.o \
./pattern.o \
./run_daw.o \
./section.o \
./snare_pattern.o \
./vertical_scrollbar.o \
./window.o 

CPP_DEPS += \
./arrange_window.d \
./audio_clip.d \
./audio_track.d \
./clap_pattern.d \
./files_window.d \
./folder.d \
./hat_pattern.d \
./kick_pattern.d \
./menu.d \
./menu_option.d \
./pattern.d \
./section.d \
./snare_pattern.d \
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


