################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Vendor/src/stb_image.cpp \
../Vendor/src/tiny_obj_loader.cpp 

OBJS += \
./Vendor/src/stb_image.o \
./Vendor/src/tiny_obj_loader.o 

CPP_DEPS += \
./Vendor/src/stb_image.d \
./Vendor/src/tiny_obj_loader.d 


# Each subdirectory must supply rules for building sources it contributes
Vendor/src/%.o: ../Vendor/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


