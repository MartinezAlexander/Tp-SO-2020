################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/planificacion/planificacion.c \
../src/planificacion/planificacion_fifo.c \
../src/planificacion/planificacion_rr.c \
../src/planificacion/planificacion_sjf.c \
../src/planificacion/planificador.c 

OBJS += \
./src/planificacion/planificacion.o \
./src/planificacion/planificacion_fifo.o \
./src/planificacion/planificacion_rr.o \
./src/planificacion/planificacion_sjf.o \
./src/planificacion/planificador.o 

C_DEPS += \
./src/planificacion/planificacion.d \
./src/planificacion/planificacion_fifo.d \
./src/planificacion/planificacion_rr.d \
./src/planificacion/planificacion_sjf.d \
./src/planificacion/planificador.d 


# Each subdirectory must supply rules for building sources it contributes
src/planificacion/%.o: ../src/planificacion/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


