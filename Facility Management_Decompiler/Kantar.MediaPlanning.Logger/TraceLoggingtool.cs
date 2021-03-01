using Kantar.MediaPlanning.Logger.Model;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace Kantar.MediaPlanning.Logger
{
    public static class TraceLoggingtool
    {
        // to do tracelog service
        private static TraceLogger m_TraceLogger;

        public static TraceLogger Logger
        {
            get
            {
                if (m_TraceLogger == null)
                {
                    m_TraceLogger = TraceLogger.Instance;
                    // -----------------------------
                    // reading settings of trace job
                    // -----------------------------
                    m_TraceLogger.SetupTrace();
                }
                return m_TraceLogger;
            }
        }


        #region General

        public static void LogError(string oneMessage, Exception oneEx)
        {
            string msg;
            TraceLogger.Instance.WriteLine("---------------------------------------");
            msg = string.Format("[...] Exception Message [{0}] ", oneEx.Message);
            Logger.WriteLine(msg);
            msg = string.Format("[...] Exception Stack [{0}] ", oneEx.StackTrace);
            Logger.WriteLine(msg);
            Logger.WriteLine("---------------------------------------");
        }

        internal static void LogChangeMode(object startService)
        {
            throw new NotImplementedException();
        }

        public static void LogServiceManagementError(Exception pEx)
        {
            string msg;
            TraceLogger.Instance.WriteLine("---------------------------------------");
            msg = "[...] Error on restart services";
            Logger.WriteLine(msg);
            msg = string.Format("[...] Exception Stack [{0}] ", pEx.StackTrace);
            Logger.WriteLine(msg);
            TraceLogger.Instance.WriteLine("---------------------------------------");
        }

        public static void WriteLineIf(TraceLevel oneTraceLevel, string oneMessage)
        {
            Logger.WriteLineIf(oneTraceLevel, oneMessage);
        }

        public static void WriteLine(string oneMessage)
        {
            Logger.WriteLine(oneMessage);
        }

        public static void LogChangeMode(CommonSystem.ServiceMode pChangeMode)
        {
            string tVersion = string.Empty;

            TraceLoggingtool.WriteLineIf(TraceLevel.Verbose, "--------------------------------------------------------------------------------------");
            switch (pChangeMode)
            {
                case CommonSystem.ServiceMode.StartService:
                    TraceLoggingtool.WriteLineIf(TraceLevel.Info, "Service has Started...");

                    tVersion = Assembly.GetExecutingAssembly().GetName().Version.ToString();
                    TraceLoggingtool.WriteLineIf(TraceLevel.Info, string.Format("Version {0}", tVersion));

                    break;

                case CommonSystem.ServiceMode.StopService:
                    TraceLoggingtool.WriteLineIf(TraceLevel.Info, " Service has Stopped...");
                    break;

                case CommonSystem.ServiceMode.PauseService:
                    TraceLoggingtool.WriteLineIf(TraceLevel.Info, " Service has Paused...");
                    break;

                case CommonSystem.ServiceMode.ContinueService:
                    TraceLoggingtool.WriteLineIf(TraceLevel.Info, " Service has Continue...");
                    break;

                default:
                    break;
            }
        }
        #endregion
    }
}
