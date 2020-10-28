// © 2020 Joseph Cameron - All Rights Reserved
#include <jfc/travis_ci_canary/notification.h>

#include <iostream>
#include <mutex>

#include <gtk/gtk.h>

using namespace jfc::travis_ci_canary::notify;

static GApplication *pApplication;

static GNotification *pNotification;

void static build_failed_notification_behaviour(GSimpleAction*, GVariant*, gpointer)
{
    std::cout << "TODO: open build page for the corresponding build\n";
}

void static init_once()
{
    static std::once_flag flag;

    std::call_once(flag, []()
    {
        pApplication = []()
        {
            auto p = (g_application_new("jfcameron.github.travis_ci_canary", G_APPLICATION_FLAGS_NONE));
            
            return p ? p : throw std::runtime_error("could not initialize the g_application");
        }();

        pNotification = []()
        {
            auto p = (g_notification_new("Build failed"));

            return p ? p : throw std::runtime_error("could not initialize g_notification");
        }();

        g_application_register(pApplication, nullptr, nullptr);

        static const GActionEntry ACTIONS[] = {
            { "build.failed", build_failed_notification_behaviour}
        };

        g_action_map_add_action_entries (G_ACTION_MAP (pApplication), ACTIONS, 
                G_N_ELEMENTS (ACTIONS), pApplication);

        g_notification_set_default_action(pNotification, "app.build.failed");
    });
}

void jfc::travis_ci_canary::notify::build_state_changed(const std::string &aSlug, state_type state)
{
    init_once();

    switch (state)
    {
        case state_type::succeeded: g_notification_set_title(pNotification, "succeeded"); break;
        case state_type::cancelled: g_notification_set_title(pNotification, "cancelled"); break;
        case state_type::failed: g_notification_set_title(pNotification, "failed"); break;
        case state_type::building: g_notification_set_title(pNotification, "building"); break; 
    }

    g_notification_set_body(pNotification, aSlug.c_str());

    g_application_send_notification(pApplication, "notification", pNotification);
}

void jfc::travis_ci_canary::notify::connection_state_changed(connection_state_type state)
{
    init_once();

    std::cout << "connection_state_changed unimplemented\n";
}

