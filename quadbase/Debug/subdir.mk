################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../LiquidCrystal.cpp \
../main.cpp \
../radioclient.cpp \
../vrbot.cpp 

OBJS += \
./LiquidCrystal.o \
./main.o \
./radioclient.o \
./vrbot.o 

CPP_DEPS += \
./LiquidCrystal.d \
./main.d \
./radioclient.d \
./vrbot.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"C:\Users\derekja\workspace\SENG466Project1\radio" -I"C:\Users\derekja\workspace\SENG466Project1\Arduino" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=atmega1280 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


