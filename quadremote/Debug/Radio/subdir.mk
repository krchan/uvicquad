################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Radio/radio.cpp \
../Radio/spi.cpp 

OBJS += \
./Radio/radio.o \
./Radio/spi.o 

CPP_DEPS += \
./Radio/radio.d \
./Radio/spi.d 


# Each subdirectory must supply rules for building sources it contributes
Radio/%.o: ../Radio/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"C:\Users\derekja\workspace\SENG466RemoteStation\Arduino" -I"C:\Users\derekja\workspace\SENG466RemoteStation\Radio" -I"C:\Users\derekja\workspace\SENG466RemoteStation\RTOS" -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=atmega1280 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


