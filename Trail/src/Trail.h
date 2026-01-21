#pragma once


//for use by Trail applications, not the Trail engine
#include "Trail/Application.h"
#include "Trail/Layer.h"
#include "Trail/Log.h"
#include "Trail/Input.h"
#include "Trail/KeyCodes.h"
#include "Trail/MouseButtonCodes.h"
#include "Trail/Core/DeltaTime.h"
#include <glm/glm.hpp>
#include <stdio.h>
#include "Trail/ImGui/ImGuiLayer.h"

//-----Renderer---------------------
#include "Trail/Renderer/Renderer.h"
#include "Trail/Renderer/Camera.h"
#include "Trail/Renderer/Buffer.h"
#include "Trail/Renderer/Shader.h"
#include "Trail/Renderer/Texture.h"
//-----Renderer---------------------

//----------AUDIO-------------------
#include "Trail/SoundRenderer/SoundBuffer.h"
#include "Trail/SoundRenderer/SoundDevice.h"
#include "Trail/SoundRenderer/SoundSource.h"
//----------AUDIO-------------------