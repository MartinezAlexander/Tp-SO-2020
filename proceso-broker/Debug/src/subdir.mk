################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/administrador_mensajes.c \
../src/cola.c \
../src/config.c \
../src/dump.c \
../src/logger.c \
../src/manejador_suscripciones.c \
../src/memoria_cache.c \
../src/procesar.c \
../src/proceso-broker.c \
../src/suscriptor.c 

OBJS += \
./src/administrador_mensajes.o \
./src/cola.o \
./src/config.o \
./src/dump.o \
./src/logger.o \
./src/manejador_suscripciones.o \
./src/memoria_cache.o \
./src/procesar.o \
./src/proceso-broker.o \
./src/suscriptor.o 

C_DEPS += \
./src/administrador_mensajes.d \
./src/cola.d \
./src/config.d \
./src/dump.d \
./src/logger.d \
./src/manejador_suscripciones.d \
./src/memoria_cache.d \
./src/procesar.d \
./src/proceso-broker.d \
./src/suscriptor.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


