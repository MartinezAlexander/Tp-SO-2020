################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/deadlock.c \
../src/entrenador.c \
../src/objetivos.c \
../src/proceso-team.c 

OBJS += \
./src/deadlock.o \
./src/entrenador.o \
./src/objetivos.o \
./src/proceso-team.o 

C_DEPS += \
./src/deadlock.d \
./src/entrenador.d \
./src/objetivos.d \
./src/proceso-team.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


