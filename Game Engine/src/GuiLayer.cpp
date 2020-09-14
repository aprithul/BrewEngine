#include "GuiLayer.hpp"
#include "Logger.hpp"
namespace PrEngine
{

    GuiLayer::GuiLayer(SDL_Window* sdl_window, SDL_GLContext* gl_context):window(sdl_window),gl_context(gl_context)
    {
        this->name = "GUI";
        panning = nullptr;
        tiling = nullptr;

    }

    GuiLayer::~GuiLayer()
    {
        
    }

    void GuiLayer::start()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        ImGui_ImplOpenGL3_Init("#version 410");
        
        //inspector initialization
        inspector_active = true;
        //ImGui::StyleColorsClassic();
    }


    void GuiLayer::update()
    {
        IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing dear imgui context. Refer to examples app!"); // Exceptionally add an extra assert here for people confused with initial dear imgui setup
        
        ImGuiIO& io = ImGui::GetIO();
        //io.DisplaySize = ImVec2( 1440, 900);
        //io.DisplayFramebufferScale
        io.DeltaTime = Time::Frame_time;
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();
        static bool show = true;
        //ImGui::ShowDemoWindow(&show);

        if(inspector_active)
        {
            ImGui::Begin("Inspector", &inspector_active, ImGuiWindowFlags_MenuBar);
            
            ImGui::Text("Panning:");
            if(panning!=nullptr)
            {
                ImGui::DragFloat("Pan X", &(panning->x), 0.01f);
                ImGui::DragFloat("Pan Y", &(panning->y),0.01f);
            }
            ImGui::Text("Tiling:");
            if(tiling!=nullptr)
            {
                ImGui::DragFloat("Tile X", &(tiling->x),0.01f);
                ImGui::DragFloat("Tile Y", &(tiling->y),0.01f);
            }
            ImGui::End();
        }

        //ImGui::ShowDemoWindow();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData());
    }

    void GuiLayer::end()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
    }
}