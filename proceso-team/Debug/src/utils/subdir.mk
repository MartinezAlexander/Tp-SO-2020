################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/utils/array_utils.c \
../src/utils/log_utils.c \
../src/utils/posicion_utils.c 

OBJS += \
./src/utils/array_utils.o \
./src/utils/log_utils.o \
./src/utils/posicion_utils.o 

C_DEPS += \
./src/utils/array_utils.d \
./src/utils/log_utils.d \
./src/utils/posicion_utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/%.o: ../src/utils/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


