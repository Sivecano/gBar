#pragma once
#include "Common.h"
#include "System.h"
#include "Config.h"

#include <cstdint>
#include <string>
#include <cstdlib>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#ifdef WITH_WORKSPACES
namespace Workspaces
{
    void Init();

    void PollStatus(const std::string& monitor, uint32_t numWorkspaces);

    System::WorkspaceStatus GetStatus(uint32_t workspaceId);

    uint32_t GetMaxUsedWorkspace();

    void Shutdown();

    // TODO: Use ext_workspaces for this, if applicable
    inline void Goto(uint32_t workspace)
    {
        if (RuntimeConfig::Get().hasWorkspaces == false)
        {
            LOG("Error: Called Go to workspace, but Workspaces isn't open!");
            return;
        }
        std::string cmd = "hyprctl dispatch 'hl.dsp.focus({ workspace = \""
            + std::to_string(workspace) + "\" })'";

        LOG("Switching workspace: " + cmd);
        system(cmd.c_str());
    }

    // direction: + or -
    inline void GotoNext(char direction)
    {
        if (RuntimeConfig::Get().hasWorkspaces == false)
        {
            return;
        }

        // Modern Hyprland expects "e+1" or "e-1" to target existing workspaces
        std::string target = "e" + std::string(1, direction) + "1";

        if (Config::Get().workspaceScrollOnMonitor)
        {
            // Fix: Use "e+1" combined with the monitor_bound boolean flag
            std::string cmd = "hyprctl dispatch 'hl.dsp.focus({ workspace = \""
                + target + "\", monitor_bound = true })'";
            LOG(" Scrolling non-empty workspaces (monitor bound): "
                << cmd.c_str());
            system(cmd.c_str());
        }
        else
        {
            // Global non-empty workspace scroll
            std::string cmd = "hyprctl dispatch 'hl.dsp.focus({ workspace = \""
                + target + "\" })'";
            LOG(" Scrolling non-empty workspaces globally: " << cmd.c_str());
            system(cmd.c_str());
        }
    }

}
#endif
