################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/conexiones/conexion_broker.c \
../src/conexiones/conexion_gameboy.c 

OBJS += \
./src/conexiones/conexion_broker.o \
./src/conexiones/conexion_gameboy.o 

C_DEPS += \
./src/conexiones/conexion_broker.d \
./src/conexiones/conexion_gameboy.d 


# Each subdirectory must supply rules for building sources it contributes
src/conexiones/%.o: ../src/conexiones/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


