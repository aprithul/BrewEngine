################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AudioManager.cpp \
../src/Behaviour.cpp \
../src/Camera3D.cpp \
../src/Component.cpp \
../src/DirectionalLight.cpp \
../src/Engine.cpp \
../src/Entity.cpp \
../src/Entity3D.cpp \
../src/EntityManagementSystemModule.cpp \
../src/FrameRateCounterModule.cpp \
../src/FrameRateRegulatorModule.cpp \
../src/Game.cpp \
../src/GeometryLayer.cpp \
../src/GlAssert.cpp \
../src/Graphics.cpp \
../src/GuiLayer.cpp \
../src/InputModule.cpp \
../src/Logger.cpp \
../src/Material.cpp \
../src/Matrix3x3f.cpp \
../src/Mesh.cpp \
../src/Module.cpp \
../src/NetworkManager.cpp \
../src/RenderLayer.cpp \
../src/RendererOpenGL.cpp \
../src/Sprite.cpp \
../src/SpriteLayer.cpp \
../src/Texture.cpp \
../src/TimeModule.cpp \
../src/Transform.cpp \
../src/Transform3D.cpp \
../src/Utils.cpp \
../src/Vertex.cpp \
../src/main.cpp 

OBJS += \
./src/AudioManager.o \
./src/Behaviour.o \
./src/Camera3D.o \
./src/Component.o \
./src/DirectionalLight.o \
./src/Engine.o \
./src/Entity.o \
./src/Entity3D.o \
./src/EntityManagementSystemModule.o \
./src/FrameRateCounterModule.o \
./src/FrameRateRegulatorModule.o \
./src/Game.o \
./src/GeometryLayer.o \
./src/GlAssert.o \
./src/Graphics.o \
./src/GuiLayer.o \
./src/InputModule.o \
./src/Logger.o \
./src/Material.o \
./src/Matrix3x3f.o \
./src/Mesh.o \
./src/Module.o \
./src/NetworkManager.o \
./src/RenderLayer.o \
./src/RendererOpenGL.o \
./src/Sprite.o \
./src/SpriteLayer.o \
./src/Texture.o \
./src/TimeModule.o \
./src/Transform.o \
./src/Transform3D.o \
./src/Utils.o \
./src/Vertex.o \
./src/main.o 

CPP_DEPS += \
./src/AudioManager.d \
./src/Behaviour.d \
./src/Camera3D.d \
./src/Component.d \
./src/DirectionalLight.d \
./src/Engine.d \
./src/Entity.d \
./src/Entity3D.d \
./src/EntityManagementSystemModule.d \
./src/FrameRateCounterModule.d \
./src/FrameRateRegulatorModule.d \
./src/Game.d \
./src/GeometryLayer.d \
./src/GlAssert.d \
./src/Graphics.d \
./src/GuiLayer.d \
./src/InputModule.d \
./src/Logger.d \
./src/Material.d \
./src/Matrix3x3f.d \
./src/Mesh.d \
./src/Module.d \
./src/NetworkManager.d \
./src/RenderLayer.d \
./src/RendererOpenGL.d \
./src/Sprite.d \
./src/SpriteLayer.d \
./src/Texture.d \
./src/TimeModule.d \
./src/Transform.d \
./src/Transform3D.d \
./src/Utils.d \
./src/Vertex.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


