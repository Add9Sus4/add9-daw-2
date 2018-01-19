################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../AudioFile.cpp \
../Folder.cpp \
../MainWindow.cpp \
../Test.cpp \
../VerticalScrollBar.cpp \
../WindowArea.cpp 

OBJS += \
./AudioFile.o \
./Folder.o \
./MainWindow.o \
./Test.o \
./VerticalScrollBar.o \
./WindowArea.o 

CPP_DEPS += \
./AudioFile.d \
./Folder.d \
./MainWindow.d \
./Test.d \
./VerticalScrollBar.d \
./WindowArea.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


