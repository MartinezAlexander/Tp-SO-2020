################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/buddy_system/administrador_buddy_system.c \
../src/buddy_system/buddy_system.c 

OBJS += \
./src/buddy_system/administrador_buddy_system.o \
./src/buddy_system/buddy_system.o 

C_DEPS += \
./src/buddy_system/administrador_buddy_system.d \
./src/buddy_system/buddy_system.d 


# Each subdirectory must supply rules for building sources it contributes
src/buddy_system/%.o: ../src/buddy_system/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


