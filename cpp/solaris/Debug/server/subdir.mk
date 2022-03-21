################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../server/ReceiveDatagramThread.cpp \
../server/ServerMain.cpp 

CPP_DEPS += \
./server/ReceiveDatagramThread.d \
./server/ServerMain.d 

OBJS += \
./server/ReceiveDatagramThread.o \
./server/ServerMain.o 


# Each subdirectory must supply rules for building sources it contributes
server/%.o: ../server/%.cpp server/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/ -I"/home/harlan/eclipse-workspace/solaris" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-server

clean-server:
	-$(RM) ./server/ReceiveDatagramThread.d ./server/ReceiveDatagramThread.o ./server/ServerMain.d ./server/ServerMain.o

.PHONY: clean-server

