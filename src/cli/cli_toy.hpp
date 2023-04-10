
/**
 * @file
 * this file contains definitions for a toy protocol CLI tool
 */


#include "cli/cli_config.h"
#include "cli/cli_output.hpp"
#include "common/time.hpp"
#include "common/timer.hpp"

namespace ot {
namespace Cli {

class Toy : private Output
{
public:
    typedef Utils::CmdLineParser::Arg Arg;

    /**
     * Constructor
     *
     * @param[in]  aInstance            The OpenThread Instance.
     * @param[in]  aOutputImplementer   An `OutputImplementer`.
     *
     */
    Toy(otInstance *aInstance, OutputImplementer &aOutputImplementer)
        : Output(aInstance, aOutputImplementer)
    {
    }

    /**
     * This method interprets a list of CLI arguments.
     *
     * @param[in]  aArgs        A pointer to an array of command line arguments.
     *
     */
    otError Process(Arg aArgs[]);
    void    SendToyMessage(void);

private:
    using Command = CommandEntry<Toy>;

    template <CommandId kCommandId> otError Process(Arg aArgs[]);

    static void HandleCallback(otError aError, void *aContext);
    void        HandleCallback(otError aError);
    void        HandleTimer(Timer &aTimer);
    void        StartTimer(uint32_t aTimeout);
    void        StopTimer(void);

    TimerMilli mTimer;
    uint32_t   mTimeout;
};

} // namespace Cli
} // namespace ot
