################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Vendor/imgui-master/misc/cpp/imgui_stdlib.cpp 

OBJS += \
./Vendor/imgui-master/misc/cpp/imgui_stdlib.o 

CPP_DEPS += \
./Vendor/imgui-master/misc/cpp/imgui_stdlib.d 


# Each subdirectory must supply rules for building sources it contributes
Vendor/imgui-master/misc/cpp/%.o: ../Vendor/imgui-master/misc/cpp/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


