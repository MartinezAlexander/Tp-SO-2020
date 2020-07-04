################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/particiones_dinamicas/administrador_particiones.c \
../src/particiones_dinamicas/particiones_dinamicas.c 

OBJS += \
./src/particiones_dinamicas/administrador_particiones.o \
./src/particiones_dinamicas/particiones_dinamicas.o 

C_DEPS += \
./src/particiones_dinamicas/administrador_particiones.d \
./src/particiones_dinamicas/particiones_dinamicas.d 


# Each subdirectory must supply rules for building sources it contributes
src/particiones_dinamicas/%.o: ../src/particiones_dinamicas/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


