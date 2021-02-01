using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Kantar.MediaPlanning.Logger.Model
{

    public static class CommonSystem
    {
        public enum ServiceMode { StartService, StopService, PauseService, ContinueService };
        private static TraceLogger _traceLogger;                  // to do tracelog service

        public enum ReturnType
        {
            /// <summary>The operation is successfull</summary>
            Successfull,
            /// <summary>The operation has trigger an exception that require to stop the application.</summary>
            ExceptionAbort,
            /// <summary>The operation has trigger an exception that does not require to stop the application.</summary>
            ExceptionContinue,
            /// <summary>The operation has report a logical error (Rules) that require to notify the user.</summary>
            LogicalAbort,
            /// <summary>The operation has report a logical warning (Rules) that require to notify the user.</summary>
            LogicalContinue,
            /// <summary>Xml file script has no .doc filename</summary>
            ExceptionScriptWrongFormed,
            /// <summary> The Xml file is invalid </summary>
            ExceptionInvalidXml,
            /// <summary>file copy exception</summary>
            IOException,
            /// <summary>no hit exception</summary>
            ExceptionNoHit
        }

        #region Logger
        // ------------------------------------
        // Accessor to object Logger (ie trace)
        // ------------------------------------
        public static TraceLogger Logger
        {
            get
            {
                if (_traceLogger == null)
                {
                    _traceLogger = TraceLogger.Instance;
                    // -----------------------------
                    // reading settings of trace job
                    // -----------------------------
                    _traceLogger.SetupTrace();
                }
                return _traceLogger;
            }
        }
        #endregion
    }

}
