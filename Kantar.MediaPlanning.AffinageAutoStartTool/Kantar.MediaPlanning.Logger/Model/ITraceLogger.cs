using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Kantar.MediaPlanning.Logger.Model
{
    interface ITraceLogger
    {
        void SetupTrace();
        void SetupTrace(string defaultTraceSwitchName);
        TraceLevel Level { get; }
        TraceSwitch DefaultTraceSwitch { get; }
        void WriteLineIf(TraceLevel oneLevel, string oneMessage);
        void WriteLine(string oneMessage);
    }
}
