################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../AppConfig.cpp \
../JSONObject.cpp \
../SharedData.cpp 

CPP_DEPS += \
./AppConfig.d \
./JSONObject.d \
./SharedData.d 

OBJS += \
./AppConfig.o \
./JSONObject.o \
./SharedData.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/ -I"/home/harlan/eclipse-workspace/solaris" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean--2e-

clean--2e-:
	-$(RM) ./AppConfig.d ./AppConfig.o ./JSONObject.d ./JSONObject.o ./SharedData.d ./SharedData.o

.PHONY: clean--2e-

