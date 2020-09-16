################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Vendor/imgui-master/imgui.cpp \
../Vendor/imgui-master/imgui_demo.cpp \
../Vendor/imgui-master/imgui_draw.cpp \
../Vendor/imgui-master/imgui_impl_opengl3.cpp \
../Vendor/imgui-master/imgui_impl_sdl.cpp \
../Vendor/imgui-master/imgui_widgets.cpp 

OBJS += \
./Vendor/imgui-master/imgui.o \
./Vendor/imgui-master/imgui_demo.o \
./Vendor/imgui-master/imgui_draw.o \
./Vendor/imgui-master/imgui_impl_opengl3.o \
./Vendor/imgui-master/imgui_impl_sdl.o \
./Vendor/imgui-master/imgui_widgets.o 

CPP_DEPS += \
./Vendor/imgui-master/imgui.d \
./Vendor/imgui-master/imgui_demo.d \
./Vendor/imgui-master/imgui_draw.d \
./Vendor/imgui-master/imgui_impl_opengl3.d \
./Vendor/imgui-master/imgui_impl_sdl.d \
./Vendor/imgui-master/imgui_widgets.d 


# Each subdirectory must supply rules for building sources it contributes
Vendor/imgui-master/%.o: ../Vendor/imgui-master/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


