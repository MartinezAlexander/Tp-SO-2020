################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/mensajes/procesamiento_appeared.c \
../src/mensajes/procesamiento_caught.c \
../src/mensajes/procesamiento_localized.c 

OBJS += \
./src/mensajes/procesamiento_appeared.o \
./src/mensajes/procesamiento_caught.o \
./src/mensajes/procesamiento_localized.o 

C_DEPS += \
./src/mensajes/procesamiento_appeared.d \
./src/mensajes/procesamiento_caught.d \
./src/mensajes/procesamiento_localized.d 


# Each subdirectory must supply rules for building sources it contributes
src/mensajes/%.o: ../src/mensajes/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


