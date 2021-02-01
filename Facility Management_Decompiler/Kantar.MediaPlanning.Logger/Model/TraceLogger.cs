using System;
using System.Collections.Generic;
using System.Configuration;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Kantar.MediaPlanning.Logger.Model
{

    public class TraceLogger : ITraceLogger
    {
        #region Accessors
        /// <summary>
        /// ------------------
        /// Level of tracing
        /// read only accessor
        /// ------------------
        /// </summary>
        public TraceLevel Level
        {
            get
            {
                return DefaultTraceSwitch.Level;
            }
        }
        /// <summary>
        /// global trace swith for application
        /// </summary>
        public TraceSwitch DefaultTraceSwitch
        {
            get
            {
                if (TheDefaultTraceSwitch == null)
                {
                    TheDefaultTraceSwitch = new TraceSwitch(TheDefaultTraceSwitchName, "the default trace switch");
                }

                return TheDefaultTraceSwitch;
            }
        }
        /// <summary>
        /// default name of the global trace switch
        /// see in App.Config
        /// <code>
        ///		<system.diagnostics>
        /// 		<switches>
        ///				<!-- values are 0 for None, 1 for Error, 2 for Warning, 3 for Info, 4 for Verbose /-->
        ///				<add name="Default" value="2" /><!-- replace here Default by myAppTraceSwitchName /-->
        ///			</switches>
        ///		</system.diagnostics>
        ///	</code>
        /// </summary>
        public string DefaultTraceSwitchName
        {
            get { return TheDefaultTraceSwitchName; }
            set { TheDefaultTraceSwitchName = value; }
        }

        /// <summary>
        /// get the unique instance of thsi class (Singleton)
        /// </summary>
        public static TraceLogger Instance
        {
            get
            {
                if (TheTraceLogger == null)
                    TheTraceLogger = new TraceLogger();

                return TheTraceLogger;
            }
        }
        #endregion
        #region Variables
        private static TraceLogger TheTraceLogger = null;
        private static string TheDefaultTraceSwitchName = "Default";
        private static TraceSwitch TheDefaultTraceSwitch = null;
        private TraceListener TheTraceListener;
        #endregion
        #region Constructor
        /// <summary>
        /// ------------------
        /// Default contructor
        /// ------------------
        /// </summary>
        private TraceLogger()
        {
        }
        #endregion

        public void Flush()
        {
            TheTraceListener.Flush();
        }

        /// <summary>
        /// -----------------------------------------------------
        /// Initialising tracing function
        /// must be the first use of Logger object in application
        /// -----------------------------------------------------
        /// </summary>
        public void SetupTrace(string defaultTraceSwitchName)
        {
            DefaultTraceSwitchName = defaultTraceSwitchName;
            SetupTrace();
        }
        /// <summary>
        /// -----------------------------------------------------
        /// Initialising tracing function
        /// must be the first use of Logger object in application
        /// -----------------------------------------------------
        /// </summary>
        public void SetupTrace()
        {
            // ---------------------------------
            // Reading Configuration setting for 
            // directory and file Name
            // ---------------------------------
            string TheTraceLogfile = ConfigurationManager.AppSettings["TraceLogPath"];
            // ---------------------------------
            // creating the folder if don't exist
            // ---------------------------------
            if (!Directory.Exists(TheTraceLogfile))
            {
                Directory.CreateDirectory(TheTraceLogfile);
            }
            // ----------------------------------------------------
            // searching number max of log file allowed in settings
            // ----------------------------------------------------
            int maxLogFile = Convert.ToInt16(ConfigurationManager.AppSettings["MaxLogFile"]);
            // ----------------------------------------------------
            // searching all existing logfiles 
            // ----------------------------------------------------
            string[] TheFiles = Directory.GetFiles(TheTraceLogfile, "*.txt");
            // -------------------------------------------
            // deleting the older excessive number of files
            // -------------------------------------------
            while (TheFiles.Length > (maxLogFile - 1))
            {
                FileInfo ReferenceFileInfo = new FileInfo(TheFiles[0]);
                foreach (string OneFile in TheFiles)
                {
                    FileInfo CurrentFileInfo = new FileInfo(OneFile);
                    if (CurrentFileInfo.LastWriteTime < ReferenceFileInfo.LastWriteTime)
                    {
                        ReferenceFileInfo = CurrentFileInfo;
                    }
                }
                // ------------------
                //Delete old Log File
                // ------------------
                if (ReferenceFileInfo != null)
                {
                    ReferenceFileInfo.Delete();
                }
                TheFiles = Directory.GetFiles(TheTraceLogfile, "*.txt");
            }
            // -------------------------------------------------------------------------
            // creating new  filename  for trace file, example : 20060221_INFO_DEV01.txt
            // -------------------------------------------------------------------------
            TheTraceLogfile += DateTime.Today.ToString("yyyyMMdd") + "_" + System.Environment.MachineName + ".txt";
            // ----------------------
            // Trace must be active ?
            // ----------------------
            if (DefaultTraceSwitch.Level != TraceLevel.Off)
            {
                // -----------------------------------------
                // Searching name of listener in appsettings
                // -----------------------------------------
                string ListenerName = ConfigurationManager.AppSettings["TraceListenerName"];

                // ---------------
                // listener exists
                // ---------------
                if (TheTraceListener != null)
                {
                    Trace.Listeners.Remove(TheTraceListener);
                    TheTraceListener.Close();
                }

                // ---------
                // create it
                // ---------
                TheTraceListener = new TextWriterTraceListener(TheTraceLogfile, ListenerName);
                Trace.Listeners.Add(TheTraceListener);
            }

#if DEBUG
            // ----------------
            //Output to Console 
            // ----------------
            Trace.Listeners.Add(new TextWriterTraceListener(Console.Out));
#endif
        }
        /// <summary>
        /// -----------------------------------------
        /// write a line with a message in trace file 
        /// at every level of trace
        /// -----------------------------------------
        /// </summary>
        /// <param name="oneMessage"></param>
        public void WriteLine(string oneMessage)
        {
            string txt2Print;

            txt2Print = string.Format("[{0} {1}] {2}", System.DateTime.Now.ToShortDateString(), System.DateTime.Now.ToString("HH:mm:ss"), oneMessage);

            Trace.WriteLine(txt2Print);
        }

        /// <summary>
        /// -----------------------------------------
        /// write a line with a message in trace file 
        /// on one level of tracing
        /// -----------------------------------------
        /// </summary>
        /// <param name="OneLevel">the needed level of tracing</param>
        /// <param name="OneMessage">the text to write</param>
        public void WriteLineIf(TraceLevel oneLevel, string oneMessage)
        {
            string txt2Print;

            txt2Print = string.Format("[{0} {1}] {2}", System.DateTime.Now.ToShortDateString(), System.DateTime.Now.ToString("HH:mm:ss"), oneMessage);

            switch (oneLevel)
            {
                case TraceLevel.Error:
                    Trace.WriteLineIf(DefaultTraceSwitch.TraceError, txt2Print);
                    break;
                case TraceLevel.Info:
                    Trace.WriteLineIf(DefaultTraceSwitch.TraceInfo, txt2Print);
                    break;
                case TraceLevel.Verbose:
                    Trace.WriteLineIf(DefaultTraceSwitch.TraceVerbose, txt2Print);
                    break;
                case TraceLevel.Warning:
                    Trace.WriteLineIf(DefaultTraceSwitch.TraceWarning, txt2Print);
                    break;
                default:
                    break;


            }
        }
    }
}
