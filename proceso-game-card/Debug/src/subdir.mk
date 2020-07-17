################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bitmap.c \
../src/block.c \
../src/conexion_broker.c \
../src/conexion_gameboy.c \
../src/file_pokemon.c \
../src/info_posiciones.c \
../src/logger.c \
../src/procesamiento_mensajes.c \
../src/proceso-game-card.c \
../src/tall_grass.c 

OBJS += \
./src/bitmap.o \
./src/block.o \
./src/conexion_broker.o \
./src/conexion_gameboy.o \
./src/file_pokemon.o \
./src/info_posiciones.o \
./src/logger.o \
./src/procesamiento_mensajes.o \
./src/proceso-game-card.o \
./src/tall_grass.o 

C_DEPS += \
./src/bitmap.d \
./src/block.d \
./src/conexion_broker.d \
./src/conexion_gameboy.d \
./src/file_pokemon.d \
./src/info_posiciones.d \
./src/logger.d \
./src/procesamiento_mensajes.d \
./src/proceso-game-card.d \
./src/tall_grass.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


