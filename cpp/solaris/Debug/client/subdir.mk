################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../client/ClientMain.cpp \
../client/PublishDatagramThread.cpp 

CPP_DEPS += \
./client/ClientMain.d \
./client/PublishDatagramThread.d 

OBJS += \
./client/ClientMain.o \
./client/PublishDatagramThread.o 


# Each subdirectory must supply rules for building sources it contributes
client/%.o: ../client/%.cpp client/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/ -I"/home/harlan/eclipse-workspace/solaris" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-client

clean-client:
	-$(RM) ./client/ClientMain.d ./client/ClientMain.o ./client/PublishDatagramThread.d ./client/PublishDatagramThread.o

.PHONY: clean-client

