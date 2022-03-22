################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../util/Base64.cpp 

CPP_DEPS += \
./util/Base64.d 

OBJS += \
./util/Base64.o 


# Each subdirectory must supply rules for building sources it contributes
util/%.o: ../util/%.cpp util/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/ -I"/home/harlan/eclipse-workspace/solaris/cpp/solaris" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-util

clean-util:
	-$(RM) ./util/Base64.d ./util/Base64.o

.PHONY: clean-util

