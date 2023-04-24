//
// Created by light on 4/3/2023.

#include "cli_toy.hpp"
#include <cli/cli.hpp>
#include "core/common/instance.hpp"
#include "core/net/ip6.hpp"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#if OPENTHREAD_CONFIG_TOY_ENABLE

    static otInstance *sInstance = NULL;

void ot::Cli::Toy::SendToyMessage()
{
    otMessage *message;
    otMessageInfo messageInfo;

    message = otIp6NewMessage(sInstance, NULL);
    if (message != NULL)
    {
        otIp6Address ip6Address;
        otIp6GetLinkLocalAddress(sInstance, &ip6Address);
        otIp6AddressSetPrefix(&ip6Address, OT_IP6_ADDRESS_SCOPE_LINK_LOCAL);

        otMessageInfoInit(&messageInfo);
        messageInfo.mPeerAddr = ip6Address;
        otMessageAppend(message, "hello", sizeof("hello") - 1);
        otError error = otIp6Send(sInstance, message, &messageInfo);

        if (error != OT_ERROR_NONE)
        {
            printf("Error: failed to send message (%d)\n", error);
        }
    }
}

void ot::Cli::Toy::StartTimer(uint32_t aTimeout)
{
    mTimer.Start(aTimeout);
}

void ot::Cli::Toy::StopTimer()
{
    mTimer.Stop();
}

void ot::Cli::Toy::HandleTimer(TimerMilli &aTimer, void *aContext)
{
    Toy *obj = static_cast<Toy *>(aContext);
    obj->SendToyMessage();
    aTimer.Start(1000);
}

otError ot::Cli::Toy::ProcessCommand(const char *aCommand)
{
    if (strcmp(aCommand, "toy start") == 0)
    {
        AppendResult("Starting to send 'hello' every second until the command 'toy stop' is run.\n");
        StartTimer(1000);
        return OT_ERROR_NONE;
    }
    else if (strcmp(aCommand, "toy stop") == 0)
    {
        AppendResult("Stopping 'hello' messages.\n");
        StopTimer();
        return OT_ERROR_NONE;
    }
    return OT_ERROR_NOT_FOUND;
}

static void HandleCommand(otCliCommand *aCommand)
{
    static ot::Cli::Toy toy(sInstance, *otCliOutputGet(sInstance));
    otError error = toy.ProcessCommand(aCommand->mCommandName);

    if (error == OT_ERROR_NOT_FOUND)
    {
        otCliOutputFormat("Error: command not found\n");
    }
}

#endif // OPENTHREAD_CONFIG_TOY_ENABLE


//#include "cli_toy.hpp"
//#include <cli/cli.hpp>
//
//#include <string.h>
//#include <stdio.h>
//#include <stdlib.h>
//
//#if OPENTHREAD_CONFIG_TOY_ENABLE
//
//static otInstance *sInstance = NULL; //pointer to openthread instance initialized as null
//static otTimer mTimer; // timer used to keep track of when messages are sent
//
//
////This is a callback function that will be called by the timer
//// whenever it expires. It creates an OpenThread message with a
//// "hello" payload, sets the destination address, and sends it using the OpenThread stack.
//static void TimerCallback(otTimer *aTimer, void *aContext)
//{
//    (void)aTimer;
//    (void)aContext;
//
//    otMessage *message;
//    otMessageInfo messageInfo;
//
//    message = otIp6NewMessage(sInstance, NULL);
//    if (message != NULL)
//    {
//        otIp6Address ip6Address;
//        otIp6GetLinkLocalAddress(sInstance, &ip6Address);
//        otIp6AddressSetPrefix(&ip6Address, OT_IP6_ADDRESS_SCOPE_LINK_LOCAL);
//
//        otMessageInfoInit(&messageInfo);
//        messageInfo.mPeerAddr = ip6Address;
//        otMessageAppend(message, "hello", sizeof("hello") - 1);
//        otError error = otIp6Send(sInstance, message, &messageInfo);
//
//        if (error != OT_ERROR_NONE)
//        {
//            printf("Error: failed to send message (%d)\n", error);
//        }
//    }
//}
//
//
////This is a callback function that will be called whenever
//// a CLI command is received. It checks if the command is
//// "toy start" or "toy stop" and starts or stops the timer accordingly.
//static void HandleCommand(otCliCommand *aCommand)
//{
//    if (strcmp(aCommand->mCommandName, "toy") == 0 && strcmp(aCommand->mSubcommands[0]->mCommandName, "start") == 0)
//    {
//        otCliOutputFormat("Starting to send 'hello' every few seconds until the command 'toy stop' is run.\n");
//
//        otCliOutputFormat("Creating timer.\n");
//
//        otInstance *instance = otThreadGetDeviceRole(sInstance) == OT_DEVICE_ROLE_LEADER ? sInstance : NULL;
//
//        otError error = otTimerStart(&mTimer, otThreadGetClock(sInstance), 1000, TimerCallback, instance);
//        if (error != OT_ERROR_NONE)
//        {
//            otCliOutputFormat("Error: failed to start timer (%d)\n", error);
//            return;
//        }
//
//        otCliOutputFormat("Timer started.\n");
//    }
//    else if (strcmp(aCommand->mCommandName, "toy") == 0 && strcmp(aCommand->mSubcommands[0]->mCommandName, "stop") == 0)
//    {
//        otCliOutputFormat("Stopping 'hello' messages.\n");
//
//        otError error = otTimerStop(&mTimer);
//        if (error != OT_ERROR_NONE)
//        {
//            otCliOutputFormat("Error: failed to stop timer (%d)\n", error);
//            return;
//        }
//
//        otCliOutputFormat("Timer stopped.\n");
//    }
//}
//
//
//int main(int argc, char *argv[])
//{
//    otError error;
//
//    // Initialize OpenThread
//    //This initializes an OpenThread instance for
//    // single-threaded use and assigns the instance to the sInstance pointer.
//    error = otInstanceInitSingleThreaded(&sInstance, NULL);
//    if (error != OT_ERROR_NONE)
//    {
//        printf("Error: failed to initialize OpenThread instance (%d)\n", error);
//        return 1;
//    }
//
//    // Initialize CLI
//    //This initializes the CLI interface for the OpenThread instance.
//    error = otCliUartInit(sInstance, NULL);
//    if (error != OT_ERROR_NONE)
//    {
//        printf("Error: failed to initialize CLI (%d)\n", error);
//        return 1;
//    }
//
//    otCliUartSetPrompt(sInstance, APP_NAME "> ");
//
//    // Register CLI command handler
//    //This registers the HandleCommand function
//    // as the command handler for the OpenThread instance.
//    otCliRegisterCommandHandler(sInstance, HandleCommand, NULL);
//
//    // Start OpenThread instance
//    error = otIp6SetEnabled(sInstance, true);
//    if (error != OT_ERROR_NONE)
//    {
//        printf("Error: failed to enable OpenThread instance (%d)\n", error);
//        return 1;
//    }
//
//    // Run CLI event loop
//    //This runs an infinite loop that processes CLI commands using the otCliUartProcess function.
//    while (true)
//    {
//        otCliUartProcess(sInstance);
//    }
//    return 0;
//}
//
//#endif //OPENTHREAD_CONFIG_TOY_ENABLE